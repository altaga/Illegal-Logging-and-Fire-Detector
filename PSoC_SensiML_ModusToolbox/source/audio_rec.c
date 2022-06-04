/******************************************************************************
* File Name:   Audio_rec.c
*
* Description: This file contains the implementation to read data from the PDM 
*              microphone. The data can also be fed to the SensiML knowloedge pack 
*              to run inferencing.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "cybsp.h"
#include "cyhal.h"
#include "cJSON.h"
#include "cy_retarget_io.h"
#include "audio_rec.h"
#include "cy_rgb_led.h"
#include "GUI.h"
#include "mtb_ssd1306.h"
#include "mtb_ssd1306_i2c.h"
#include "cy8ckit_032.h"
#include "images.h"

#if (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)
#include "kb.h"
#include "sml_recognition_run.h"
#endif // (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)


/*******************************************************************************
* Constants
*******************************************************************************/
#define AUDIO_REC_AUDIO_FREQUENCY    (16000U)
#define AUDIO_REC_CHANNEL_NUMBER     (1)
#define AUDIO_REC_RECORD_BUFFER_SIZE (64)
#define AUDIO_REC_RECORD_BYTE_SIZE    AUDIO_REC_RECORD_BUFFER_SIZE*2      
#define DECIMATION_RATE                 (96u)
#define AUDIO_SYS_CLOCK_HZ              (24576000u)
#define PDM_DATA                        (P10_5)
#define PDM_CLK                         (P10_4)
#define AUDIO_EVENT_BIT 1u<<3
#define I2C_SPEED                   (400000)

/*******************************************************************************
* Typedefs
*******************************************************************************/
struct {
    uint32_t channel_number;
    uint32_t audio_frequency;
    uint32_t record_buffer_size;
    bool is_recording;
} audio_rec;

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Event set when audio is done collecting */
cy_event_t audio_event;
uint32_t audio_event_bits  = AUDIO_EVENT_BIT;

/* PDM/PCM Configuration variables */

cyhal_pdm_pcm_t pdm_pcm;
cyhal_clock_t   audio_clock;
cyhal_clock_t   pll_clock;

const cyhal_pdm_pcm_cfg_t pdm_pcm_cfg =
{
    .sample_rate     = AUDIO_REC_AUDIO_FREQUENCY,
    .decimation_rate = DECIMATION_RATE,
    .mode            = CYHAL_PDM_PCM_MODE_LEFT,
    .word_length     = 16,  /* bits */
    .left_gain       = 21,   /* dB */
    .right_gain      = 21,   /* dB */
};

static int16_t ping_pong_buffer[2][AUDIO_REC_RECORD_BUFFER_SIZE];
static int32_t last_read_index = -1;
static int32_t read_index = -1;
static int32_t write_index = 1;


/*******************************************************************************
* Local Functions
*******************************************************************************/
void pdm_pcm_isr_handler(void *arg, cyhal_pdm_pcm_event_t event);

void OledStart(void);
void ShowStartupScreen(void);

/*******************************************************************************
* Function Name: pdm_pcm_clock_init
********************************************************************************
* Summary:
*   Initialize the PLL clock to feed the PDM/PCM block
*
* Parameters:
*     None
*
* Return:
*   The status of the initialization.
*******************************************************************************/
cy_rslt_t pdm_pcm_clock_init(void) {
    cy_rslt_t result;
    result = cyhal_clock_get(&pll_clock, &CYHAL_CLOCK_PLL[1]);

    if (result == CY_RSLT_SUCCESS) {
        result = cyhal_clock_init(&pll_clock);
    }

    if (result == CY_RSLT_SUCCESS) {
        result = cyhal_clock_set_frequency(&pll_clock, AUDIO_SYS_CLOCK_HZ, NULL);
    }

    if (result == CY_RSLT_SUCCESS) {
        result = cyhal_clock_set_enabled(&pll_clock, true, true);
    }

    if (result == CY_RSLT_SUCCESS) {
        result = cyhal_clock_get(&audio_clock, &CYHAL_CLOCK_HF[1]);
    }

    if (result == CY_RSLT_SUCCESS) {
        result = cyhal_clock_init(&audio_clock);
    }

    if (result == CY_RSLT_SUCCESS) {
        result = cyhal_clock_set_source(&audio_clock, &pll_clock);
    }

    if (result == CY_RSLT_SUCCESS) {
        result = cyhal_clock_set_enabled(&audio_clock, true, true);
    }

    return result;
}

/*******************************************************************************
* Function Name: pdm_pcm_clock_init
********************************************************************************
* Summary:
*   Initialize Audio path by setting up clocks, callbacks and configurations 
*
* Parameters:
*     None
*
* Return:
*   The status of the initialization.
*******************************************************************************/
cy_rslt_t audio_rec_init(void) {
    cy_rslt_t result;
    result = pdm_pcm_clock_init();
    if (result != CY_RSLT_SUCCESS) {
        CY_ASSERT(0);
    }

    result = cyhal_pdm_pcm_init(&pdm_pcm, PDM_DATA, PDM_CLK, &audio_clock, &pdm_pcm_cfg);
    if (result != CY_RSLT_SUCCESS) {
        CY_ASSERT(0);
    }

    cyhal_pdm_pcm_register_callback(&pdm_pcm, pdm_pcm_isr_handler, NULL);
    cyhal_pdm_pcm_enable_event(&pdm_pcm, CYHAL_PDM_PCM_ASYNC_COMPLETE, CYHAL_ISR_PRIORITY_DEFAULT, true);

    // Configure asynchronous transfers to use DMA to free up the CPU during transfers
    result = cyhal_pdm_pcm_set_async_mode(&pdm_pcm, CYHAL_ASYNC_DMA,
                                                    CYHAL_DMA_PRIORITY_DEFAULT);

    CY_ASSERT(CY_RSLT_SUCCESS == result);


    audio_rec.channel_number = AUDIO_REC_CHANNEL_NUMBER;
    audio_rec.audio_frequency = AUDIO_REC_AUDIO_FREQUENCY;
    audio_rec.record_buffer_size = AUDIO_REC_RECORD_BUFFER_SIZE;

    return CY_RSLT_SUCCESS;
}

/*******************************************************************************
* Function Name: audio_rec_start
********************************************************************************
* Summary:
*   Starts streaming audio to the fifo buffer  
*
* Parameters:
*     None
*
* Return:
*   SUCCESS.
*******************************************************************************/
cy_rslt_t audio_rec_start(void) {
    cyhal_pdm_pcm_start(&pdm_pcm);

    cyhal_pdm_pcm_read_async(&pdm_pcm, &ping_pong_buffer[write_index][0], AUDIO_REC_RECORD_BUFFER_SIZE);

    audio_rec.is_recording = true;
    return CY_RSLT_SUCCESS;
}

/*******************************************************************************
* Function Name: audio_rec_stop
********************************************************************************
* Summary:
*   Stops streaming audio to the fifo buffer  
*
* Parameters:
*     None
*
* Return:
*   SUCCESS.
*******************************************************************************/
cy_rslt_t audio_rec_stop(void) {
    cyhal_pdm_pcm_stop(&pdm_pcm);
    audio_rec.is_recording = false;
    return CY_RSLT_SUCCESS;
}

/*******************************************************************************
* Function Name: audio_rec_get_new_buffer
********************************************************************************
* Summary:
*   Helper function to get which ping/pong buffer can be read from  
*
* Parameters:
*     None
*
* Return:
*   Pointer to the ping/pong buffer.
*******************************************************************************/
int16_t *audio_rec_get_new_buffer(void) {
    if (read_index == -1) {
        return NULL;
    }

    if (last_read_index == read_index) {
        return NULL;
    }

    last_read_index = read_index;

    return ping_pong_buffer[read_index];
}


/*******************************************************************************
* Function Name: pdm_pcm_isr_handler
********************************************************************************
* Summary:
*   Interrupt function for PDM/PCM block
*
* Parameters:
*     None
*
* Return:
*   None
*******************************************************************************/
void pdm_pcm_isr_handler(void *arg, cyhal_pdm_pcm_event_t event) {
    (void) arg;
    (void) event;
    cyhal_pdm_pcm_read_async(&pdm_pcm, &ping_pong_buffer[write_index][0], AUDIO_REC_RECORD_BUFFER_SIZE);
    cy_rtos_setbits_event(&audio_event, AUDIO_EVENT_BIT, true);
    read_index = write_index;
    write_index = (write_index == 1) ? 0 : 1;
    
}

/*******************************************************************************
* Function Name: audio_rec_deinit
********************************************************************************
* Summary:
*   Frees clocks used for PDM/PCM audio block
*
* Parameters:
*     None
*
* Return:
*   None
*******************************************************************************/

void audio_rec_deinit(void) {
    cyhal_clock_free(&audio_clock);
    cyhal_clock_free(&pll_clock);
}

/*******************************************************************************
* Function Name: print_audio_config_json
********************************************************************************
* Summary:
*   This function creates and prints the config structure needed for 
*   SensiML Data capture lab 
*
* Parameters:
*     None
*
* Return:
*   None
*******************************************************************************/
void print_audio_config_json(void)
{
    char *string = NULL;
    cJSON *sample_rate = NULL;
    cJSON *version = NULL;
    cJSON *samples_per_packet = NULL;
    cJSON *column_location = NULL;
    cJSON *Microphone = NULL;



    cJSON *config_json = cJSON_CreateObject();
    if (config_json == NULL)
    {
        goto end;
    }

    /* Create sample rate */
    sample_rate = cJSON_CreateNumber((unsigned int) 16000);
    if (sample_rate == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(config_json, "sample_rate", sample_rate);
    
    /* Create version (Version-1) */
    version = cJSON_CreateNumber(1);
    if (version == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(config_json, "version", version);

    /* Create samples per packet */
    samples_per_packet = cJSON_CreateNumber((unsigned int) AUDIO_REC_RECORD_BUFFER_SIZE);
    if (samples_per_packet == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(config_json, "samples_per_packet", samples_per_packet);

    /* Create Column location array */
    column_location = cJSON_CreateObject();
    if (column_location == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(config_json, "column_location", column_location);


    /* Create Accelerometer arrays */
    Microphone = cJSON_CreateNumber((unsigned int) 0);
    if (Microphone == NULL)
    {
        goto end;
    }
    cJSON_AddItemToObject(column_location, "Microphone", Microphone);

    // string = cJSON_Print(config_json);
    string = cJSON_PrintUnformatted(config_json);
    if (string == NULL)
    {
        printf( "Failed to print monitor.\n");
    }

end:
    cJSON_Delete(config_json);
    printf(string);
    printf("\r\n");
}



#if (APPLICATION_RUNNING_MODE == DATA_CAPTURE_RUNNING_MODE)
/*******************************************************************************
* Function Name: audio_task
********************************************************************************
* Summary:
*   A task used to get data from PDM microphone and send it over UART.
*
* Parameters:
*     arg: not used
*
*
*******************************************************************************/
void audio_task(void *arg)
{
    (void)arg;
    volatile uint32_t transmit_flag = 0;
    uint32_t read_len = 0;

    print_audio_config_json();
    cy_rtos_delay_milliseconds(500);

    for(;;)
    {
        /* Wait until there are enough samples from the accelerometer and the
         * gyroscope in the circular buffer */
        //cy_rtos_waitbits_event(&audio_event, &audio_event_bits, true, true, CY_RTOS_NEVER_TIMEOUT);
        // printf("h\r");
        /* Check for a connect/disconnect from host */
        read_len = cyhal_uart_readable(&cy_retarget_io_uart_obj);
        if(read_len > 0)
        {
            char read_buffer[20];
            memset(read_buffer, 0, 20);
            cyhal_uart_read(&cy_retarget_io_uart_obj, read_buffer, &read_len);
            // printf(read_buffer);
            // printf("\r\n");
            if(strcmp("connect",read_buffer) == 0)
            {
                transmit_flag = 1;


            }
            else if(strcmp("disconnect",read_buffer) == 0)
            {
                transmit_flag = 0;
                cy_rgb_led_off();

                print_audio_config_json();
                cy_rtos_delay_milliseconds(500);
            }
        }

        if(transmit_flag)
        {
        	cy_rtos_waitbits_event(&audio_event, &audio_event_bits, true, true, CY_RTOS_NEVER_TIMEOUT);
            int16_t *audio_buf = audio_rec_get_new_buffer();
            size_t write_size = AUDIO_REC_RECORD_BYTE_SIZE;
            // fwrite(audio_buf, AUDIO_REC_RECORD_BYTE_SIZE, 1, stdout);

            if(audio_buf != NULL){
              cy_rgb_led_on(CY_RGB_LED_COLOR_GREEN, CY_RGB_LED_MAX_BRIGHTNESS);
              cyhal_uart_write(&cy_retarget_io_uart_obj, audio_buf, &write_size);
              cy_rgb_led_off();
            }
            //fwrite(audio_buf, AUDIO_REC_RECORD_BYTE_SIZE, 1, stdout);


        }
        else
        {
        	print_audio_config_json();
			cy_rtos_delay_milliseconds(500);
        }
    }
}
#endif

#if (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)
/*******************************************************************************
* Function Name: audio_task
********************************************************************************
* Summary:
*   A task used to get data from PDM microphone and run the knowledge pack.
*
* Parameters:
*     arg: not used
*
*
*******************************************************************************/
void audio_task(void *arg)
{
    (void)arg;

    OledStart();

    printf("Running Audio Recognition\r\n");
    cy_rtos_delay_milliseconds(500);

    for(;;)
    {
        /* Wait until there are enough samples from the accelerometer and the
         * gyroscope in the circular buffer */
        cy_rtos_waitbits_event(&audio_event, &audio_event_bits, true, true, CY_RTOS_NEVER_TIMEOUT);
        // printf("h\r");
        int16_t *audio_buf = audio_rec_get_new_buffer();
        if(audio_buf != NULL){
          int result = sml_recognition_run(audio_buf, AUDIO_REC_RECORD_BUFFER_SIZE, 1);
          if(result != -1){
			printf("%d\r\n", result);
          }
        }
    }

}
#endif //#if (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)

/*******************************************************************************
* Function Name: audio_init
********************************************************************************
* Summary:
*   This function initializes the audio path and starts the audio pipeline
*
* Parameters:
*     None
*
* Return:
*   None
*******************************************************************************/
cy_rslt_t audio_init(void)
{
    cy_rslt_t result;
    result = audio_rec_init();
    if (result != CY_RSLT_SUCCESS) {
        printf("Audio init failed");
    }

    result = audio_rec_start();
    if (result != CY_RSLT_SUCCESS) {
        printf("Recording audio failed");
    }

    /* Create an event that will be set when data collection is done */
    cy_rtos_init_event(&audio_event);

    return(CY_RSLT_SUCCESS);
}

void OledStart(void){
	    cyhal_i2c_t i2c_obj;

	    /* Configuration to initialize the I2C block */
	    cyhal_i2c_cfg_t i2c_config = {
	        .is_slave = false,
	        .address = OLED_I2C_ADDRESS,
	        .frequencyhal_hz = I2C_SPEED
	    };
	    /* Initialize and configure the I2C to use with the OLED display */
	        cyhal_i2c_init( &i2c_obj, CY8CKIT_032_PIN_I2C_SDA,
	                                 CY8CKIT_032_PIN_I2C_SCL, NULL);
	        cyhal_i2c_configure(&i2c_obj, &i2c_config);
	        mtb_ssd1306_init_i2c(&i2c_obj);
	        GUI_Init();
	        ShowStartupScreen();
}

void ShowStartupScreen(void)
{
    /* Set foreground and background color and font size */
    GUI_SetFont(GUI_FONT_10S_1);
    GUI_SetColor(GUI_WHITE);
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_DrawBitmap(&bmCypress_Logo_1_BPP_Inv, 0, 0);
    GUI_SetTextAlign(GUI_TA_HCENTER);
    GUI_DispStringAt("Illegal Logging Detector", 64, 42);
}

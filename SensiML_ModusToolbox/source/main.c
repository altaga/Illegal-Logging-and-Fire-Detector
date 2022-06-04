#include <sensor.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "audio_rec.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cy_rgb_led.h"
#include "app_config.h"

#if (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)
#include "kb.h"
#endif // (APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE)


#define TASK_STACK_SIZE 			8096
#define TASK_PRIORITY   			2
#define IO5 						P9_6
#define IO6 						P9_7

#define I2C_SPEED                   (400000)

#define SINGLE_CHANNEL 1
#define ADC_EXAMPLE_MODE SINGLE_CHANNEL
#define VPLUS_CHANNEL_0             (P10_7)

enum ADC_CHANNELS
{
  CHANNEL_0 = 0,
  CHANNEL_1,
  NUM_CHANNELS
} adc_channel;

/*******************************************************************************
* Global Variables
********************************************************************************/
/* This enables RTOS aware debugging */
volatile int uxTopUsedPriority;

/*******************************************************************************
* Forward declaration
*******************************************************************************/
/* Function prototypes */

void adc_single_channel_init(void);

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/

cyhal_adc_t adc_obj;
cyhal_adc_channel_t adc_chan_0_obj;

const cyhal_adc_config_t adc_config = {
        .continuous_scanning=false, // Continuous Scanning is disabled
        .average_count=1,           // Average count disabled
        .vref=CYHAL_ADC_REF_VDDA,   // VREF for Single ended channel set to VDDA
        .vneg=CYHAL_ADC_VNEG_VSSA,  // VNEG for Single ended channel set to VSSA
        .resolution = 12u,          // 12-bit resolution
        .ext_vref = NC,             // No connection
        .bypass_pin = NC };       // No connection

int main(void)
{
    cy_rslt_t result;

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    adc_single_channel_init();
    cyhal_adc_configure(&adc_obj, &adc_config);

    cyhal_gpio_init(IO5 , CYHAL_GPIO_DIR_OUTPUT,
            					CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	cyhal_gpio_init(IO6, CYHAL_GPIO_DIR_OUTPUT,
    						CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	cy_rgb_led_init(CYBSP_LED_RGB_RED, CYBSP_LED_RGB_GREEN, CYBSP_LED_RGB_BLUE, CY_RGB_LED_ACTIVE_LOW);

    /* Enable global interrupts */
    __enable_irq();
#if APPLICATION_RUNNING_MODE == RECOGNITION_RUNNING_MODE
    kb_model_init();
#endif //APPLICATION_RUNNING_MODE RECOGNITION_RUNNING_MODE

    /* Initialize retarget-io to use the debug UART port */
    if(CY_RSLT_SUCCESS != cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, 1000000))
    {
        while(1);
    }
    #if(SENSOR_SELECT_MODE==SENSOR_MOTION)
        /* Create one task, sends motion sensor data via UART */
        xTaskCreate(sensor_task, "sensortask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    #elif(SENSOR_SELECT_MODE==SENSOR_AUDIO)
        /* Create one task, sends audio data via UART */
        xTaskCreate(audio_task, "audiotask", TASK_STACK_SIZE, NULL, TASK_PRIORITY, NULL);
    #endif
    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    for (;;)
    {
    }
}

void vApplicationDaemonTaskStartupHook(void)
{
    /* Initializes the inference engine */
    cy_rslt_t result;

    #if(SENSOR_SELECT_MODE==SENSOR_MOTION)
    result = sensor_init();
    if(CY_RSLT_SUCCESS != result)
    {
        /* Reset the system on sensor fail */
        NVIC_SystemReset();
    }
    #elif(SENSOR_SELECT_MODE==SENSOR_AUDIO)
    result = audio_init();
    if(CY_RSLT_SUCCESS != result)
    {
        /* Reset the system on sensor fail */
        NVIC_SystemReset();
    }
    #endif
}

void adc_single_channel_init(void)
{
    cy_rslt_t result;
    result = cyhal_adc_init(&adc_obj, VPLUS_CHANNEL_0, NULL);
    if(result != CY_RSLT_SUCCESS)
    {
        printf("ADC initialization failed. Error: %ld\n", (long unsigned int)result);
        CY_ASSERT(0);
    }
    const cyhal_adc_channel_config_t channel_config = {
            .enable_averaging = false,  // Disable averaging for channel
            .min_acquisition_ns = 1000, // Minimum acquisition time set to 1us
            .enabled = true };          // Sample this channel when ADC performs a scan
    result  = cyhal_adc_channel_init_diff(&adc_chan_0_obj, &adc_obj, VPLUS_CHANNEL_0,
                                          CYHAL_ADC_VNEG, &channel_config);
    if(result != CY_RSLT_SUCCESS)
    {
        printf("ADC single ended channel initialization failed. Error: %ld\n", (long unsigned int)result);
        CY_ASSERT(0);
    }
}

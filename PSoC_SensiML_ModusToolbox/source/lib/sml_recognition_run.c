#include "kb.h"
#include "kb_defines.h"
#include "testdata.h"
#include "kb_debug.h"
#include "cy_pdl.h"
#include "cy_retarget_io.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_rgb_led.h"

#define IO5 						P9_6
#define IO6 						P9_7

int humanCounter = 0;

static char str_buffer[2048];
static uint8_t fv_arr[MAX_VECTOR_SIZE];

extern cyhal_adc_channel_t adc_chan_0_obj;

#if SML_PROFILER
float recent_fv_times[MAX_VECTOR_SIZE];
unsigned int recent_fv_cycles[MAX_VECTOR_SIZE];
#endif

void sml_output_results(int model_index, int model_result)
{
    kb_print_model_result(model_index, model_result, str_buffer, 1, fv_arr);
    printf("%s\r\n", str_buffer);

    int32_t adc_result_0 = 0;

    adc_result_0 = cyhal_adc_read_uv(&adc_chan_0_obj);

    if(model_result == 2){
    	humanCounter=5;
    	cyhal_gpio_write(IO5, CYBSP_LED_STATE_OFF);
    	cyhal_gpio_write(IO6, CYBSP_LED_STATE_ON);
    	cy_rgb_led_on(CY_RGB_LED_COLOR_BLUE, 50u);
    }
    if(humanCounter>0)
	{
		humanCounter--;
		cyhal_gpio_write(IO5, CYBSP_LED_STATE_OFF);
		cyhal_gpio_write(IO6, CYBSP_LED_STATE_ON);
		cy_rgb_led_on(CY_RGB_LED_COLOR_BLUE, 50u);
	}
    else if(1000000 < adc_result_0)
    {
		cyhal_gpio_write(IO5, CYBSP_LED_STATE_OFF);
		cyhal_gpio_write(IO6, CYBSP_LED_STATE_OFF);
		cy_rgb_led_on(CY_RGB_LED_COLOR_RED, 50u);
	}
    else if(model_result == 1)
    {
		cyhal_gpio_write(IO5, CYBSP_LED_STATE_ON);
		cyhal_gpio_write(IO6, CYBSP_LED_STATE_OFF);
		cy_rgb_led_on(CY_RGB_LED_COLOR_RED, 50u);
	}
    else if(model_result == 3)
    {
    	cyhal_gpio_write(IO5, CYBSP_LED_STATE_ON);
    	cyhal_gpio_write(IO6, CYBSP_LED_STATE_ON);
    	cy_rgb_led_on(CY_RGB_LED_COLOR_GREEN, 10u);
    }
    #if SML_PROFILER
    memset(str_buffer, 0, 2048);
    kb_print_model_cycles(model_index, model_result, str_buffer, recent_fv_cycles);
    printf("%s\r\n", str_buffer);
    memset(str_buffer, 0, 2048);
    kb_print_model_times(model_index, model_result, str_buffer, recent_fv_times);
    printf("%s\r\n", str_buffer);
    #endif

};


int sml_recognition_run(int16_t *data, int batch_sz, uint8_t num_sensors)
{
    int ret = 0;
    int index = 0;
    SENSOR_DATA_T * pData;

        for(index = 0; index < batch_sz; index++)
        {
        	pData = (SENSOR_DATA_T*)&data[index];

			ret = kb_run_model((SENSOR_DATA_T *)pData, num_sensors, 0);
			if (ret >= 0)
			{
				sml_output_results(0, ret);
				kb_reset_model(0);
			};

        }
    return(ret);
}


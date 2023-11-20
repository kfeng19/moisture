#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "moisture.h"

const char* TAG="MoistureMain";
double y = 0;

// ADC constants all defined in hal/adc_types.h
void app_main(void)
{
    adc_oneshot_unit_handle_t adc_handle = config_adc(ADC_CHANNEL_4);
    while (1)
    {
        y = measure_moisture(&adc_handle, ADC_CHANNEL_4);
        ESP_LOGI(TAG, "Measured: %.2f", y);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

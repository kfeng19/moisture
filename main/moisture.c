#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

static int adc_raw;
const char* TAG="lol";
double alpha;
double y = 0;

// dry air ~ 2870

// deep measure: surface level between "sensor" and "v1.2"; water ~ 1344

// shallow measure: level between "Moisture" and "sensor"; water ~ 1520

// tip measure: surface level betwee "Moi" and "sture"; water ~ 2000

// #define SHALLOW_MEASURE
// #define DEEP_MEASURE
#define TIP_MEASURE

#ifdef DEEP_MEASURE

const double k = -0.06553;
const double b = 188.1;

#elif defined(SHALLOW_MEASURE)

const double k = -0.07407;
const double b = 212.6;

#elif defined(TIP_MEASURE)

const double k = -0.1149;
const double b = 329.9;

#else

#error Please specify DEEP_MEASURE or SHALLOW_MEASURE mode for the sensor depth

#endif

double p;

const int n_avg = 10;


// ADC constants all defined in hal/adc_types.h
void app_main(void)
{
    double sum = 0;

    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_4, &config));
    while (1)
    {
        sum = 0;
        for (int count = 1;count<n_avg + 1;count++)
        {
            // Read raw
            ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_4, &adc_raw));
            // ESP_LOGI(TAG, "ADC raw read: %d", adc_raw);
            // p = adc_raw;
            p = k * adc_raw + b;
            sum += p;
        }
        y = sum / n_avg;
        y = y < 0 ? 0 : y;
        ESP_LOGI(TAG, "Filtered: %.2f", y);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

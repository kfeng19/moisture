#include "moisture.h"

// dry air ~ 2870

// deep measure: surface level between "sensor" and "v1.2"; water ~ 1344

// shallow measure: level between "Moisture" and "sensor"; water ~ 1520

// tip measure: surface level betwee "Moi" and "sture"; water ~ 2000

#ifdef CONFIG_DEEP_MEASUREMENT

const double k = -0.06553;
const double b = 188.1;

#elif defined(CONFIG_SHALLOW_MEASUREMENT)

const double k = -0.07407;
const double b = 212.6;

#elif defined(CONFIG_TIP_MEASUREMENT)

const double k = -0.1149;
const double b = 329.9;

#else

#error Please specify measurement depth mode through menuconfig

#endif

const int n_avg = 10;


adc_oneshot_unit_handle_t config_adc(adc_channel_t adc_channel){  
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, adc_channel, &config));
    return adc1_handle;
}

double measure_moisture(adc_oneshot_unit_handle_t *adc1_handle, adc_channel_t adc_channel){
    int adc_raw;
    double p;
    double y = 0;
    double sum = 0;
    for (int count = 1;count<n_avg + 1;count++)
    {
        // Read raw
        ESP_ERROR_CHECK(adc_oneshot_read(*adc1_handle, adc_channel, &adc_raw));
        // ESP_LOGI(TAG, "ADC raw read: %d", adc_raw);
        // p = adc_raw;
        p = k * adc_raw + b;
        sum += p;
    }
    y = sum / n_avg;
    return y < 0 ? 0 : y;
    
}
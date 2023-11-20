
#include "esp_adc/adc_oneshot.h"


adc_oneshot_unit_handle_t config_adc(adc_channel_t adc_channel);

double measure_moisture(adc_oneshot_unit_handle_t*, adc_channel_t);
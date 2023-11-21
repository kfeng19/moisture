
#include "esp_adc/adc_oneshot.h"

// Create a ADC handle
adc_oneshot_unit_handle_t config_adc(adc_channel_t adc_channel);

// Take one averaged moisture measurement
double measure_moisture(adc_oneshot_unit_handle_t*, adc_channel_t);

// Take one calibration / raw measurement
void calibrate_measure(adc_oneshot_unit_handle_t *handle, adc_channel_t channel);
#include "ultrasonic_sensor_handler.h"
#include "adc.h"
#include "delay.h"
#include "gpio.h"

void ultrasonic_sensor_handler__initialize_sensors(void) {
  gpio__construct_with_function(GPIO__PORT_0, 25, GPIO__FUNCTION_1); // left: channel 2
  gpio__construct_with_function(GPIO__PORT_1, 30, GPIO__FUNCTION_1); // right: channel 4
  gpio__construct_with_function(GPIO__PORT_1, 31, GPIO__FUNCTION_1); // front: channel 5
  gpio__construct_with_function(GPIO__PORT_0, 26, GPIO__FUNCTION_1); // back: channel 3
  adc__initialize();
}

uint16_t ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(uint16_t adc_value) {
  float range_in_cm = 0;

  range_in_cm = adc_value / 8;

  return range_in_cm;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_left(void) {
  uint16_t sensor_value = 0;
  uint16_t converted_sensor_value = 0;

  sensor_value = adc__get_adc_value(ADC__CHANNEL_2);
  converted_sensor_value = ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(sensor_value);

  return converted_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_right(void) {
  uint16_t sensor_value = 0;
  uint16_t converted_sensor_value = 0;

  sensor_value = adc__get_adc_value(ADC__CHANNEL_4);
  converted_sensor_value = ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(sensor_value);

  return converted_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_front(void) {
  uint16_t sensor_value = 0;
  uint16_t converted_sensor_value = 0;

  sensor_value = adc__get_adc_value(ADC__CHANNEL_5);
  converted_sensor_value = ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(sensor_value);

  return converted_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_back(void) {
  uint16_t sensor_value = 0;
  uint16_t converted_sensor_value = 0;

  // TODO: add support to ADC driver for channel 3
  // sensor_value = adc__get_adc_value(ADC__CHANNEL_3);
  // converted_sensor_value = ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(sensor_value);

  return converted_sensor_value;
}
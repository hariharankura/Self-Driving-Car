#include "ultrasonic_sensor_handler.h"
#include "adc.h"
#include "gpio.h"

void ultrasonic_sensor_handler__initialize_sensors(void) {
  gpio__construct_with_function(GPIO__PORT_0, 25, GPIO__FUNCTION_1); // left: channel 2
  gpio__construct_with_function(GPIO__PORT_1, 30, GPIO__FUNCTION_1); // right: channel 4
  gpio__construct_with_function(GPIO__PORT_1, 31, GPIO__FUNCTION_1); // front: channel 5
  gpio__construct_with_function(GPIO__PORT_0, 26, GPIO__FUNCTION_1); // back: channel 3
  adc__initialize();
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_left(void) {
  uint16_t sensor_value = 0;

  sensor_value = 400; // dummy value
  sensor_value = adc__get_adc_value(ADC__CHANNEL_2);

  return sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_right(void) {
  uint16_t sensor_value = 0;

  sensor_value = 500; // dummy value
  sensor_value = adc__get_adc_value(ADC__CHANNEL_4);

  return sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_front(void) {
  uint16_t sensor_value = 0;

  sensor_value = 600; // dummy value
  sensor_value = adc__get_adc_value(ADC__CHANNEL_5);

  return sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_back(void) {
  uint16_t sensor_value = 0;

  sensor_value = 700; // dummy value
  // TODO: add support to ADC driver for channel 3
  // sensor_value = adc__get_adc_value(ADC__CHANNEL_3);

  return sensor_value;
}
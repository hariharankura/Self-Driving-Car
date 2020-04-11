#include "ultrasonic_sensor_handler.h"
#include "adc.h"
#include "delay.h"
#include "gpio.h"
#include "mode_filter.h"
#include <stdio.h>
#include <string.h>

// Private Variable
static int driver_threshold_in_cm = 50;
static int consec_values_threshold = 2;

static int consec_values_left = 0;
static int consec_values_right = 0;
static int consec_values_front = 0;
static int consec_values_back = 0;

// Private Functions
static uint16_t ultrasonic_sensor_handler__3_3V_convert_12_bit_adc_value_to_cm(uint16_t adc_value) {
  float distance_in_cm = 0;
  float slope = 0.1264;
  float y_intercept = -21.86;

  distance_in_cm = (slope * adc_value) + y_intercept;

  return distance_in_cm;
}

static uint16_t ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(uint16_t adc_value) {
  float distance_in_cm = 0;
  float slope = 0.077656;
  float y_intercept = -9.706729;

  distance_in_cm = (slope * adc_value) + y_intercept;

  return distance_in_cm;
}

static uint16_t ultrasonic_sensor_handler__get_filtered_value_in_cm_from_n_sensor_values(int n,
                                                                                         int ADC_channel_number) {
  adc_channel_e ADC_channel;

  switch (ADC_channel_number) {
  case 2:
    ADC_channel = 2;
    break;
  case 3:
    ADC_channel = 3;
    break;
  case 4:
    ADC_channel = 4;
    break;
  case 5:
    ADC_channel = 5;
    break;

  default:
    ADC_channel = 2;
    printf("Input ADC channel is not supported! Using default (channel 2)\n");
    break;
  }

  uint16_t raw_sensor_value = 0;
  uint16_t filtered_sensor_value = 0;
  uint16_t sensor_values_in_cm[n];

  memset(sensor_values_in_cm, 0, sizeof(sensor_values_in_cm));

  for (int i = 0; i < n; i++) {
    raw_sensor_value = adc__get_adc_value(ADC_channel);
    sensor_values_in_cm[i] = ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(raw_sensor_value);
  }

  filtered_sensor_value = mode_filter__find_mode_of_sensor_values_converted_to_cm(sensor_values_in_cm, n);

  return filtered_sensor_value;
}

static uint16_t ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(uint16_t sensor_value,
                                                                                      int threshold,
                                                                                      int numb_of_consec_values) {
  uint16_t corrected_sensor_value = 0;

  if (sensor_value <= threshold) {
    consec_values_left++;
    if (consec_values_left >= numb_of_consec_values) {
      consec_values_left = 0;
      corrected_sensor_value = sensor_value;
    } else {
      corrected_sensor_value = threshold + 1;
    }
  } else {
    corrected_sensor_value = sensor_value;
  }

  return corrected_sensor_value;
}

static uint16_t ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(uint16_t sensor_value,
                                                                                       int threshold,
                                                                                       int numb_of_consec_values) {
  uint16_t corrected_sensor_value = 0;

  if (sensor_value <= threshold) {
    consec_values_right++;
    if (consec_values_right >= numb_of_consec_values) {
      consec_values_right = 0;
      corrected_sensor_value = sensor_value;
    } else {
      corrected_sensor_value = threshold + 1;
    }
  } else {
    corrected_sensor_value = sensor_value;
  }

  return corrected_sensor_value;
}

static uint16_t ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(uint16_t sensor_value,
                                                                                       int threshold,
                                                                                       int numb_of_consec_values) {
  uint16_t corrected_sensor_value = 0;

  if (sensor_value <= threshold) {
    consec_values_front++;
    if (consec_values_front >= numb_of_consec_values) {
      consec_values_front = 0;
      corrected_sensor_value = sensor_value;
    } else {
      corrected_sensor_value = threshold + 1;
    }
  } else {
    corrected_sensor_value = sensor_value;
  }

  return corrected_sensor_value;
}

static uint16_t ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(uint16_t sensor_value,
                                                                                      int threshold,
                                                                                      int numb_of_consec_values) {
  uint16_t corrected_sensor_value = 0;

  if (sensor_value <= threshold) {
    consec_values_back++;
    if (consec_values_back >= numb_of_consec_values) {
      consec_values_back = 0;
      corrected_sensor_value = sensor_value;
    } else {
      corrected_sensor_value = threshold + 1;
    }
  } else {
    corrected_sensor_value = sensor_value;
  }

  return corrected_sensor_value;
}

// Public Functions
void ultrasonic_sensor_handler__initialize_sensors(void) {
  gpio__construct_with_function(GPIO__PORT_0, 25, GPIO__FUNCTION_1); // left: channel 2
  gpio__construct_with_function(GPIO__PORT_1, 30, GPIO__FUNCTION_1); // right: channel 4
  gpio__construct_with_function(GPIO__PORT_1, 31, GPIO__FUNCTION_1); // front: channel 5
  gpio__construct_with_function(GPIO__PORT_0, 26, GPIO__FUNCTION_1); // back: channel 3

  adc__initialize();
}

// get unfiltered sensor values
uint16_t ultrasonic_sensor_handler__get_sensor_value_left(void) {
  uint16_t sensor_value = 0;
  uint16_t converted_sensor_value = 0;

  sensor_value = adc__get_adc_value(ADC__CHANNEL_2);
  converted_sensor_value = ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(sensor_value);

  return converted_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_right(void) {
  uint16_t sensor_value = 0;
  uint16_t converted_sensor_value = 0;

  sensor_value = adc__get_adc_value(ADC__CHANNEL_4);
  converted_sensor_value = ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(sensor_value);

  return converted_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_front(void) {
  uint16_t sensor_value = 0;
  uint16_t converted_sensor_value = 0;

  sensor_value = adc__get_adc_value(ADC__CHANNEL_5);
  converted_sensor_value = ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(sensor_value);

  return converted_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_sensor_value_back(void) {
  uint16_t sensor_value = 0;
  uint16_t converted_sensor_value = 0;

  sensor_value = adc__get_adc_value(ADC__CHANNEL_3);
  converted_sensor_value = ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(sensor_value);

  return converted_sensor_value;
}

// get filtered sensor values
uint16_t ultrasonic_sensor_handler__get_filtered_sensor_value_left(void) {
  uint16_t filtered_sensor_value = 0;

  filtered_sensor_value = ultrasonic_sensor_handler__get_filtered_value_in_cm_from_n_sensor_values(10, 2);
  filtered_sensor_value = ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(
      filtered_sensor_value, driver_threshold_in_cm, consec_values_threshold);

  return filtered_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_filtered_sensor_value_right(void) {
  uint16_t filtered_sensor_value = 0;

  filtered_sensor_value = ultrasonic_sensor_handler__get_filtered_value_in_cm_from_n_sensor_values(10, 4);
  filtered_sensor_value = ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
      filtered_sensor_value, driver_threshold_in_cm, consec_values_threshold);

  return filtered_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_filtered_sensor_value_front(void) {
  uint16_t filtered_sensor_value = 0;

  filtered_sensor_value = ultrasonic_sensor_handler__get_filtered_value_in_cm_from_n_sensor_values(10, 5);
  filtered_sensor_value = ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
      filtered_sensor_value, driver_threshold_in_cm, consec_values_threshold);

  return filtered_sensor_value;
}

uint16_t ultrasonic_sensor_handler__get_filtered_sensor_value_back(void) {
  uint16_t filtered_sensor_value = 0;

  filtered_sensor_value = ultrasonic_sensor_handler__get_filtered_value_in_cm_from_n_sensor_values(10, 3);
  filtered_sensor_value = ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(
      filtered_sensor_value, driver_threshold_in_cm, consec_values_threshold);

  return filtered_sensor_value;
}
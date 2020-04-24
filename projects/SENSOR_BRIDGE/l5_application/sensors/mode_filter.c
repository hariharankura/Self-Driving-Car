#include "mode_filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t mode_filter__find_mode_of_sensor_values_converted_to_cm(uint16_t *sensor_values, int numb_of_sensor_values) {
  int range_of_sensor_values = 600; // max value is 500cm. +100cm extra to be safe
  int highest_frequency = 0;
  uint16_t most_frequent_sensor_value = 0;

  int sensor_value_frequencies[range_of_sensor_values];
  memset(sensor_value_frequencies, 0, sizeof(sensor_value_frequencies));

  for (int i = 0; i < numb_of_sensor_values; i++) {
    sensor_value_frequencies[*sensor_values]++;

    if (sensor_value_frequencies[*sensor_values] >= highest_frequency) {
      highest_frequency = sensor_value_frequencies[*sensor_values];
      most_frequent_sensor_value = *sensor_values;
    }

    sensor_values++;
  }

  // TODO: handle condition if all sensor values occur only once differently? right now, the most recent value is
  // returned (value with highest sensor_value array index)
  if (highest_frequency == 1) {
    printf("Unreliable filtered value: No repeat sensor values!\n");
    // light up an LED? return 1000?
  }

  // TODO: handle condition where multiple sensor values have the same number of occurences (multiple modes)
  // differently? average the multiple modes? right now, the most recent mode is returned (the one that has the highest
  // sensor_value array index)

  return most_frequent_sensor_value;
}
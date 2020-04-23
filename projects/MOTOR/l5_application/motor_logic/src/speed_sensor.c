#include "speed_sensor.h"
#include "board_io.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"

#include "stdio.h"

static uint8_t motor_test_button_status = 0;

const float pi = 3.1416;
const float wheel_diameter_in_cm = 10;
const uint32_t miles_to_cm = 160934;

uint8_t windowtime_in_seconds = 1;
uint32_t rotations_in_windowtime = 0, recorded_rotations_in_windowtime;
uint32_t rpm;
float mph;

void handle_interrupts(void) {
  if (LPC_GPIOINT->IO0IntStatR & (1 << 29)) {
    motor_test_button_status = 1;
    LPC_GPIOINT->IO0IntClr = (1 << 29);
  }
  if (LPC_GPIOINT->IO0IntStatR & (1 << 6)) {
    rotations_in_windowtime++;
    LPC_GPIOINT->IO0IntClr = (1 << 6);
  }
}

void initialize_test_button_and_speed_sensor_interrupts(void) {
  gpio__construct_as_input(0, 29);
  gpio__construct_as_input(0, 6);
  gpio_enable_interrupt(29);
  gpio_enable_interrupt(6);
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, handle_interrupts, NULL);
}

float get_mph(void) { return mph; }

uint32_t get_rpm(void) { return rpm; }

void calculate_rpm_and_mph(uint8_t rotations) {
  rpm = (rotations * (60 / windowtime_in_seconds));
  mph = ((pi * wheel_diameter_in_cm) * (rpm * 60)) / miles_to_cm;
}

void clear_rotations_in_windowtime(void) {
  recorded_rotations_in_windowtime = rotations_in_windowtime;
  calculate_rpm_and_mph(recorded_rotations_in_windowtime);
  print_recorded_data();
  rotations_in_windowtime = 0;
}

uint8_t get_windowtime(void) { return windowtime_in_seconds; }

void print_recorded_data(void) {
  // printf("rotations = %ld\n rpm: %ld \n mph: %f\n\n", recorded_rotations_in_windowtime, get_rpm(), get_mph());
}

uint8_t get_motor_test_button_status(void) { return motor_test_button_status; }

void reset_motor_test_button_status(void) { motor_test_button_status = 0; }
#include "speed_sensor.h"

#include <stdio.h>

#include "board_io.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "motor_self_test.h"

static const float pi = 3.1416;
static const float wheel_diameter_in_cm = 10;
static const uint32_t miles_to_cm = 160934;
static const float gear_ratio = 2.72;

static uint16_t windowtime_in_ms = 500;
static uint32_t rotations_in_windowtime = 0, recorded_rotations_in_windowtime;
static uint32_t rpm;
static float mph;

void handle_interrupt(void) {
  if (LPC_GPIOINT->IO0IntStatR & (1 << 6)) {
    rotations_in_windowtime++;
    LPC_GPIOINT->IO0IntClr = (1 << 6);
  }
  if (LPC_GPIOINT->IO0IntStatR & (1 << 29)) {
    set_motor_test_button_status(1);
    LPC_GPIOINT->IO0IntClr = (1 << 6);
  }
}

void initialize_speed_sensor_and_test_button_interrupts(void) {
  gpio__construct_as_input(0, 6);
  gpio_enable_interrupt(6);
  gpio__construct_as_input(0, 29);
  gpio_enable_interrupt(29);
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, handle_interrupt, NULL);
}

float get_mph(void) { return mph; }

uint32_t get_rpm(void) { return rpm; }

void calculate_rpm_and_mph(uint8_t rotations) {
  rpm = (rotations * ((60 * 1000) / windowtime_in_ms));
  mph = ((pi * wheel_diameter_in_cm) * (rpm * 60)) / (miles_to_cm * gear_ratio);
}

void clear_rotations_in_windowtime(void) {
  recorded_rotations_in_windowtime = rotations_in_windowtime;
  calculate_rpm_and_mph(recorded_rotations_in_windowtime);
  rotations_in_windowtime = 0;
}

uint16_t get_windowtime(void) { return windowtime_in_ms; }
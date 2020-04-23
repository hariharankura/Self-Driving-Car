#include "unity.h"

#include "Mockgpio.h"
#include "Mocksjvalley_lcd.h"
#include "driver_diagnostics.h"
#include "driver_logic.c"
#include "driving_algo.h"
#include "obstacle_avoidance.h"

void setUp() {}
void tearDown() {}

void test__driver_logic__set_car_mode(void) {
  dbc_CAR_ACTION_s car_action = {};
  car_action.CAR_ACTION_cmd = 1;
  driver_logic__set_car_mode(car_action);
}

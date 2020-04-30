#include "driving_algo.h"
#include "driver_diagnostics.h"
#include "gpio.h"

static int8_t TOLERANCE_DIRECTION_POSITIVE = 10;
static int8_t TOLERANCE_DIRECTION_NEGATIVE = -10;
static const uint8_t MINIMUM_DISTACE_RANGE = 5;

static const gpio_s DRIVE_MODE_STATUS_LED = {GPIO__PORT_1, 24};

static dbc_GEO_COMPASS_s current_and_destination_heading_angle;

void driving_algo__process_geo_compass_data(const dbc_GEO_COMPASS_s compass_angle) {
  current_and_destination_heading_angle = compass_angle;
}

static float driving_algo__compute_deflection(const dbc_GEO_COMPASS_s *heading_angle) {
  float deflection = heading_angle->GEO_COMPASS_current_heading - heading_angle->GEO_COMPASS_desitination_heading;
  if (deflection > 180) {
    deflection -= 360;
  } else if (deflection < -180) {
    deflection += 360;
  }
  return deflection;
}

static void driving_algo__get_gps_heading_direction(dbc_DRIVER_STEER_SPEED_s *driving_direction) {
  float deflection = driving_algo__compute_deflection(&current_and_destination_heading_angle);
  driving_direction->DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  if (deflection > TOLERANCE_DIRECTION_POSITIVE) {
    driving_direction->DRIVER_STEER_direction =
        (deflection < 90) ? DRIVER_STEER_direction_SOFT_RIGHT : DRIVER_STEER_direction_HARD_RIGHT;
  } else if (deflection < TOLERANCE_DIRECTION_NEGATIVE) {
    driving_direction->DRIVER_STEER_direction =
        (deflection > -90) ? DRIVER_STEER_direction_SOFT_LEFT : DRIVER_STEER_direction_HARD_LEFT;
  } else {
    driving_direction->DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  }
}

static bool driving_algo__is_destination_reached(void) {
  bool return_val = false;
  if (current_and_destination_heading_angle.GEO_COMPASS_distance < MINIMUM_DISTACE_RANGE) {
    return_val = true;
  }
  return return_val;
}

dbc_DRIVER_STEER_SPEED_s driving_algo__compute_heading() {
  dbc_DRIVER_STEER_SPEED_s driving_direction;
  if (driving_algo__is_destination_reached()) {
    driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
    driving_direction.DRIVER_STEER_move_speed = DRIVER_STEER_move_STOP;
  } else {
    if (obstacle_avoidance__is_required()) {
      gpio__reset(DRIVE_MODE_STATUS_LED);
      obstacle_avoidance__get_direction(&driving_direction); // follow ultrasonic direction
    } else {
      driving_algo__get_gps_heading_direction(&driving_direction);
      gpio__set(DRIVE_MODE_STATUS_LED);
    }
    light_up_direction_led(driving_direction);
  }
  return driving_direction;
}
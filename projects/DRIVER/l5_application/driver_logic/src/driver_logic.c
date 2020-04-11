#include "driver_logic.h"
#include "driving_algo.h"
#include <stdio.h>
#include <string.h>

dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void) { return driving_algo__compute_heading(); }

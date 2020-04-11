#pragma once
#include "project.h"

dbc_DRIVER_STEER_SPEED_s driving_algo__compute_heading();
void driving_algo__process_geo_compass_data(const dbc_GEO_COMPASS_s compass_angle);
#pragma once
#include "project.h"

#define PI 3.14159f

void compass__set_destination_gps(dbc_BRIDGE_GPS_s *copy_dest_data);
dbc_GEO_COMPASS_s compass__get_current_and_destination_heading(void);
<<<<<<< HEAD
void compass__read_current_gps_coordinate();
=======
>>>>>>> GEO BOARD: COMPASS ANGLE LOGIC

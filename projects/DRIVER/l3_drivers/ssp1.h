#pragma once

#include "stdint.h"

void ssp1__init(uint32_t clock_frequency_in_mhz);
uint8_t ssp1__transfer_data_1_byte(uint8_t data);
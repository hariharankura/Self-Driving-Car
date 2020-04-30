#pragma once
#include <stdbool.h>

void full_can_bus_handler__init(void);

void full_can_bus_handler__receive_messages(void);

void full_can_bus_handler__reset_if_bus_off(void);

void full_can_bus_handler__transmit_message_in_20hz(void);

void full_can_bus_handler__manage_mia_50hz(void);
bool dbc_send_can_message(void *argument, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc);

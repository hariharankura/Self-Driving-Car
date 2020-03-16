#pragma once
#include <stdbool.h>
#include <stdint.h>

void can_bus_handler__init(void);
void can_bus_handler__process_all_received_messages(void);
void can_bus_handler__transmit_message(void);
bool dbc_send_can_message(void *argument, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc);
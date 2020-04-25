#pragma once

void init_can_driver(void);
void can_handler__reset_if_bus_off(void);
void can_bus_handler__process_all_received_messages_in_20hz(void);
void can_bus_handler__manage_mia_20hz(void);
void can_bus_handler__transmit_message_in_10hz(void);
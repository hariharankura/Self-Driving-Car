#pragma once

void can_bus_handler__init(void);
void can_bus_handler__process_all_received_messages(void);
void can_bus_handler__transmit_message(void);
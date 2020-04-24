#pragma once

void bridge_can_handler__transmit_messages_10hz(void);
void bridge_can_handler__transmit_start_stop_condition(void);

// for testing purposes
void bridge_can_handler__handle_all_incoming_messages(void);
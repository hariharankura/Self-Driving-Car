#pragma once
#include <stdbool.h>
#include <stdint.h>

void sjvalley_lcd__communication_init(void);

void sjvalley_lcd__init(void);

bool sjvalley_lcd__send_line(uint8_t line_number, char *line);
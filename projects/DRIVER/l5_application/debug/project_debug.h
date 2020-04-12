#pragma once

#define PROJECT__ENABLE_DEBUGGING 1

// Printf on serial console
#if PROJECT__ENABLE_DEBUGGING
#include <stdio.h>
#define PROJECT_DEBUG__PRINTF(f_, ...)                                                                                 \
  do {                                                                                                                 \
    fprintf(stderr, "PROJECT:");                                                                                       \
    fprintf(stderr, (f_), ##__VA_ARGS__);                                                                              \
    fprintf(stderr, "\n");                                                                                             \
  } while (0)
#else
#define PROJECT_DEBUG__PRINTF(f_, ...) /* NOOP */
#endif

#if PROJECT__ENABLE_DEBUGGING
#include "sjvalley_lcd.h"
#include "sl_string.h"
static char lcd_buffer[20];
#define PROJECT_DEBUG__LCD_PRINTF(l_, f_, ...)                                                                         \
  do {                                                                                                                 \
    snprintf(lcd_buffer, 20, (f_), ##__VA_ARGS__);                                                                     \
    sjvalley_lcd__send_line(l_, lcd_buffer);                                                                           \
  } while (0)
#else
#define PROJECT_DEBUG__LCD_PRINTF(l_, f_, ...) /* NOOP */
#endif

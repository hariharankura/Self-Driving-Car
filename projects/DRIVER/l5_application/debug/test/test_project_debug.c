#include "unity.h"

#include "project_debug.h"

#include "Mocksjvalley_lcd.h"

bool sjvalley_lcd__send_line_hijacked_stub(uint8_t line_number, char *line, int callback_count) {
  PROJECT_DEBUG__PRINTF("Line = %d, MSG = %s\n", line_number, line);
  return true;
}

void test_project_debug__print_console(void) { PROJECT_DEBUG__PRINTF("This is test_printf\n"); }

void test_project_debug__print_lcd(void) {
  sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
  PROJECT_DEBUG__LCD_PRINTF(1, "LCD Debug Printf");
  PROJECT_DEBUG__LCD_PRINTF(2, "Hello");
}
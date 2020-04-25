#include "sjvalley_lcd.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "uart_printf.h"
#include <stdio.h>
#include <string.h>

static const uart_e SJVALLEY_LCD_UART = UART__3;
static const uint8_t BRIGHTNESS = 100;

static gpio_s sjvalley_lcd_reset_pin;

void sjvalley_lcd__uart_pin_function_set(void) {
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2);
  sjvalley_lcd_reset_pin = gpio__construct_with_function(GPIO__PORT_1, 23, GPIO__FUNCITON_0_IO_PIN);
}

void sjvalley_lcd__uart_init(void) {
  uart__init(SJVALLEY_LCD_UART, clock__get_peripheral_clock_hz(), 38400);
  QueueHandle_t rxq_handle = xQueueCreate(32, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(32, sizeof(char)); // We don't send anything to the GPS
  uart__enable_queues(SJVALLEY_LCD_UART, rxq_handle, txq_handle);
}

void sjvalley_lcd__clear_screen(void) {
  char *msg = "$CLR_SCR\n";
  uart_puts(SJVALLEY_LCD_UART, msg);
  char ch;
  while (uart__get(SJVALLEY_LCD_UART, &ch, 0)) {
    printf("Data = %c\n", ch);
  }
}

void sjvalley_lcd__reset(void) {
  gpio__reset(sjvalley_lcd_reset_pin);
  delay__ms(1);
  gpio__set(sjvalley_lcd_reset_pin);
}

void sjvalley_lcd__communication_init(void) {
  delay__ms(100);
  uart__put(SJVALLEY_LCD_UART, 0xF0, 0);
  delay__ms(100);
  uart_printf(SJVALLEY_LCD_UART, "$BLIGHT:%d\n", BRIGHTNESS);
  sjvalley_lcd__clear_screen();
}

void sjvalley_lcd__init(void) {
  sjvalley_lcd__uart_pin_function_set();
  sjvalley_lcd__uart_init();
}

bool sjvalley_lcd__send_line(uint8_t line_number, char *line) {
  bool return_flag = false;
  int line_len = strlen(line);
  if ((line_number < 4) && (line_len > 0) && (line_len < 20)) {
    uart_printf(SJVALLEY_LCD_UART, "$L:%d:%s\n", line_number, line);
  }
  return return_flag;
}

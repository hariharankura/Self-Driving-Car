#include "oled.h"
#include "gpio.h"
#include "ssp1.h"
#include <stdio.h>
#include <string.h>

static gpio_s cs;
static gpio_s dc;

static const size_t kColumns = 128;
/// Defines the width in pixels, which is simply the columns
static const size_t kWidth = kColumns;
/// Defines the height in pixels
static const size_t kHeight = 64;
/// Defines the column height which is a single byte or 8 bits
static const size_t kColumnHeight = 8;
/// Calculates the number of rows that exist for the device.
static const size_t kRows = kHeight / kColumnHeight;
static uint8_t bitmap_[8 + 5][128 + 5];

static const bool kCommand = false;
static const bool kData = true;

typedef struct Color_t {
  /// Bits of the red channel
  uint8_t red;
  /// Bits of the green channel
  uint8_t green;
  /// Bits of the blue channel
  uint8_t blue;
  /// Bits of alpha (transparent) channel
  uint8_t alpha;
} Color_t;

void draw_actual_pixel(int32_t x, int32_t y, Color_t color) {
  uint32_t bit_position = y & 0b111;

  // Each byte makes up a vertical column.
  // Shifting by 3, which also divides by 8 (the 8-bits of a column), will
  // be the row that we need to edit.
  uint32_t row = y >> 3;

  // Mask to clear the bit
  uint32_t clear_mask = ~(1 << bit_position);

  // Mask to set the bit, if color.alpha != 0
  bool pixel_is_on = true;
  if ((color.alpha == 0) && (color.red == 0) && (color.green == 0) && (color.blue == 0)) {
    pixel_is_on = false;
  }
  uint32_t set_mask = pixel_is_on << bit_position;

  // Address of the pixel column to edit
  uint8_t *pixel_column = &(bitmap_[row][x]);

  // Read pixel column and update the pixel
  uint32_t result = (*pixel_column & clear_mask) | set_mask;
  printf("result = %d\n", result);
  // Update pixel with the result of this operation
  *pixel_column = (uint8_t)result;
}

void DrawPixel(uint32_t x, uint32_t y) {
  Color_t color_ = {255};
  // Pixels outside of the bounds of the screen will not be drawn.
  if (x <= kWidth && y <= kHeight) {
    draw_actual_pixel(x, y, color_);
  }
}

void oled__clear() { memset(bitmap_, 0x00, sizeof(bitmap_)); }

void oled__write(uint32_t data, bool transaction, size_t size) {
  if (transaction)
    gpio__set(dc);
  else
    gpio__reset(dc);
  gpio__reset(cs);
  printf("data = %d\n", data);
  for (size_t i = 0; i < size; i++) {
    uint8_t send = (uint8_t)(data >> (((size - 1) - i) * 8));
    printf("Data send = %d\n", send);
    ssp1__transfer_data_1_byte(send);
  }
  gpio__set(cs);
}

void oled_set_horizontal_address_mode() {
  // Set Addressing mode
  // Addressing mode = Horizontal Mode (0b00)
  oled__write(0x2000, kCommand, 2);

  // Set Column Addresses
  // Set Column Address start = Column 0
  // Set Column Address start = Column 127
  oled__write(0x21007F, kCommand, 3);

  // Set Page Addresses
  // Set Page Address start = Page 0
  // Set Page Address start = Page 7
  oled__write(0x220007, kCommand, 3);
}

void oled__initialization_panel() {
  oled__write(0xAE, kCommand, 1);
  oled__write(0xD580, kCommand, 2);
  oled__write(0xA83F, kCommand, 2);
  oled__write(0xD300, kCommand, 2);
  oled__write(0x40, kCommand, 1);
  oled__write(0x8D14, kCommand, 2);
  oled__write(0xA1, kCommand, 1);
  oled__write(0xC8, kCommand, 1);
  oled__write(0xDA12, kCommand, 2);
  oled__write(0x81CF, kCommand, 2);
  oled__write(0xD9F1, kCommand, 2);
  oled__write(0xDB40, kCommand, 2);
  oled_set_horizontal_address_mode();
  oled__write(0xA4, kCommand, 1);
  oled__write(0xA6, kCommand, 1);
  oled__write(0xAF, kCommand, 1);
}

void oled__init() {
  cs = gpio__construct_with_function(1, 22, GPIO__FUNCITON_0_IO_PIN);
  dc = gpio__construct_with_function(1, 25, GPIO__FUNCITON_0_IO_PIN);
  gpio__set(cs);
  gpio__set(dc);
  gpio__set_as_output(cs);
  gpio__set_as_output(dc);
  ssp1__init(2);
  oled__clear();
  oled__initialization_panel();
}

void oled__update() {
  oled_set_horizontal_address_mode();
  for (size_t row = 0; row < kRows; row++) {
    for (size_t column = 0; column < kColumns; column++) {
      oled__write(32, kData, 1);
    }
  }
}
#include "line_buffer.h"

#include <string.h>

void line_buffer__init(line_buffer_s *buffer, void *memory, size_t size) {
  buffer->memory = (char *)memory;
  memset(memory, 0, size);
  buffer->max_size = size;
  buffer->write_index = 0;
  buffer->read_index = 0;
  buffer->byte_count = 0;
}

bool line_buffer__is_full(line_buffer_s *buffer) { return buffer->byte_count == buffer->max_size; }

bool line_buffer__contains_data(line_buffer_s *buffer) { return buffer->byte_count > 0; }

size_t line_buffer__get_write_index_and_increment(line_buffer_s *buffer) {
  int buffer_write_index = buffer->write_index;
  buffer_write_index = (buffer_write_index % buffer->max_size) + 1;
  buffer->write_index = buffer_write_index;
  return buffer_write_index - 1;
}

size_t line_buffer__get_read_index_and_increment(line_buffer_s *buffer) {
  int buffer_read_index = buffer->read_index;
  buffer_read_index = (buffer_read_index % buffer->max_size) + 1;
  buffer->read_index = buffer_read_index;
  return buffer_read_index - 1;
}

// Adds a byte to the buffer, and returns true if the buffer had enough space to add the byte
bool line_buffer__add_byte(line_buffer_s *buffer, char byte) {
  if (!line_buffer__is_full(buffer)) {
    int index = line_buffer__get_write_index_and_increment(buffer);
    ((char *)buffer->memory)[index] = byte;
    buffer->byte_count++;
    return true;
  }
  return false;
}

/**
 * If the line buffer has a complete line, it will remove that contents and save it to "char * line"
 * @param line_max_size This is the max size of 'char * line' memory pointer
 */

bool line_buffer__process_data_return_true_if_newline(line_buffer_s *buffer, char *out_buffer) {
  // bool return_val = false;
  int index = line_buffer__get_read_index_and_increment(buffer);
  char data = ((char *)buffer->memory)[index];
  if (buffer->byte_count == 1) {
    if (data != '\n') {
      *out_buffer = '\0';
      buffer->read_index--;
      return true;
    }
  }
  buffer->byte_count--;
  if (data == '\n') {
    data = '\0';
    *out_buffer = data;
    ((char *)buffer->memory)[index] = 0;
    return true;
  }
  if (data == '\r') {
    data = '\0';
    *out_buffer = data;
    ((char *)buffer->memory)[index] = 0;
  }
  *out_buffer = data;
  ((char *)buffer->memory)[index] = 0;
  return false;
}

bool line_buffer__has_line(const line_buffer_s *line_buffer) {
  bool has_line = false;
  size_t read_index = line_buffer->read_index;
  for (size_t chars = 0; chars < line_buffer->byte_count; chars++) {
    if (((char *)line_buffer->memory)[read_index] == '\n') {
      has_line = true;
      break;
    }
    read_index = (1 + read_index) % line_buffer->max_size;
  }
  return has_line;
}

bool line_buffer__remove_line(line_buffer_s *buffer, char *line, size_t line_max_size) {
  bool return_value = false;
  if (line_buffer__has_line(buffer) || buffer->max_size == buffer->byte_count) {
    for (int i = 0; i < line_max_size; i++) {
      if (line_buffer__contains_data(buffer)) {
        if (line_buffer__process_data_return_true_if_newline(buffer, &line[i])) {
          return_value = true;
          break;
        }
      } else {
        return_value = false;
        break;
      }
    }
  }
  return return_value;
}
#include "unity.h"

#include "line_buffer.c"

static line_buffer_s line_buffer;
static char memory[8];

static line_buffer_s line_buffer_1;
static char memory_1[200];

void setUp(void) { line_buffer__init(&line_buffer, memory, sizeof(memory)); }

void tearDown(void) {}

void test_line_buffer__is_full(void) {
  for (size_t i = 0; i < sizeof(memory); i++) {
    TEST_ASSERT_FALSE(line_buffer__is_full(&line_buffer));
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
  }
  TEST_ASSERT_TRUE(line_buffer__is_full(&line_buffer));
}

void test_line_buffer__contains_data_with_new_line(void) {
  TEST_ASSERT_FALSE(line_buffer__contains_data(&line_buffer));
  for (size_t i = 0; i < sizeof(memory) - 1; i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
    TEST_ASSERT_TRUE(line_buffer__contains_data(&line_buffer));
  }
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  char line_data[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line_data, sizeof(line_data)));
  TEST_ASSERT_FALSE(line_buffer__contains_data(&line_buffer));
}

void test_line_buffer__contains_data_without_new_line(void) {
  TEST_ASSERT_FALSE(line_buffer__contains_data(&line_buffer));
  for (size_t i = 0; i < sizeof(memory); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
    TEST_ASSERT_TRUE(line_buffer__contains_data(&line_buffer));
  }
  char line_data[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line_data, sizeof(line_data)));
  TEST_ASSERT_TRUE(line_buffer__contains_data(&line_buffer));
  TEST_ASSERT_FALSE(line_buffer__remove_line(&line_buffer, line_data, sizeof(line_data)));
}

void test_line_buffer__has_line(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, 'c');
  TEST_ASSERT_FALSE(line_buffer__has_line(&line_buffer));
  line_buffer__add_byte(&line_buffer, '\n');
  TEST_ASSERT_TRUE(line_buffer__has_line(&line_buffer));
  line_buffer__add_byte(&line_buffer, 'd');
  line_buffer__add_byte(&line_buffer, 'e');
  line_buffer__add_byte(&line_buffer, 'f');
  line_buffer__add_byte(&line_buffer, '\n');
  TEST_ASSERT_TRUE(line_buffer__has_line(&line_buffer));
  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abc");
  TEST_ASSERT_TRUE(line_buffer__has_line(&line_buffer));
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "def");
}

void test_line_buffer__write_index_increment(void) {
  for (size_t j = 0; j < 2; j++) {
    for (size_t i = 0; i < sizeof(memory); i++) {
      size_t index = line_buffer__get_write_index_and_increment(&line_buffer);
      TEST_ASSERT_EQUAL_size_t(i, index);
    }
  }
}

void test_line_buffer__read_processes_data(void) {
  char ch;
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  TEST_ASSERT_TRUE(line_buffer__process_data_return_true_if_newline(&line_buffer, &ch));
  TEST_ASSERT_EQUAL_CHAR('\0', ch);

  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\r'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  TEST_ASSERT_FALSE(line_buffer__process_data_return_true_if_newline(&line_buffer, &ch));
  TEST_ASSERT_EQUAL_CHAR('\0', ch);
  TEST_ASSERT_TRUE(line_buffer__process_data_return_true_if_newline(&line_buffer, &ch));
  TEST_ASSERT_EQUAL_CHAR('\0', ch);

  for (size_t i = 0; i < sizeof(memory); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
  }

  for (size_t i = 0; i < sizeof(memory) - 1; i++) {
    TEST_ASSERT_FALSE(line_buffer__process_data_return_true_if_newline(&line_buffer, &ch));
    TEST_ASSERT_EQUAL_CHAR(ch, 'a' + i);
  }
  TEST_ASSERT_TRUE(line_buffer__process_data_return_true_if_newline(&line_buffer, &ch));
  TEST_ASSERT_EQUAL_CHAR(ch, '\0');
}

void test_line_buffer__read_index_increment(void) {
  for (size_t j = 0; j < 2; j++) {
    for (size_t i = 0; i < sizeof(memory); i++) {
      int index = line_buffer__get_read_index_and_increment(&line_buffer);
      TEST_ASSERT_EQUAL_size_t(i, index);
    }
  }
}

void test_line_buffer__nominal_case(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, 'c');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abc");
}

void test_line_buffer__slash_r_slash_n_case(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, '\r');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  // TEST_ASSERT_EQUAL_STRING(line, "ab\r");
  TEST_ASSERT_EQUAL_STRING(line, "ab");
}

// Line buffer should be able to add multiple lines and we should be able to remove them one at a time
void test_line_buffer__multiple_lines(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, '\r');
  line_buffer__add_byte(&line_buffer, '\n');
  line_buffer__add_byte(&line_buffer, 'c');
  line_buffer__add_byte(&line_buffer, 'd');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "ab");

  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "cd");
}

void test_line_buffer__overflow_case(void) {
  // Add chars until full capacity
  for (size_t i = 0; i < sizeof(memory); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
  }

  // Buffer should be full now
  TEST_ASSERT_FALSE(line_buffer__add_byte(&line_buffer, 'b'));

  // Retreive truncated output (without the newline char)
  // Do not modify this test; instead, change your API to make this test pass
  // Note that line buffer was full with "abcdefgh" but we should only
  // retreive "abcdefg" because we need to write NULL char to line[8]
  char line[8] = {0};
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abcdefg");
}

void test_line_buffer__overflow_case_check_new_data(void) {
  // Add chars until full capacity
  for (size_t i = 0; i < sizeof(memory); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
  }

  // Buffer should be full now
  TEST_ASSERT_FALSE(line_buffer__add_byte(&line_buffer, 'b'));

  // Retreive truncated output (without the newline char)
  // Do not modify this test; instead, change your API to make this test pass
  // Note that line buffer was full with "abcdefgh" but we should only
  // retreive "abcdefg" because we need to write NULL char to line[8]
  char line[8] = {0};
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abcdefg");
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'i'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "hi");
}

void test_line_buffer__remove_incomplete_line(void) {
  for (size_t i = 0; i < sizeof(memory) - 2; i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a' + i));
  }
  char line[8] = {0};
  TEST_ASSERT_FALSE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'i'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abcdefi");
}

void test_line_buffer__add_remove_line(void) {
  static char test_string[] = "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n$GPGGA,123519,4807.038,N,01131.000,E,"
                              "1,08,0.9,545.4,M,46.9,M,,*47\n$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n";
  line_buffer__init(&line_buffer_1, memory_1, sizeof(memory_1));
  for (size_t i = 0; i < sizeof(test_string); i++) {
    line_buffer__add_byte(&line_buffer_1, test_string[i]);
  }
  char data[100];
  line_buffer__remove_line(&line_buffer_1, data, sizeof(data));
  TEST_ASSERT_EQUAL_STRING(data, "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39");
  line_buffer__remove_line(&line_buffer_1, data, sizeof(data));
  TEST_ASSERT_EQUAL_STRING(data, "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
  line_buffer__remove_line(&line_buffer_1, data, sizeof(data));
  TEST_ASSERT_EQUAL_STRING(data, "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39");
}
#include "unity.h"

#include "bridge_buffer.h"

void test_is_buffer_empty_negative() {
  front = -1;
  rear = -1;
  TEST_ASSERT_TRUE(is_buffer_empty());
}

void test_is_buffer_empty_positive() {
  front = -1;
  rear = -1;

  push_buffer('A');
  push_buffer('B');
  TEST_ASSERT_FALSE(is_buffer_empty());
}

void test_get_count() {
  front = -1;
  rear = -1;

  char byte;
  push_buffer('A');
  push_buffer('B');
  push_buffer('A');
  push_buffer('B');

  pop_buffer(&byte);

  TEST_ASSERT_EQUAL(3, get_count());
}

void test_push_buffer_positive() {

  front = -1;
  rear = -1;
  gps_buffer[SIZE];
  char byte = 'A';

  push_buffer(&byte);
  push_buffer(&byte);
  push_buffer(&byte);

  TEST_ASSERT_EQUAL('A', gps_buffer[2]);
}

void test_push_buffer_negative() {

  front = -1;
  rear = -1;
  gps_buffer[SIZE];
  memset(gps_buffer, 0, SIZE);
  char byte = 160;

  push_buffer(&byte);
  push_buffer(&byte);
  push_buffer(&byte);
  push_buffer(&byte);
  push_buffer(&byte);

  TEST_ASSERT_EQUAL(0, gps_buffer[0]);
}

void test_pop_buffer_one() {

  char byte;

  front = -1;
  rear = -1;
  gps_buffer[SIZE];
  memset(gps_buffer, 0, SIZE);

  push_buffer('A');
  push_buffer('A');
  push_buffer('A');

  pop_buffer(&byte);

  TEST_ASSERT_EQUAL('A', byte);
  TEST_ASSERT_TRUE(pop_buffer(&byte));
}

void test_pop_buffer_two() {

  char byte;
  front = -1;
  rear = -1;

  TEST_ASSERT_FALSE(pop_buffer(&byte));
}

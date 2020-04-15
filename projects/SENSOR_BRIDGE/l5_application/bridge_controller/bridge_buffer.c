#include "bridge_buffer.h"

int front = -1, rear = -1;
unsigned char gps_buffer[SIZE];

bool is_buffer_empty() {
  if (front == -1 && rear == -1)
    return true;
  else
    return false;
}

void push_buffer(char value) {

  if (isalnum(value) || isspace(value) || ispunct(value)) {
    if ((rear + 1) % SIZE == front) {
      return;
    } else if (front == -1 && rear == -1) {
      front = 0;
      rear = 0;
    } else {
      rear = (rear + 1) % SIZE;
    }
    gps_buffer[rear] = value;
  }
}

bool pop_buffer(char *value) {

  if (is_buffer_empty()) {
    return false;
  } else {
    if (front == rear) {
      *value = gps_buffer[front];
      front = -1;
      rear = -1;
    } else {
      *value = gps_buffer[front];
      front = (front + 1) % SIZE;
    }

    return true;
  }
}

void set__front_back() { front = rear = -1; }

int get_count(void) {
  int count = (rear + SIZE - front) % SIZE + 1;
  return count;
}

void print_buffer(void) {
  int count = get_count();
  for (int i = 0; i < count; i++) {
    int index = (front + i) % SIZE;
    printf("%c", gps_buffer[index]);
  }
}

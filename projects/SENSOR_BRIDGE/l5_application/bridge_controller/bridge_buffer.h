#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#define SIZE 500

bool is_buffer_empty();
void push_buffer(char value);
bool pop_buffer(char *value);
int get_count(void);
void print_buffer(void);

extern int front;
extern int rear;
extern unsigned char gps_buffer[SIZE];

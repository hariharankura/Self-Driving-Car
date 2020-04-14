#include "bridge_controller_handler.h"

static int address_count = 0;
static bool previous_line_data_flag;

void bridge_controller_handler__initialize_bluetooth_module(void) {

  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // Tx
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // Rx

  QueueHandle_t rxq_handle = xQueueCreate(300, sizeof(unsigned char));
  QueueHandle_t txq_handle = xQueueCreate(300, sizeof(unsigned char));

  // UART  Initialize
  uart__init(UART__3, clock__get_peripheral_clock_hz(), 38400U);
  uart__enable_queues(UART__3, rxq_handle, txq_handle);
}

void bridge_controller_handler__convert_gps_data_to_degrees(gps_data *data) {
  // Latitude - ddmm.mmmm
  // Longitutde - dddmm.mmmm
  // https://www.pgc.umn.edu/apps/convert/ - To Confirm

  uint32_t latitude_degree = data->latitude / 100;
  uint32_t longitude_degree = data->longitude / 100;

  float latitude_minutes = (data->latitude) - (latitude_degree * 100);
  float longitude_minutes = (data->longitude) - (longitude_degree * 100);

  data->latitude = latitude_degree + (latitude_minutes / 60.0f);
  data->longitude = longitude_degree + (longitude_minutes / 60.0f);
}

void bridge_controller_handler__assign_lat_long_direction(gps_data *data) {
  if (data->longitude_direction == 'W' || data->longitude_direction == 'w') {
    data->longitude = data->longitude * -1;
  }
  if (data->latitude_direction == 'S' || data->latitude_direction == 's') {
    data->latitude = data->latitude * -1;
  }
}

void bridge_controller_handler__parse_gps_data(const char *input_buffer, float *latitude, float *longitude) {
  int j = 0;

  gps_data data;

  // GPS messages start with $ char
  if (input_buffer[0] == '$' && address_count < 5) {

    // Check if string we collected is the $GPGGA message
    if (input_buffer[3] == 'G' && input_buffer[4] == 'G' && input_buffer[5] == 'A') {

      address_count++;

      sscanf(input_buffer, "$GPGGA,%*f,%f,%c,%f,%c", &(data.latitude), &data.latitude_direction, &data.longitude,
             &data.longitude_direction);

      // printf("%s\n", input_buffer);

      // convert latitude and longitude to degrees and minutes
      bridge_controller_handler__convert_gps_data_to_degrees(&data);

      // assign direction sign
      bridge_controller_handler__assign_lat_long_direction(&data);

      *latitude = data.latitude;
      *longitude = data.longitude;

      // printf("Latitude: %f\n", data.latitude);
      // printf("Longitude: %f\n", data.longitude);
    }
  }
}

void bridge_controller_handler__get_data_from_uart(void) {
  unsigned char byte;
  while (uart__get(UART__3, &byte, 0)) {
    push_buffer(byte);
    // printf("%c", byte);
  }
}

bool bridge_controller_handler__buffer_has_gps_message(const char *array, int *message_count) {
  bool has_line = false;
  int read_index = front;
  int count_max = get_count();
  for (int i = 0; (i < count_max && array[read_index] != '\0'); i++) {
    if (array[read_index] == '\n') {
      has_line = true;
      (*message_count)++;
      break;
    }
    read_index = (1 + read_index) % SIZE;
  }
  return has_line;
}

void bridge_controller_handler__get_single_gps_message(char *line) {
  int i = 0;
  while (gps_buffer[front] != '\n') {
    char value;
    if (pop_buffer(&value)) {
      line[i] = value;
      i++;
    }
  }
  front = (front + 1) % SIZE;
  line[i] = '\0';
}

bool bridge_controller_handler__get_gps_message_from_buffer(char *temp_buffer) {
  bool return_value = false;
  int message_count = 0;
  if (bridge_controller_handler__buffer_has_gps_message(gps_buffer, &message_count)) {
    if (message_count > 0) {
      bridge_controller_handler__get_single_gps_message(temp_buffer);
      return_value = true;
    }
  }
  return return_value;
}

void bridge_controller_handler__get_gps_coordinates(float *latitude, float *longitude) {
  char temp_buffer[100];
  if (bridge_controller_handler__get_gps_message_from_buffer(temp_buffer)) {
    bridge_controller_handler__parse_gps_data(temp_buffer, latitude, longitude);
  }
}

void bridge_controller_handler__get_destination_coordinates(float *latitude, float *longitude) {

  bridge_controller_handler__get_data_from_uart();
  bridge_controller_handler__get_start_stop_condition();
  bridge_controller_handler__get_gps_coordinates(latitude, longitude);
}

bool bridge_controller_handler__get_start_stop_condition() {
  char *line;
  bridge_controller_handler__get_single_gps_message(line);
  if (line == 'START') {
    previous_line_data_flag = true;
    return true;
  } else if (line == 'STOP') {
    previous_line_data_flag = false;
    return false;
  } else
    return previous_line_data_flag;
}

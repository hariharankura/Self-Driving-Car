#include "FreeRTOS.h"

#include "driver_logic.h"
#include "queue.h"

static QueueHandle_t steer_data_queue;
static QueueHandle_t speed_data_queue;

void driver_logic__process_ultrasonic_sensors_data(dbc_SENSOR_USONARS_s sensor_data) {}

bool driver_logic__check_if_steer_data_to_send(dbc_DRIVER_STEER_s *steer_info) {
  bool return_flag = false;
  if (pdTRUE == xQueueReceive(steer_data_queue, steer_info, 0)) {
    return_flag = true;
  }
  return return_flag;
}

bool driver_logic__check_if_speed_data_to_send(dbc_MOTOR_SPEED_s *speed_info) {
  bool return_flag = false;
  if (pdTRUE == xQueueReceive(speed_data_queue, speed_info, 0)) {
    return_flag = true;
  }
  return return_flag;
}
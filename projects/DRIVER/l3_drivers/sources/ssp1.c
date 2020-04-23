#include "ssp1.h"
#include "gpio.h"
#include "lpc40xx.h"

#define A_CPU_CLOCK_FREQUENCY 96 * 1000 * 1000

void ssp1__init(uint32_t clock_frequency_in_mhz) {
  LPC_SC->PCONP |= (1 << 10);
  LPC_IOCON->P0_7 = (0b010 << 0);
  LPC_IOCON->P0_8 = (0b010 << 0);
  LPC_IOCON->P0_9 = (0b010 << 0);
  LPC_SSP1->CR0 = (0b111 << 0);
  LPC_SSP2->CR1 = (1 << 1);
  LPC_SSP1->CPSR = (A_CPU_CLOCK_FREQUENCY / (1000 * 1000)) / (clock_frequency_in_mhz);
}

bool ssp1__is_bus_busy() { return (LPC_SSP1->SR & (1 << 0)); }

uint8_t ssp1__transfer_data_1_byte(uint8_t data) {
  LPC_SSP1->DR = data;

  // printf("Data to be written = %x\n", data);
  while (ssp1__is_bus_busy()) {
    continue;
  }
  // printf("Data written to ssp\n");
  return (uint8_t)LPC_SSP1->DR;
}
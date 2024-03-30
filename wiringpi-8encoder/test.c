#include <stdio.h>
#include "wiringpi-8encoder.h"

int main(int argc, char* argv[]) {
  int fd = wiringPiI2CSetup(ENCODER_ADDR);
  printf("switch: %s\n", wiringpi_8encoder_getSwitchStatus(fd) ? "up" : "down");
}
#include "linux-8encoder.h"

int main(int argc, char *argv[]) {
  int bus = open("/dev/i2c-1", O_RDWR);
  if (file < 0) {
    return 1;
  }
  if (ioctl(bus, I2C_SLAVE, ENCODER_ADDR) < 0) {
    return 2;
  }



  return 0;
}
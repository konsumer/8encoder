#include "linux-8encoder.h"


int main(int argc, char *argv[]) {
  int i2c = open("/dev/i2c-1", O_RDWR);
  if (i2c < 0) {
    return 1;
  }
  if (ioctl(i2c, I2C_SLAVE, ENCODER_ADDR) < 0) {
    return 2;
  }

  linux_8encoder_get_firmware_version

  printf("Firmware version: %d\nAddress: 0x%.2x\n", linux_8encoder_get_firmware_version(i2c), linux_8encoder_get_address(i2c));
  
  for (int i = 0; i < 8; i++) {
    linux_8encoder_set_encoder_value(i2c, i, 0);
  }

  while(true) {
    for (int i = 0; i < 8; i++) {
      if (linux_8encoder_is_button_down(i2c, i)) {
        linux_8encoder_set_led_color_int(i2c, i, 0xffffff);
      } else {
        // make a rainbow with HSV
        linux_8encoder_set_led_color_hsv(i2c, i, (float)i * 0.125, 1.0, 1.0);
      }
    }
    printf("%d\n", linux_8encoder_get_encoder_value(i2c, 0));
  }

  return 0;
}

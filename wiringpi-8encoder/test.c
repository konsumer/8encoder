#include <stdio.h>
#include "wiringpi-8encoder.h"

int main(int argc, char* argv[]) {
  int fd;

  if ((fd = wiringPiI2CSetup(ENCODER_ADDR)) < 0) {
    printf("I2CSetup Failed, %i\n", fd);
    return 1;
  }

  printf("Firmware version: %d\nAddress: 0x%.2x\n", wiringpi_8encoder_get_firmware_version(fd), wiringpi_8encoder_get_address(fd));

  wiringpi_8encoder_set_led_color_int(fd, 0, 0xff0000);
  wiringpi_8encoder_set_led_color_int(fd, 1, 0x00ff00);
  wiringpi_8encoder_set_led_color_int(fd, 2, 0x0000ff);

  int i;

  bool b[8] = {false};
  bool bo[8] = {false};
  uint8_t r[8] = {false};
  uint8_t ro[8] = {false};
  bool s = false;
  bool os = false;

  // set/display initial
  s = wiringpi_8encoder_get_switch_value(fd);
  printf("Switch position: %d\n", s);
  os = s;
  for (i = 0; i < 8; i++) {
    b[i] = wiringpi_8encoder_is_button_down(fd, i);
    printf("Button %d %s\n", i, b[i] ? "PUSHED" : "NO");
    bo[i] = b[i];
    r[i] = wiringpi_8encoder_get_encoder_value(fd, i);
    printf("Rotary %d %d\n", i, r[i]);
    ro[i] = r[i];
  }

  // fire events on change
  while (1) {
    s = wiringpi_8encoder_get_switch_value(fd);
    if (s != os) {
      printf("Switch position: %d\n", s);
    }
    os = s;

    for (i = 0; i < 8; i++) {
      b[i] = wiringpi_8encoder_is_button_down(fd, i);
      if (b[i] != bo[i]) {
        printf("Button %d %s\n", i, b[i] ? "PUSHED" : "NO");
      }
      bo[i] = b[i];

      r[i] = wiringpi_8encoder_get_encoder_value(fd, i);
      if (r[i] != ro[i]) {
        printf("Rotary %d %d\n", i, r[i]);
      }
      ro[i] = r[i];
    }
  }

  return 0;
}
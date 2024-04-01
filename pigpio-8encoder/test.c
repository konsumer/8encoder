#include <stdio.h>
#include "pigpio-8encoder.h"

int main(int argc, char* argv[]) {
  if (gpioInitialise() < 0) {
    printf("Error: pigpio init failed\n");
  }

  int i2c = i2cOpen(1, ENCODER_ADDR, 0);

  if (i2c < 0) {
    printf("Error: ");
    if (i2c == PI_BAD_I2C_BUS) {
      printf("Bad i2c bus.\n");
    } else if (i2c == PI_BAD_I2C_ADDR) {
      printf("Bad i2c address.\n");
    } else if (i2c == PI_BAD_FLAGS) {
      printf("Bad i2c flags.\n");
    } else if (i2c == PI_NO_HANDLE) {
      printf("No i2c handle.\n");
    } else if (i2c == PI_I2C_OPEN_FAILED) {
      printf("i2c open failed.\n");
    } else {
      printf("Unkown.\n");
    }
    return 1;
  }

  printf("Firmware version: %d\nAddress: 0x%.2x\n", pigio_8encoder_get_firmware_version(i2c), pigio_8encoder_get_address(i2c));

  for (int i = 0; i < 8; i++) {
    pigio_8encoder_set_led_color_hsv(i2c, i, (float)i * 0.125, 1.0, 1.0);
  }

  bool s = false;
  bool os = false;
  bool b[8] = {false};
  bool bo[8] = {false};
  uint8_t r[8] = {0};
  uint8_t ro[8] = {0};

  // set/display initial
  s = pigio_8encoder_get_switch_value(i2c);
  printf("Switch position: %d\n", s);
  os = s;
  for (int i = 0; i < 8; i++) {
    pigio_8encoder_set_encoder_value(i2c, i, 0);
    printf("Button %d NO\n", i);
    printf("Rotary %d 0\n", i);
  }

  // fire events on change
  while (1) {
    s = pigio_8encoder_get_switch_value(i2c);
    if (s != os) {
      printf("Switch position: %d\n", s);
    }
    os = s;

    for (int i = 0; i < 8; i++) {
      b[i] = pigio_8encoder_is_button_down(i2c, i);
      if (b[i] != bo[i]) {
        printf("Button %d %s\n", i, b[i] ? "PUSHED" : "NO");
      }
      bo[i] = b[i];

      r[i] = pigio_8encoder_get_encoder_value(i2c, i);
      if (r[i] != ro[i]) {
        printf("Rotary %d %d\n", i, r[i]);
      }
      ro[i] = r[i];
    }
  }

  i2cClose(i2c);
  gpioTerminate();
  return 0;
}
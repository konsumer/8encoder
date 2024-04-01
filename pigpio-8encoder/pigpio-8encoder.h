#pragma once
#include <pigpio.h>
#include <stdbool.h>
#include <string.h>

#define ENCODER_ADDR 0x41
#define ENCODER_REG 0x00
#define INCREMENT_REG 0x20
#define BUTTON_REG 0x50
#define SWITCH_REG 0x60
#define RGB_LED_REG 0x70
#define RESET_COUNTER_REG 0x40
#define FIRMWARE_VERSION_REG 0xFE
#define I2C_ADDRESS_REG 0xFF

// convert array of 4 uint8 to 1 uint32
uint32_t uint8s_to_32(uint8_t v4[4]);

// HSV is 0-1 floats, returns single uint32
uint32_t hsvToRgbInt(float h, float s, float v);

// Check to see if a rotary pushbutton (0-7) is pressed
bool pigio_8encoder_is_button_down(int i2c, uint8_t index);

// Get the current value of an encoder (0-7)
int pigio_8encoder_get_encoder_value(int i2c, uint8_t index);

// Set the current value of an encoder (0-7)
void pigio_8encoder_set_encoder_value(int i2c, uint8_t index, int32_t value);

// Get the increment-value for an encoder (0-7)
// TODO: not sure if this really works right
uint32_t pigio_8encoder_get_increment_value(int i2c, uint8_t index);

// Get current position of toggle switch
uint8_t pigio_8encoder_get_switch_value(int i2c);

// Set the RGB LED (0-7) color using seperate uint8's for R, G, B
void pigio_8encoder_set_led_color_rgb(int i2c, uint8_t index, uint8_t r, uint8_t g, uint8_t b);

// Set the RGB LED (0-7) color using a single int like 0xFF0000
void pigio_8encoder_set_led_color_int(int i2c, uint8_t index, uint32_t color);

// Set the RGB LED (0-7) color using seperate 0-1 floats for H, S, V
void pigio_8encoder_set_led_color_hsv(int i2c, uint8_t index, float h, float s, float v);

// Get the firmware-version, on the controller
uint8_t pigio_8encoder_get_firmware_version(int i2c);

// Set the current i2c address for device
uint8_t pigio_8encoder_set_address(int i2c, uint8_t addr);

// Get the current i2c address from device
uint8_t pigio_8encoder_get_address(int i2c);

// Reset a counter (by index: 0-7)
void pigio_8encoder_reset_counter(int i2c, uint8_t index);

#ifndef NO_PIGPIO_8ENCODER_IMPLEMENTATION

uint32_t uint8s_to_32(uint8_t v[4]) {
  return v[0] | (v[1] << 8) | (v[2] << 16) | (v[3] << 24);
}

uint32_t hsvToRgbInt(float h, float s, float v) {
  uint8_t hsv[3] = {(uint8_t)(h * 255.0), (uint8_t)(s * 255.0), (uint8_t)(v * 255.0)};

  uint8_t rgb[4] = {0};
  uint8_t region, remainder, p, q, t;

  if (hsv[1] == 0) {
    rgb[0] = hsv[2];
    rgb[1] = hsv[2];
    rgb[2] = hsv[2];
    return uint8s_to_32(rgb);
  }

  region = hsv[0] / 43;
  remainder = (hsv[0] - (region * 43)) * 6;

  p = (hsv[2] * (255 - hsv[1])) >> 8;
  q = (hsv[2] * (255 - ((hsv[1] * remainder) >> 8))) >> 8;
  t = (hsv[2] * (255 - ((hsv[1] * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
    case 0:
      rgb[0] = hsv[2];
      rgb[1] = t;
      rgb[2] = p;
      break;
    case 1:
      rgb[0] = q;
      rgb[1] = hsv[2];
      rgb[2] = p;
      break;
    case 2:
      rgb[0] = p;
      rgb[1] = hsv[2];
      rgb[2] = t;
      break;
    case 3:
      rgb[0] = p;
      rgb[1] = q;
      rgb[2] = hsv[2];
      break;
    case 4:
      rgb[0] = t;
      rgb[1] = p;
      rgb[2] = hsv[2];
      break;
    default:
      rgb[0] = hsv[2];
      rgb[1] = p;
      rgb[2] = q;
      break;
  }

  return uint8s_to_32(rgb);
}

bool pigio_8encoder_is_button_down(int i2c, uint8_t index) {
  i2cWriteByte(i2c, BUTTON_REG + index);
  return !i2cReadByte(i2c);
}

int pigio_8encoder_get_encoder_value(int i2c, uint8_t index) {
  i2cWriteByte(i2c, ENCODER_REG + (index * 4));
  uint8_t a[4] = {
      i2cReadByte(i2c),
      i2cReadByte(i2c),
      i2cReadByte(i2c),
      i2cReadByte(i2c)};
  return uint8s_to_32(a);
}

void pigio_8encoder_set_encoder_value(int i2c, uint8_t index, int32_t value) {
  i2cWriteByte(i2c, ENCODER_REG + (index * 4));
  uint8_t a[4];
  memcpy(a, &value, 4);
  for (int i = 0; i < 4; i++) {
    i2cWriteByte(i2c, a[i]);
  }
}

uint8_t pigio_8encoder_get_switch_value(int i2c) {
  i2cWriteByte(i2c, SWITCH_REG);
  return i2cReadByte(i2c);
}

uint8_t pigio_8encoder_get_firmware_version(int i2c) {
  i2cWriteByte(i2c, FIRMWARE_VERSION_REG);
  return i2cReadByte(i2c);
}

uint8_t pigio_8encoder_get_address(int i2c) {
  i2cWriteByte(i2c, I2C_ADDRESS_REG);
  return i2cReadByte(i2c);
}

void pigio_8encoder_set_led_color_rgb(int i2c, uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t m[4] = {RGB_LED_REG + (index * 3), r, g, b};
  i2cWriteDevice(i2c, m, 4);
}

void pigio_8encoder_set_led_color_int(int i2c, uint8_t index, uint32_t color) {
  uint8_t a[3];
  memcpy(a, &color, 3);
  pigio_8encoder_set_led_color_rgb(i2c, index, a[0], a[1], a[2]);
}

void pigio_8encoder_set_led_color_hsv(int i2c, uint8_t index, float h, float s, float v) {
  uint32_t color = hsvToRgbInt(h, s, v);
  pigio_8encoder_set_led_color_int(i2c, index, color);
}

#endif  // NO_PIGPIO_8ENCODER_IMPLEMENTATION
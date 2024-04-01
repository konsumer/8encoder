#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <wiringPi.h>
#include "wiringPiI2C_fork.h"

// convert array of 4 uint8 to 1 uint32
uint32_t uint8s_to_32(uint8_t v4[4]) {
  return v4[0] | (v4[1] << 8) | (v4[2] << 16) | (v4[3] << 24);
}

// HSV is 0-1 floats, returns single uint32
uint32_t hsvToRgbInt(float h, float s, float v) {
  uint8_t hsv[3] = {0};

  float cnum = 1.0 / 255.0;

  hsv[0] = (uint8_t)(h * cnum);
  hsv[1] = (uint8_t)(s * cnum);
  hsv[2] = (uint8_t)(v * cnum);

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

#define ENCODER_ADDR 0x41
#define ENCODER_REG 0x00
#define INCREMENT_REG 0x20
#define BUTTON_REG 0x50
#define SWITCH_REG 0x60
#define RGB_LED_REG 0x70
#define RESET_COUNTER_REG 0x40
#define FIRMWARE_VERSION_REG 0xFE
#define I2C_ADDRESS_REG 0xFF

// Check to see if a rotary pushbutton (0-7) is pressed
bool wiringpi_8encoder_is_button_down(uint8_t fd, uint8_t index) {
  wiringPiI2CWrite(fd, BUTTON_REG + index);
  int i = wiringPiI2CRead(fd);
  return i != 1;
}

// Get the current value of an encoder (0-7)
int wiringpi_8encoder_get_encoder_value(uint8_t fd, uint8_t index) {
  wiringPiI2CWrite(fd, ENCODER_REG + (index * 4));
  return wiringPiI2CRead(fd);
}

// Set the current value of an encoder (0-7)
void wiringpi_8encoder_set_encoder_value(uint8_t fd, uint8_t index, int32_t value) {}

// Get the increment-value for an encoder (0-7)
// TODO: not sure if this really works right
uint32_t wiringpi_8encoder_get_increment_value(uint8_t fd, uint8_t index) {
  wiringPiI2CWrite(fd, INCREMENT_REG + (index * 4));
  return wiringPiI2CRead(fd);
}

// Get current position of toggle switch
uint8_t wiringpi_8encoder_get_switch_value(uint8_t fd) {
  wiringPiI2CWrite(fd, SWITCH_REG);
  return wiringPiI2CRead(fd);
}

// Set the RGB LED (0-7) color using seperate uint8's for R, G, B
void wiringpi_8encoder_set_led_color_rgb(uint8_t fd, uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t c[3] = {r, g, b};
  wiringPiI2CWriteBlockData(fd, 3, RGB_LED_REG + (index * 3), c);
}

// Set the RGB LED (0-7) color using a single int like 0xFF0000
void wiringpi_8encoder_set_led_color_int(uint8_t fd, uint8_t index, uint32_t color) {
  uint8_t red = (color >> 16) & 0xff;
  uint8_t green = (color >> 8) & 0xff;
  uint8_t blue = color & 0xff;
  wiringpi_8encoder_set_led_color_rgb(fd, index, red, green, blue);
}

// Set the RGB LED (0-7) color using seperate 0-1 floats for H, S, V
void wiringpi_8encoder_set_led_color_hsv(uint8_t fd, uint8_t index, float h, float s, float v) {
  uint32_t color = hsvToRgbInt(h, s, v);
  wiringpi_8encoder_set_led_color_int(fd, index, color);
}

// Get the firmware-version, on the controller
uint8_t wiringpi_8encoder_get_firmware_version(uint8_t fd) {
  wiringPiI2CWrite(fd, FIRMWARE_VERSION_REG);
  return wiringPiI2CRead(fd);
}

// Set the current i2c address for device
uint8_t wiringpi_8encoder_set_address(uint8_t fd, uint8_t addr) {}

// Get the current i2c address from device
uint8_t wiringpi_8encoder_get_address(uint8_t fd) {
  wiringPiI2CWrite(fd, I2C_ADDRESS_REG);
  return wiringPiI2CRead(fd);
}

// Reset a counter (by index: 0-7)
void wiringpi_8encoder_reset_counter(uint8_t fd, uint8_t index) {}

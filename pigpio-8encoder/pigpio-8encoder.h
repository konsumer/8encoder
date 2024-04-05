#pragma once
#include <pigpio.h>
#include <stdbool.h>
#include <string.h>
#include "../shared.h"

typedef union {
  unsigned int integer;
  unsigned char byte[4];
} pigio_8encoder_i32;

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

// define this, if you just want to use this as a header
#ifndef NO_PIGPIO_8ENCODER_IMPLEMENTATION


bool pigio_8encoder_is_button_down(int i2c, uint8_t index) {
  i2cWriteByte(i2c, BUTTON_REG + index);
  return !i2cReadByte(i2c);
}

int pigio_8encoder_get_encoder_value(int i2c, uint8_t index) {
  i2cWriteByte(i2c, ENCODER_REG + (index * 4));
  pigio_8encoder_i32 a;
  a.byte[0] = i2cReadByte(i2c);
  a.byte[1] = i2cReadByte(i2c);
  a.byte[2] = i2cReadByte(i2c);
  a.byte[3] = i2cReadByte(i2c);
  return a.integer;
}

void pigio_8encoder_set_encoder_value(int i2c, uint8_t index, int32_t value) {
  pigio_8encoder_i32 i;
  i.integer = value;
  uint8_t a[5] = {
      ENCODER_REG + (index * 4),
      i.byte[0],
      i.byte[1],
      i.byte[2],
      i.byte[3]};
  i2cWriteDevice(i2c, a, 5);
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
  uint32_t color = hsv_to_rgb_int(h, s, v);
  pigio_8encoder_set_led_color_int(i2c, index, color);
}

#endif  // NO_PIGPIO_8ENCODER_IMPLEMENTATION
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <wiringPi.h>
#include "wiringPiI2C_fork.h"
#include "../shared.h"

#define ENCODER_ADDR 0x41
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
  uint32_t color = hsv_to_rgb_int(h, s, v);
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

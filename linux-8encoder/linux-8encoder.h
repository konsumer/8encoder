#pragma once

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include "../shared.h"
#include <stdbool.h> 

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
bool linux_8encoder_is_button_down(int i2c, uint8_t index);

// Get the current value of an encoder (0-7)
int linux_8encoder_get_encoder_value(int i2c, uint8_t index);

// Set the current value of an encoder (0-7)
void linux_8encoder_set_encoder_value(int i2c, uint8_t index, int32_t value);

// Get the increment-value for an encoder (0-7)
// TODO: not sure if this really works right
uint32_t linux_8encoder_get_increment_value(int i2c, uint8_t index);

// Get current position of toggle switch
uint8_t linux_8encoder_get_switch_value(int i2c);

// Set the RGB LED (0-7) color using seperate uint8's for R, G, B
void linux_8encoder_set_led_color_rgb(int i2c, uint8_t index, uint8_t r, uint8_t g, uint8_t b);

// Set the RGB LED (0-7) color using a single int like 0xFF0000
void linux_8encoder_set_led_color_int(int i2c, uint8_t index, uint32_t color);

// Set the RGB LED (0-7) color using seperate 0-1 floats for H, S, V
void linux_8encoder_set_led_color_hsv(int i2c, uint8_t index, float h, float s, float v);

// Get the firmware-version, on the controller
uint8_t linux_8encoder_get_firmware_version(int i2c);

// Set the current i2c address for device
uint8_t linux_8encoder_set_address(int i2c, uint8_t addr);

// Get the current i2c address from device
uint8_t linux_8encoder_get_address(int i2c);

// Reset a counter (by index: 0-7)
void linux_8encoder_reset_counter(int i2c, uint8_t index);

// define this, if you just want to use this as a header
#ifndef NO_LINUX_8ENCODER_IMPLEMENTATION



#endif  // NO_LINUX_8ENCODER_IMPLEMENTATION
#pragma once

// TODO: do I really need all these?
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../shared.h"

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

// 4 bytes of data used back & forth
uint8_t linux_8encoder_buf[5];

bool linux_8encoder_is_button_down(int i2c, uint8_t index) {
  linux_8encoder_buf[0] = BUTTON_REG + index;
  if (write(i2c, linux_8encoder_buf, 1) != 1) {
    printf("Error getting button status (write) %d\n", index);
    return false;
  } else {
    if (read(i2c, linux_8encoder_buf, 1) != 1) {
      printf("Error getting button status (read) %d\n", index);
      return false;
    }
    // 1us pause helps with missing messages
    usleep(1);
    return linux_8encoder_buf[0] == 0;
  }
}

int linux_8encoder_get_encoder_value(int i2c, uint8_t index) {
  linux_8encoder_buf[0] = ENCODER_REG + (index * 4);
  if (write(i2c, linux_8encoder_buf, 1) != 1) {
    printf("Error getting encoder value (write) %d\n", index);
    return 0;
  } else {
    if (read(i2c, linux_8encoder_buf, 4) != 4) {
      printf("Error getting button value (read) %d\n", index);
      return 0;
    }
    // 1us pause helps with missing messages
    usleep(1);
    int32_t v = 0;
    memcpy(&v, &linux_8encoder_buf, 4); 
    return v;
  }
}

void linux_8encoder_set_encoder_value(int i2c, uint8_t index, int32_t value) {
  linux_8encoder_buf[0] = ENCODER_REG + (index * 4);
  memcpy(&linux_8encoder_buf[1], &value, 4);
  if (write(i2c, linux_8encoder_buf, 5) != 5) {
    printf("Error setting encoder value (write) %d\n", index);
  } else {
    // 1us pause helps with missing messages
    usleep(1);
  }
}

uint32_t linux_8encoder_get_increment_value(int i2c, uint8_t index);

uint8_t linux_8encoder_get_switch_value(int i2c);

void linux_8encoder_set_led_color_rgb(int i2c, uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  linux_8encoder_buf[0] = RGB_LED_REG + (index*3);
  linux_8encoder_buf[1] = r;
  linux_8encoder_buf[2] = b;
  linux_8encoder_buf[3] = g;
  if (write(i2c, linux_8encoder_buf, 4) != 4) {
    printf("Error writing RGB (%d): %d, %d, %d\n", index, r, g, b);
  } else {
    // 1us pause helps with missing messages
    usleep(1);
  }
}

void linux_8encoder_set_led_color_int(int i2c, uint8_t index, uint32_t color) {
  memcpy(linux_8encoder_buf, &color, 3);
  linux_8encoder_set_led_color_rgb(i2c, index, linux_8encoder_buf[0], linux_8encoder_buf[1], linux_8encoder_buf[2]);
}

void linux_8encoder_set_led_color_hsv(int i2c, uint8_t index, float h, float s, float v) {
  linux_8encoder_set_led_color_int(i2c, index, hsv_to_rgb_int(h, s, v));
}

uint8_t linux_8encoder_get_firmware_version(int i2c) {
  linux_8encoder_buf[0] = ENCODER_REG;
  if (write(i2c, linux_8encoder_buf, 1) != 1) {
    printf("Error getting firmware version (write)\n");
    return 0;
  } else {
    if (read(i2c, linux_8encoder_buf, 1) != 1) {
      printf("Error getting firmware version (read)\n");
      return 0;
    }
    // 1us pause helps with missing messages
    usleep(1);
    return linux_8encoder_buf[0];
  }
}

uint8_t linux_8encoder_set_address(int i2c, uint8_t addr);

uint8_t linux_8encoder_get_address(int i2c);

void linux_8encoder_reset_counter(int i2c, uint8_t index);


#endif  // NO_LINUX_8ENCODER_IMPLEMENTATION

#pragma once

#define DONT_CARE_ANYMORE

#include <stdbool.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define ENCODER_ADDR 0x41
#define ENCODER_REG 0x00
#define INCREMENT_REG 0x20
#define BUTTON_REG 0x50
#define SWITCH_REG 0x60
#define RGB_LED_REG 0x70
#define RESET_COUNTER_REG 0x40
#define FIRMWARE_VERSION_REG 0xFE
#define I2C_ADDRESS_REG 0xFF

uint32_t wiringpi_8encoder_getEncoderValue(int fd, uint8_t index) {}

void wiringpi_8encoder_setEncoderValue(int fd, uint8_t index, int32_t value) {}

uint32_t wiringpi_8encoder_getIncrementValue(int fd, uint8_t index) {}

bool wiringpi_8encoder_getButtonStatus(int fd, uint8_t index) {}

bool wiringpi_8encoder_getSwitchStatus(int fd) {
  return wiringPiI2CReadReg8(fd, SWITCH_REG);
}

void wiringpi_8encoder_setLEDColor(int fd, uint8_t index, uint32_t color) {}

void wiringpi_8encoder_setAllLEDColor(int fd, uint32_t color) {}

uint8_t wiringpi_8encoder_setI2CAddress(int fd, uint8_t addr) {}

uint8_t wiringpi_8encoder_getI2CAddress(int fd) {}

uint8_t wiringpi_8encoder_getFirmwareVersion(int fd) {}

void wiringpi_8encoder_resetCounter(int fd, uint8_t index) {}

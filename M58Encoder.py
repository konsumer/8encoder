import ctypes
from colorsys import hls_to_rgb, hsv_to_rgb

ENCODER_ADDR         = 0x41
ENCODER_REG          = 0x00
INCREMENT_REG        = 0x20
BUTTON_REG           = 0x50
SWITCH_REG           = 0x60
RGB_LED_REG          = 0x70
RESET_COUNTER_REG    = 0x40
FIRMWARE_VERSION_REG = 0xFE
I2C_ADDRESS_REG      = 0xFF

# this is a port of https://github.com/m5stack/M5Unit-8Encoder/blob/main/src/UNIT_8ENCODER.cpp

class M58Encoder:
  def __init__(self, i2c, address=ENCODER_ADDR):
    self.i2c = i2c
    self.address = address

  def read_bytes(self, reg, result=bytearray(1)):
    try:
      self.i2c.writeto(self.address, bytes([reg]))
      self.i2c.readfrom_into(self.address, result)
    except OSError:
      pass
    return result

  def read_int(self, reg):
    return int.from_bytes(self.read_bytes(reg, bytearray(4)), "little")

  def write_bytes(self, reg, value):
    try:
      self.i2c.writeto(self.address, bytes([reg]) + value)
    except OSError:
      pass
  def write_int(self, reg, value):
    self.write_bytes(reg, bytes(ctypes.c_int32(value)))

  def is_button_down(self, index):
    """
    Check to see if a rotary pushbutton (0-7) is pressed
    """
    result = self.read_bytes(index + BUTTON_REG)
    if result[0] == 1:
      return False
    else:
      return True

  def get_encoder_value(self, index):
    """
    Get the current value of an encoder (0-7)
    """
    return self.read_int((index * 4) + ENCODER_REG)

  def set_encoder_value(self, index, value):
    """
    Set the current value of an encoder (0-7)
    """
    self.write_int((index * 4) + ENCODER_REG, value)

  def get_increment_value(self, index):
    """
    Get the increment-value for an encoder (0-7)
    Not sure if this actually works
    """
    return self.read_int((index * 4) + INCREMENT_REG)

  def get_switch_value(self):
    """
    Get current position of toggle switch
    """
    return self.read_bytes(SWITCH_REG)[0]

  def set_led_color_rgb(self, index, red, green, blue):
    """
    Set the RGB LED (0-7) color using seperate uint8's for R, G, B
    """
    self.write_bytes(index * 3 + RGB_LED_REG, bytes([red, green, blue]))

  def set_led_color_int(self, index, val):
    """
    Set the RGB LED (0-7) color using a single int like 0xFF0000
    """
    blue = val & 0xff
    green = (val >> 8) & 0xff
    red = (val >> 16) & 0xff
    self.set_led_color_rgb(index, red, green, blue)

  def set_led_color_hsv(self, index, h, s, v):
    """
    Set the RGB LED (0-7) color using seperate 0-1 floats for H, S, V
    """
    r, g, b = hsv_to_rgb(h, s, v)
    self.set_led_color_rgb(index, int(r * 0xff), int(g * 0xff), int(b * 0xff))

  def get_firmware_version(self):
    """
    Get the firmware-version, on the controller
    """
    return self.read_bytes(FIRMWARE_VERSION_REG)[0]

  def set_address(self, value):
    """
    Set the current i2c address for device
    """
    self.write_bytes(I2C_ADDRESS_REG, bytes([value]))
    self.address = value

  def get_address(self):
    """
    Get the current i2c address from device
    """
    return self.read_bytes(I2C_ADDRESS_REG)[0]

  def reset_counter(self, index):
    """
    Reset a counter (by index: 0-7)
    """
    self.write_bytes(index + RESET_COUNTER_REG, bytes([1]))


# test
if __name__ == '__main__':
  import board
  import busio

  m5e = M58Encoder(busio.I2C(board.SCL, board.SDA))

  print(f"version: {m5e.get_firmware_version()} - 0x{m5e.get_address():02x}")

  for i in range(8):
    m5e.set_encoder_value(i, 1)
    m5e.set_led_color_hsv(i, i * 0.125, 1, 1)

  c = 0
  while True:
    c = c + 1
    for i in range(8):
      b=m5e.is_button_down(i)
      r=m5e.get_encoder_value(i)
      if m5e.is_button_down(i):
        m5e.set_led_color_int(i, 0xffffff)
      else:
        m5e.set_led_color_hsv(i, i * 0.125, 1, (m5e.get_encoder_value(i) % 127) / 127)

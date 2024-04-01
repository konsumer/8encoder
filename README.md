# 8encoder

Various libs for working with [an i2c 8 rotary-encoder thing](https://docs.m5stack.com/en/unit/8Encoder) on a pi.

- [pigpio-8encoder](pigpio-8encoder) - single-header C library for use with [pigpio](https://abyz.me.uk/rpi/pigpio)
- [puredata-pi8encoder](puredata-pi8encoder) - Puredata extension that use pigpio
- [M58Encoder.py](M58Encoder.py) - Python library for use in circuitpython/adafruit-blinka

Make sure to add this to your `/boot/firmware/config.txt`:

```
# dtparam=i2c_arm=on,i2c_arm_baudrate=50000
# dtparam=i2c_arm=on,i2c_arm_baudrate=100000
# dtparam=i2c_arm=on,i2c_arm_baudrate=400000
dtparam=i2c_arm=on,i2c_arm_baudrate=1000000
```

## Not Complete

- [wiringpi-8encoder](wiringpi-8encoder) - [wirinpi](https://github.com/WiringPi/WiringPi) single-header C library

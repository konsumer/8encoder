# 8encoder

Various libs for working with [an i2c 8 rotary-encoder thing](https://docs.m5stack.com/en/unit/8Encoder) on a pi.

- [linux-8encoder](linux-8encoder) - C lib. Just uses regular linux stuff.
- [puredata-pi8encoder](puredata-pi8encoder) - Puredata extension that use linux-8encoder
- [M58Encoder.py](M58Encoder.py) - Python library for use in circuitpython/adafruit-blinka

Make sure to add this to your `/boot/firmware/config.txt`:

```
# dtparam=i2c_arm=on,i2c_arm_baudrate=50000
# dtparam=i2c_arm=on,i2c_arm_baudrate=100000
# dtparam=i2c_arm=on,i2c_arm_baudrate=400000
dtparam=i2c_arm=on,i2c_arm_baudrate=1000000
```

## TODO

- Have a look [here](https://github.com/giuliomoro/OSC2OLED4Bela) for a pretty complete i2c OLED driver. This idea should work for all 3 (OLED, 4encoder, 8encoder)

## Not Complete

- [wiringpi-8encoder](wiringpi-8encoder) - [wirinpi](https://github.com/WiringPi/WiringPi) single-header C library. Having a few issues with different message-formats (see [#212](https://github.com/WiringPi/WiringPi/issues/212)) They are wokring on adding mroe, so this might be a nice way to do it, later.
- [pigpio-8encoder](pigpio-8encoder) - single-header C library for use with [pigpio](https://abyz.me.uk/rpi/pigpio). Seems a bit slow, and drops some messages the way I am doing it, but got all the message-formats working. I may come back to this later.

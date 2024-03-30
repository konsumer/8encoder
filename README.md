# 8encoder

Various libs for working with [an i2c 8 rotary-encoder thing](https://docs.m5stack.com/en/unit/8Encoder) on a pi.

- [wiringpi-8encoder](wiringpi-8encoder) - [wirinpi](https://github.com/WiringPi/WiringPi) single-header C library
- [M58Encoder.py](M58Encoder.py) - Python library for use in circuitpython/adafruit-blinka


You will need to install wiringPi. I did this:

```
sudo apt install -y build-essential git cmake libasound2-dev libjack-dev
wget https://github.com/WiringPi/WiringPi/releases/download/3.1/wiringpi_3.1_arm64.deb
sudo apt install -y ./wiringpi_3.1_arm64.deb
```
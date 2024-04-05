> [!WARNING]  
> I stopped workign on this due to some issues with different byte-formats (see [#212](https://github.com/WiringPi/WiringPi/issues/212)) They are working on adding more, so this might be a nice way to do it, later.


This is a single-header C library for using wirinpi](https://github.com/WiringPi/WiringPi).

I stopped wokring on it because I could not get i2c-write working, please use [pigpio version](../pigpio-8encoder)


You will need wiringPi, which I installed like this:

```
wget https://github.com/WiringPi/WiringPi/releases/download/3.2/wiringpi_3.2_arm64.deb
apt install -y ./wiringpi_3.2_arm64.deb
```
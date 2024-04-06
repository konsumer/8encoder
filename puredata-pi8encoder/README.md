You will need pigpio & puredata headers, which I installed like this:

```
sudo apt install libi2c-dev puredata-dev
```

Also check out [pdpigpio](https://github.com/nullpainter/pdpigpio), as it uses similar ideas, for GPIO/mcp3008/mcp23s17 and [puredata-ssd1306](https://github.com/konsumer/ssd1306/tree/main/puredata-ssd1306) for SSD1306 OLEDs.


```
params: none

inlet 1:
  bang - get values
  rotary X Y - set value of X rotary to Y 
  rgb X R G B - set color of X LED (RGB range is 0-255)
  hsv X H S V - set color of X LED (HSV range is float 0-1)

outlet 1:
  R1 R2 R3 R4 R5 R6 R7 R8 | B1 B2 B3 B4 B5 B6 B7 B8 | SW
```

All values are output as a single message.

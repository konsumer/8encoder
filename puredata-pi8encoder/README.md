You will need pigpio & puredata headers, which I installed like this:

```
sudo apt install libi2c-dev puredata-dev
```

Also check out [pdpigpio](https://github.com/nullpainter/pdpigpio), as it uses similar ideas, for GPIO/mcp3008/mcp23s17 and [puredata-ssd1306](https://github.com/konsumer/ssd1306/tree/main/puredata-ssd1306) for SSD1306 OLEDs.


```
params: none

inlet 1:
  bang - get all values (as individual messages)
  rotary X Y - set value of X rotary to Y 
  rgb X R G B - set color of X LED (RGB range is 0-255)
  hsv X H S V - set color of X LED (HSV range is float 0-1)

outlet 1:
  rotary X Y - the value of rotary X changed to Y
  button X Y - the value of button X changed to Y (0/1)
  switch Y   - the value of toggle switch changed to Y (0/1)
```
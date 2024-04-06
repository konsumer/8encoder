#include <stdlib.h>
#include "linux-8encoder.h"
#include "m_pd.h"

static t_class* pi8encoder_class;

typedef struct _pi8encoder {
  t_object x_obj;
  t_outlet* x_out;
  int i2c;
} t_pi8encoder;

static void pi8encoder_free(t_pi8encoder* x) {
  post("[pi8encoder] terminated");
}

static void* pi8encoder_new() {
  t_pi8encoder* x = (t_pi8encoder*)pd_new(pi8encoder_class);
  x->x_out = outlet_new(&x->x_obj, gensym("list"));
  x->i2c = open("/dev/i2c-1", O_RDWR);
  if (x->i2c < 0 || ioctl(x->i2c, I2C_SLAVE, ENCODER_ADDR) < 0) {
    post("[pi8encoder] init error.");
    // TODO: how to return error here, so it fails?
    return NULL;
  }
  post("[pi8encoder] initialised.");
  return x;
}

static void pi8encoder_bang(t_pi8encoder* x) {
  t_atom msg[17] = {0};
  int v = linux_8encoder_switch(x->i2c);
  SETFLOAT(&msg[16], v);
  for (int i = 0; i < 8; i++) {
    v = linux_8encoder_get_counter(x->i2c, i);
    SETFLOAT(&msg[i], v);
    v = linux_8encoder_button_down(x->i2c, i + 8) ? 1 : 0;
    SETFLOAT(&msg[i + 8], v);
  }
  outlet_list(x->x_out, 0, 17, msg);
}

static void pi8encoder_handle_rgb(t_pi8encoder* x, t_floatarg i, t_floatarg r, t_floatarg g, t_floatarg b) {
  ColorRGB c = {0};
  c.r = r;
  c.g = g;
  c.b = b;
  linux_8encoder_set_led_rgb(x->i2c, i, c);
}

static void pi8encoder_handle_hsv(t_pi8encoder* x, t_floatarg i, t_floatarg h, t_floatarg s, t_floatarg v) {
  ColorHSV c = {0};
  c.h = h;
  c.s = s;
  c.v = v;
  linux_8encoder_set_led_hsv(x->i2c, i, c);
}

static void pi8encoder_handle_rotary(t_pi8encoder* x, t_floatarg i, t_floatarg value) {
  linux_8encoder_set_counter(x->i2c, i, value);
}

void pi8encoder_setup(void) {
  pi8encoder_class = class_new(gensym("pi8encoder"),
                               (t_newmethod)pi8encoder_new,
                               (t_method)pi8encoder_free,
                               sizeof(t_pi8encoder),
                               CLASS_DEFAULT,
                               0);

  class_addmethod(
      pi8encoder_class,
      (t_method)pi8encoder_handle_rgb,
      gensym("rgb"),
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      0);
  class_addmethod(
      pi8encoder_class,
      (t_method)pi8encoder_handle_hsv,
      gensym("hsv"),
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      0);
  class_addmethod(
      pi8encoder_class,
      (t_method)pi8encoder_handle_rotary,
      gensym("rotary"),
      A_DEFFLOAT,
      A_DEFFLOAT,
      0);

  class_addbang(pi8encoder_class, pi8encoder_bang);
}
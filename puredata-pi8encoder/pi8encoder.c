#include <stdlib.h>
#include "linux-8encoder.h"
#include "m_pd.h"

static t_class* pi8encoder_class;

typedef struct _pi8encoder {
  t_object x_obj;
  t_outlet* x_out;
  int i2c;
  int tracking_buttons[8];
  int tracking_rotary[8];
  int tracking_switch;
  bool current_pass;
} t_pi8encoder;

static void pi8encoder_outputchanges(t_pi8encoder* x) {
  t_atom msg[3] = {};
  x->current_pass = !x->current_pass;
  int v = linux_8encoder_switch(x->i2c);
  if (x->tracking_switch != v) {
    SETSYMBOL(&msg[0], gensym("switch"));
    SETFLOAT(&msg[1], v);
    outlet_list(x->x_out, 0, 2, msg);
    x->tracking_switch = v;
  }
  for (int i = 0; i < 8; i++) {
    if (x->current_pass) {
      v = linux_8encoder_get_counter(x->i2c, i);
      if (x->tracking_rotary[i] != v) {
        SETSYMBOL(&msg[0], gensym("rotary"));
        SETFLOAT(&msg[1], i);
        SETFLOAT(&msg[2], v);
        outlet_list(x->x_out, 0, 3, msg);
        x->tracking_rotary[i] = v;
      }
    } else {
      v = linux_8encoder_button_down(x->i2c, i) ? 1 : 0;
      if (x->tracking_buttons[i] != v) {
        SETSYMBOL(&msg[0], gensym("button"));
        SETFLOAT(&msg[1], i);
        SETFLOAT(&msg[2], v);
        outlet_list(x->x_out, 0, 3, msg);
        x->tracking_buttons[i] = v;
      }
    }
  }
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

static void pi8encoder_free(t_pi8encoder* x) {
  post("[pi8encoder] terminated");
}

static void* pi8encoder_new() {
  t_pi8encoder* x = (t_pi8encoder*)pd_new(pi8encoder_class);
  x->i2c = open("/dev/i2c-1", O_RDWR);
  if (x->i2c < 0 || ioctl(x->i2c, I2C_SLAVE, ENCODER_ADDR) < 0) {
    post("[pi8encoder] init error.");
    // TODO: how to return error here, so it fails?
    return NULL;
  }
  x->x_out = outlet_new(&x->x_obj, gensym("list"));
  for (int i = 0; i < 8; i++) {
    x->tracking_buttons[i] = 0;
    x->tracking_rotary[i] = 0;
  }
  x->tracking_switch = 0;

  post("[pi8encoder] initialised.");
  return x;
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

  class_addbang(pi8encoder_class, pi8encoder_outputchanges);
}

#include "m_pd.h"
#include "pigpio-8encoder.h"

static t_class* pi8encoder_class;

typedef struct _pi8encoder {
  t_object x_obj;
  t_outlet* x_out;
  bool initialised;
  int i2c;
} t_pi8encoder;

static void pi8encoder_free(t_pi8encoder* x) {
  gpioTerminate();
  x->initialised = false;
  i2cClose(x->i2c);
  post("[pi8encoder] pigpio terminated");
}

static void pi8encoder_bang(t_pi8encoder* x) {
  if (!x->initialised) {
    if (geteuid() != 0) {
      pd_error(x, "[pi8encoder] root is required to use this external");
      exit(0);
    }

    // Use PWM for timing DMA transfers, as PCM is used for audio
    // gpioCfgClock(5, 0, 0);

    if (gpioInitialise() < 0) {
      pd_error(x, "[pi8encoder] Unable to initialise GPIO; ensure pi8encoderd isn't running");
      exit(0);
    }

    x->i2c = i2cOpen(1, ENCODER_ADDR, 0);

    post("[pi8encoder] pigpio initialised.");

    x->initialised = true;
  }

  t_atom values[17];
  SETFLOAT(&values[0], pigio_8encoder_get_switch_value(x->i2c));

  for (int i = 0; i < 8; i++) {
    SETFLOAT(&values[i + 1], pigio_8encoder_is_button_down(x->i2c, i) ? 1 : 0);
    SETFLOAT(&values[i + 8], pigio_8encoder_get_encoder_value(x->i2c, i));
  }

  // Output all values as a list
  outlet_list(x->x_out, 0, 17, values);
}

static void* pi8encoder_new() {
  t_pi8encoder* x = (t_pi8encoder*)pd_new(pi8encoder_class);
  x->initialised = false;
  x->x_out = outlet_new(&x->x_obj, gensym("list"));

  return x;
}

void pi8encoder_handle_rgb(t_pi_gpio* x, t_floatarg r, t_floatarg g, t_floatarg b) {
  post("RGB %f, %f, %f", r, g, b);
}

void pi8encoder_handle_hsv(t_pi_gpio* x, t_floatarg h, t_floatarg s, t_floatarg v) {
  post("HSV %f, %f, %f", h, s, v);
}

void pi8encoder_handle_rotary(t_pi_gpio* x, t_floatarg value) {
  post("Rotary %f", value);
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
      0);
  class_addmethod(
      pi8encoder_class,
      (t_method)pi8encoder_handle_hsv,
      gensym("hsv"),
      A_DEFFLOAT,
      A_DEFFLOAT,
      A_DEFFLOAT,
      0);
  class_addmethod(
      pi8encoder_class,
      (t_method)pi8encoder_handle_rotary,
      gensym("rotary"),
      A_DEFFLOAT,
      0);

  class_addbang(pi8encoder_class, pi8encoder_bang);
}
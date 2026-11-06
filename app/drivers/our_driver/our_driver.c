#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <errno.h>
#include <zephyr/drivers/gpio.h>

#include "led_control.h"

#define DT_DRV_COMPAT elv_led_control

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);  

struct our_driver_data {
    uint8_t state;
};

struct our_driver_config {
    struct gpio_dt_spec led;
};

static const struct our_driver_config our_driver_config_0 = {
    .led = GPIO_DT_SPEC_GET(DT_ALIAS(app_led), gpios),
};

static int our_driver_set_led(const struct device *dev, bool on)
{
    const struct our_driver_config *config = dev->config;

    return gpio_pin_set_dt(&config->led, on ? 1 : 0);
}

static int our_driver_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val) {

    ARG_UNUSED(chan);
    val->val1 = 0;
    val->val2 = 0;

    int ret = our_driver_set_led(dev, false); /* LED off */

    if (ret != -1) {
        LOG_INF("LED state: OFF");
    } else {
        LOG_ERR("Failed to set LED state: %d", ret);
    }

    return ret;
}

static int our_driver_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan) {

    ARG_UNUSED(chan);

    int ret = our_driver_set_led(dev, true);  /* LED on */
    
    if(ret != -1) {
        LOG_INF("LED state: ON");
    } else {
        LOG_ERR("Failed to set LED state: %d", ret);
    }

    return ret;
}

static DEVICE_API(sensor, our_driver_api) = {
    
    .channel_get  = our_driver_channel_get,
    .sample_fetch = our_driver_sample_fetch,

};

static int init(const struct device *dev)
{
    const struct our_driver_config *cfg = dev->config;
    
    if (!gpio_is_ready_dt(&cfg->led)) {
        return -ENODEV;
    }

    return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

static int our_driver_add(const struct device *dev)
{
    struct our_driver_data *data = dev->data;

    if (data->state != 0) {
        return -EALREADY;
    }

    return data->state = 1;
}

static int our_driver_take(const struct device *dev)
{
    struct our_driver_data *data = dev->data;

    if (data->state != 1) {
        return -EALREADY;
    }

    return data->state = 0;
}

static DEVICE_API(elv_led_control, elv_led_control_driver_api) = {
    .add = our_driver_add,
    .take = our_driver_take,
};

static struct our_driver_data our_driver_data_0;

DEVICE_DT_INST_DEFINE(0, init, NULL, &our_driver_data_0, &our_driver_config_0, POST_KERNEL, 80, &elv_led_control_driver_api);
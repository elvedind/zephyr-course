#ifndef LED_CONTROL_H__
#define LED_CONTROL_H__

#include <stdint.h>
#include <zephyr/device.h>

typedef int (*elv_led_control_add_t)(const struct device *dev);
typedef int (*elv_led_control_take_t)(const struct device *dev);

__subsystem struct elv_led_control_driver_api {
    elv_led_control_add_t add;
    elv_led_control_take_t take;
};

static inline int elv_led_control_add(const struct device *dev)
{
    return DEVICE_API_GET(elv_led_control, dev)->add(dev);
}

static inline int elv_led_control_take(const struct device *dev)
{
    return DEVICE_API_GET(elv_led_control, dev)->take(dev);
}


#endif /* LED_CONTROL_H__ */
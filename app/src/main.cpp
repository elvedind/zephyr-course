#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

#include "led_control.h"

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "app-led" alias. */
#define LED_NODE DT_ALIAS(app_led)

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/*
namespace{
    void test() {
        const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(led_driver0));
        struct sensor_value val;
        int ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("Channel ret %d", ret);
    }

}
*/
 const struct device* dev = DEVICE_DT_GET(DT_NODELABEL(led_driver0));

 int led_state = 0; // Global variable to hold the LED state

int main(void)
{

    if (!device_is_ready(dev)) {
        return -ENODEV;
    }

   
    while (1) {
        
        if (led_state == 0) {
            led_state = elv_led_control_add(dev); // calls channel_get: LED on
        }

        LOG_INF("LED binary state: %d", led_state);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

       if (led_state == 1) {
            led_state = elv_led_control_take(dev); // calls channel_get: LED on
        }

        LOG_INF("LED binary state: %d", led_state);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}

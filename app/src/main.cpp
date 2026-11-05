#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

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

int main(void)
{

    if (!device_is_ready(dev)) {
        return -ENODEV;
    }

   
    while (1) {
        
        int ret = sensor_sample_fetch(dev);

        if (ret < 0) {
            LOG_ERR("sensor_sample_fetch failed: %d", ret);
            return ret;
        }
         k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        struct sensor_value value;
        ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &value);

        if (ret < 0) {                       // calls channel_get: LED off
            LOG_ERR("sensor_channel_get failed: %d", ret);
            return ret;
        }

        //LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}

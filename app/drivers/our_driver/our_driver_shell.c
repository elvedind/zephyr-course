#include <zephyr/shell/shell.h>
#include <errno.h>
#include <zephyr/drivers/sensor.h>


static const struct device* dev = DEVICE_DT_GET(DT_NODELABEL(led_driver0));


static int cmd_get_handler(const struct shell *sh, size_t argc, char **argv) {
    
    if (!device_is_ready(dev)) {
       return -ENODEV;
    }

    struct sensor_value value;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &value);

        if (ret < 0) {
            shell_error(sh, "sensor_channel_get failed: %d", ret);
            return ret;
        }

    return 0;
}

static int cmd_fetch_handler(const struct shell *sh, size_t argc, char **argv) {
       
    if (!device_is_ready(dev)) {
       return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);

        if (ret < 0) {
            shell_error(sh, "sensor_sample_fetch failed: %d", ret);
            return ret;
        }

    return 0;
}

static int cmd_info_handler(const struct shell *sh, size_t argc, char **argv) {
       
    shell_print(sh,"Device Name: %s", dev->name);
    shell_print(sh,"Device Ready State: %d", dev->state->initialized);
  
    return 0;
}

       

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
    SHELL_CMD(get, NULL, "LED OFF command", cmd_get_handler),
    SHELL_CMD(fetch, NULL, "LED ON command", cmd_fetch_handler),
    SHELL_CMD(info, NULL, "Device info command", cmd_info_handler),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(our_driver, &our_driver_subcmd, "Our driver commands", NULL);
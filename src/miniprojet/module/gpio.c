#include <linux/kernel.h> /* needed for debugging */
#include <linux/gpio.h>
#include "gpio.h"

static int led_status = 0;

int init_gpio()
{
    int err;
    pr_info("Initializing GPIO\n");
    err = gpio_request(GPIO_LED, "LED");
    if (err == 0)
    {
        err = gpio_direction_output(GPIO_LED, 0);
    }else{
        pr_err("GPIO not initialized %d ", err);
    }
    
    return err;
}

int toggle_led()
{
    led_status = 1 - led_status;
    gpio_set_value(GPIO_LED, led_status);
    return 0;
}

void exit_gpio()
{
    gpio_free(GPIO_LED);
}
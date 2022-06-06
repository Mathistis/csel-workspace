#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/init.h> /* needed for macros */

#include "gpio.h"
#include "timer_controller.h"

#define NEXT_JIFFIE_FROM_FREQ(freq) jiffies + (HZ / freq)
#define BASE_FREQ 2

static struct timer_list myTimer;
static int (*handler)(void);

void timer_handler(struct timer_list *trigged_timer)
{
    int err;
    int freq = (*handler)();
    if (freq < 0) freq = BASE_FREQ;


    err = mod_timer(trigged_timer, NEXT_JIFFIE_FROM_FREQ(freq));

    if (err < 0)
    {
        pr_info("mod_timer err = %d", err);
    }

    return;
}

int init_my_timer(int (*on_timer_rised)(void))
{
    int err;
    handler = on_timer_rised;
    pr_info("myTimer is initializing\n");
    timer_setup(&myTimer, timer_handler, 0);
    err = mod_timer(&myTimer, NEXT_JIFFIE_FROM_FREQ(BASE_FREQ));
    if (err < 0)
    {
        pr_info("mod_timer err = %d", err);
    }
    pr_info("myTimer is init !\n");
    return err;
}

int exit_timer()
{
    return del_timer(&myTimer);
}
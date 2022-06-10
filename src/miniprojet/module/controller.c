#include <linux/kernel.h> /* needed for debugging */


#include "controller.h"
#include "timer_controller.h"
#include "temp_controller.h"
#include "gpio.h"

int on_timer_rised(void);

static int* current_mode;
static int* freq;

int init_controller(int* is_manu, int* freq_manual){
    int err;
    err = init_gpio();
    if(err == 0) err = init_my_timer(&on_timer_rised);
    if(err == 0) init_temp();
    current_mode = is_manu;
    freq = freq_manual;
    return err;
}

int mode_auto(void){
    int temp = 0;
    int err = 0;
    err = get_temp(&temp);
    if (err < 0)
    {
        pr_info("get_temp err = %d", err);
        return err;
    }else{
        temp /= 1000;
        // pr_info("Got %d°", temp);
        toggle_led();
    }
    if(temp < 35 ){
        *freq = 2;
    }
    if(temp < 40){
        *freq = 5;
    }
    if(temp < 45){
        *freq = 10;
    }
    else{
        *freq = 20;
    }
    return *freq;
}

int mode_manual(void){
    toggle_led();
    return *freq;
}


int on_timer_rised(){
    if(*current_mode == MODE_AUTO){
        return mode_auto();
    }else if(*current_mode == MODE_MANUAL){
        return mode_manual();
    }else{
        pr_err("UNKNOW mode");
    }
    return -1;
}

void set_mode(int mode){
    *current_mode = mode;

}


int stop_controller(){
    int err;
    err = exit_timer();
    if( err < 0 ) pr_err("Error removing timer: %d", err);
    exit_gpio();
    return 0;
}
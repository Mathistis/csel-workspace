#include <linux/kernel.h> /* needed for debugging */
#include <linux/thermal.h>
#include "temp_controller.h"


static struct thermal_zone_device* myThermalZone;

void init_temp(void){
    myThermalZone = thermal_zone_get_zone_by_name(THERMAL_ZONE);
}

int get_temp(int * temp){
    int err;
    err = thermal_zone_get_temp(myThermalZone, temp);
    if(err < 0) pr_err("Failed to get temp");
    return err;
}


#include "oled.h"
#include "ssd1306.h"
#include <stdio.h>
#define TEMP_FORMAT "Temp: %3d'C"
#define FREQ_FORMAT "Freq: %3dHz"
#define MODE_FORMAT "Mode: "
#define MODE_AUTO "auto"
#define MODE_MANU "manu"
#define AUTO_VALUE 0
#define MANU_VALUE 1
#define BUFF_SZ 30

void init_oled()
{
    ssd1306_init();
    ssd1306_set_position(0, 0);
    ssd1306_puts("CSEL1 - SP.22");
    ssd1306_set_position(0, 1);
    ssd1306_puts("  Miniproj - SW");
    ssd1306_set_position(0, 2);
    ssd1306_puts("--------------");
}

void update_temp(int temp)
{
    ssd1306_set_position(0, 3);
    char buf[BUFF_SZ] = "";
    snprintf(buf, BUFF_SZ, TEMP_FORMAT, temp);
    ssd1306_puts(buf);
}

void update_freq(int freq)
{
    ssd1306_set_position(0, 4);
    char buf[BUFF_SZ] = "";
    snprintf(buf, BUFF_SZ, FREQ_FORMAT, freq);
    ssd1306_puts(buf);
}

void update_mode(int mode)
{
    ssd1306_set_position(0, 5);
    switch (mode){
        case AUTO_VALUE:
            ssd1306_puts(MODE_FORMAT MODE_AUTO);
            break;
        case MANU_VALUE:
            ssd1306_puts(MODE_FORMAT MODE_MANU);
            break;
        default:
            break;

    }
}
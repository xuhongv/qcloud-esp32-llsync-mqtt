/*
 * @Author: your name
 * @Date: 2021-06-28 23:40:52
 * @LastEditTime: 2021-07-04 22:22:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \esp-idf-v4.2-s2\qcloud-iot-explorer-BLE-sdk-embedded-demo\qcloud-llsync-config-net-esp32\components\aithinker_board\src\led.c
 */
// Copyright 2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include "esp_log.h"
#include "esp_attr.h"
#include "led_strip.h"
#include "driver/gpio.h"


#define GPIO_OUTPUT_IO 2

#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_OUTPUT_IO))

void led_init()
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_INPUT_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
}

void led_set_high()
{
    gpio_set_level(GPIO_OUTPUT_IO, 1);
}

void led_set_low()
{
    gpio_set_level(GPIO_OUTPUT_IO, 0);
}

int led_get_level()
{
    return gpio_get_level(GPIO_OUTPUT_IO);
}

void led_set_level(bool isOpen)
{
    gpio_set_level(GPIO_OUTPUT_IO, isOpen);
}
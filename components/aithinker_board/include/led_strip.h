/*
 * @Author: xuhongv
 * @Date: 2021-06-28 23:40:52
 * @LastEditTime: 2021-07-04 22:21:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \esp-idf-v4.2-s2\qcloud-iot-explorer-BLE-sdk-embedded-demo\qcloud-llsync-config-net-esp32\components\aithinker_board\include\led_strip.h
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_err.h"

    void led_init();

    void led_set_high();

    void led_set_low();

    void led_set_level(bool isOpen);

    int led_get_level();

#ifdef __cplusplus
}
#endif

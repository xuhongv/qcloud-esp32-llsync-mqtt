#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_ota_ops.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "ble_qiot_export.h"
#include "qcloud_wifi_config.h"
#include "driver/rmt.h"
#include "button.h"

#define EXAMPLE_CHASE_SPEED_MS (10)
extern void wifi_config_sample_main(void *pvParameters);
extern void inter_llsync_combo(void *pvParameters);
static const char *TAG = "llsync config net";
#define EXAMPLE_ESP_MAXIMUM_RETRY 5

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static int s_retry_num = 0;
bool sg_gotip_flag = false;

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    wifi_config_t cfg;
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
        {
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        esp_wifi_get_config(WIFI_IF_STA, &cfg);
        //esp_wifi_connect();
        ble_event_report_wifi_connect(BLE_WIFI_MODE_STA, BLE_WIFI_STATE_OTHER, (uint8_t)strlen((const char *)cfg.sta.ssid), (const char *)cfg.sta.ssid);
        ESP_LOGI(TAG, "connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        sg_gotip_flag = true;
        xTaskCreate(wifi_config_sample_main, "wifi_config_sample_main", 1024 * 8, NULL, 7, NULL); //
    }

    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        wifi_config_t wifi_config;

        if (esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config) == ESP_OK)
        {

            if (strlen((const char *)wifi_config.sta.ssid) == 0)
            {
                ESP_LOGI(TAG, "esp_wifi_get_config ok , but null config ");
                xTaskCreate(inter_llsync_combo, "inter_llsync_combo", 1024 * 6, NULL, 7, NULL); //
            }

            else
            {
                ESP_LOGI(TAG, "esp_wifi_get_config= %s %s", wifi_config.sta.ssid, wifi_config.sta.password);
                esp_wifi_connect();
            }
        }
        else
        {
            ESP_LOGI(TAG, "esp_wifi_get_config return fail ");
            //wifi_config_sample_main(true);
        }
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");
}

ble_qiot_ret_status_t ble_combo_wifi_mode_set(BLE_WIFI_MODE mode)
{
    // 当前只支持sta模式。
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    return ble_event_report_wifi_mode(0);
}

ble_qiot_ret_status_t ble_combo_wifi_info_set(const char *ssid, uint8_t ssid_len, const char *passwd, uint8_t passwd_len)
{

    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false},
        },
    };
    memset(wifi_config.sta.ssid, 0, sizeof(wifi_config.sta.ssid));
    memset(wifi_config.sta.password, 0, sizeof(wifi_config.sta.password));
    memcpy(wifi_config.sta.ssid, ssid, ssid_len);
    memcpy(wifi_config.sta.password, passwd, passwd_len);
    ESP_LOGI(TAG, "wifi info : %s , %s", wifi_config.sta.ssid, wifi_config.sta.password);
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    set_bt_combo_config_state(WIFI_CONFIG_SUCCESS);

    return ble_event_report_wifi_info(0);
}

ble_qiot_ret_status_t ble_combo_wifi_connect()
{
    ESP_LOGI(TAG, "wifi connect");
    ESP_ERROR_CHECK(esp_wifi_connect());
    return 0;
}

#define BUTTON_GPIO 0

//短按函数
static void ButtonShortPressCallBack(void *arg)
{
    ESP_LOGI(TAG, "ButtonShortPressCallBack  esp_get_free_heap_size(): %d ", esp_get_free_heap_size());
}
//长按函数
static void ButtonLongPressCallBack(void *arg)
{
    ESP_LOGI(TAG, "ButtonLongPressCallBack  esp_get_free_heap_size(): %d ", esp_get_free_heap_size());
    esp_wifi_restore();
    vTaskDelay(2000 / portTICK_RATE_MS);
    esp_restart();
}

/**
 * @description: 按键驱动
 * @param {type} 
 * @return: 
 */
void TaskButton(void *pvParameters)
{
    //定义一个 gpio 下降沿触发
    button_handle_t btn_handle = button_dev_init(BUTTON_GPIO, 1, BUTTON_ACTIVE_LOW);
    // 50ms按钮短按
    // BUTTON_PUSH_CB 表示按下就触发回调函数，如果设置了长按，这个依然会同时触发！
    // BUTTON_RELEASE_CB 表示释放才回调，如果设置了长按，这个依然会同时触发！
    // BUTTON_TAP_CB 此选项释放才回调，如果设置了长按，这个不会同时触发！
    button_dev_add_tap_cb(BUTTON_PUSH_CB, ButtonShortPressCallBack, "TAP", 50 / portTICK_PERIOD_MS, btn_handle);
    // 设置长按 2s后触发
    button_dev_add_press_cb(0, ButtonLongPressCallBack, NULL, 2000 / portTICK_PERIOD_MS, btn_handle);
    vTaskDelete(NULL);
}

void app_main()
{
    nvs_flash_init();

    wifi_init_sta();

    xTaskCreate(TaskButton, "TaskButton", 1024 * 2, NULL, 5, NULL); //
}

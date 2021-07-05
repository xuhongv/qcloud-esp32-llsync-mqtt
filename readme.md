## 一、前言
本工程用做ESP32的示例工程，基于安信可科技`NodeMCU-ESP32`开发板上演示了终端设备通过`LLSync SDK`和`Explorer SDK`进行配网以及和云端通信。


## 二、源码目录说明

```
├─qcloud-esp32-llsync-mqtt         应用目录
│           
│  ├─components         模块目录
│  │  ├─aithinker_board    安信可NodeMCU ESP32开发板LED驱动
│  │  ├─button    按键驱动
│  │  ├─qcloud_iot_c_sdk  腾讯物联开发平台的 Explorer SDK 
│  │  ├─qcloud_llsync     腾讯物联开发平台的 LLSync SDK 
│  │  │  │ 
│  │  ├─main              应用程序入口
│  │  │  ├─app_main.c     设备程序入口文件
│  │  │  │ 
│  │  │  ├─wifi_config_sample.c  设备控制业务代码
│  │  │  │ 
```

## 三、编译指导
1. 安装`ESP-IDF`，请参考[官网文档](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#step-2-get-esp-idf)。
```c
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
```
本示例使用`SDK`的分支是`release/v4.2`, `commit`是`2532ddd9f447f6fab02bc2d1654534a7621e033a`

或者直接下载好完整版压缩包：[https://axk.coding.net/s/c3ce514c-808f-42fe-bc04-1c15ba823e45](https://axk.coding.net/s/c3ce514c-808f-42fe-bc04-1c15ba823e45)

2. 拷贝`qcloud-esp32-llsync-mqtt`文件夹到`ESP-IDF`目录下。
```c
cp -r qcloud-esp32-llsync-mqtt $IDF_PATH
cd $IDF_PATH/qcloud-esp32-llsync-mqtt
```

3. 登陆[物联网开发平台](https://cloud.tencent.com/product/iotexplorer), 创建设备，拿到三元组信息。

  使用新设备的三元信息替换`qcloud-esp32-llsync-mqtt/components/qcloud_iot_c_sdk/platform/HAL_Device_freertos.c`中宏定义的设备信息，编译并烧录到开发板。
```c
/* product Id  */
static char sg_product_id[MAX_SIZE_OF_PRODUCT_ID + 1] = "I8YSNLJF5T";
/* device name */
static char sg_device_name[MAX_SIZE_OF_DEVICE_NAME + 1] = "ESP32_D1";
/* device secret of PSK device */
static char sg_device_secret[MAX_SIZE_OF_DEVICE_SECRET + 1] = "4mFTgmh8VFoqEYLdYLOOBA==";
```

4. 在物联网开发平台修改设备配网方式为标准BLE辅助配网。

   ![](https://main.qcloudimg.com/raw/68cbcfc52dc3251a997d710efd5758bc.png)

5. 微信搜索腾讯连连小程序，进行配网操作。

   ![](https://main.qcloudimg.com/raw/e7a2109811f04f17c6e4dafde7e17086.png)

   ![](https://main.qcloudimg.com/raw/faf9dba15292241e11f503e0695e45e7.png)

   ![](https://main.qcloudimg.com/raw/8eadd13ed31b46131535b590784612fe.png)

6. 配网成功后，设备端日志如下。

   ```c
   I (962) llsync config net: wifi_init_sta finished.
   I (962) llsync config net: esp_wifi_get_config ok , but null config 
   I (962) gpio: GPIO[0]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:3 
   ERR|1970-01-01 00:15:53|qcloud_wifi_config_log_handle.c|init_dev_log_queue(55): init dev log queue
   
   INF|1970-01-01 00:15:53|HAL_Wifi_api.c|HAL_Wifi_read_err_log(42): HAL_Wifi_read_err_log
   
   INF|1970-01-01 00:15:53|qcloud_wifi_config_error_handle.c|check_err_log(91): invalid magic code: 0x3ffc8be0
   
   I (1002) BTDM_INIT: BT controller compile version [a6525b7]
   I (1352) LLSYNC: create attribute table successfully, the number handle = 6
   
   I (1352) LLSYNC: ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT
   I (1352) broadcast: 02 e8 db 84 3b 3e c2 49 38 59 53 4e 4c 4a 46 35 
   I (1362) broadcast: 54 
   I (1362) adv: 02 01 06 03 03 f0 ff 14 ff e7 fe 02 e8 db 84 3b 
   I (1372) adv: 3e c2 49 38 59 53 4e 4c 4a 46 35 54 02 09 6c 
   I (1372) LLSYNC: start advertising
   qiot info: start wait advertising
   
   I (1382) LLSYNC: SERVICE_START_EVT, status 0, service_handle 40
   I (1382) LLSYNC: Stop adv successfully
   
   I (1392) LLSYNC: ESP_GAP_BLE_ADV_DATA_RAW_SET_COMPLETE_EVT
   I (1392) LLSYNC: advertising start successfully
   I (21232) LLSYNC: 52 b6 c9 9e e8 0a 
   I (21352) LLSYNC: ESP_GATTS_MTU_EVT, MTU 517
   qiot err(../components/qcloud_llsync/src/core/ble_qiot_llsync_event.c|77): upload msg negate, device not connected
   
   I (21702) LLSYNC: update connection params status = 0, min_int = 16, max_int = 32,conn_int = 24,latency = 0, timeout = 400
   I (22252) post data: 08 00 0c 02 80 80 08 45 53 50 33 32 5f 44 31 
   qiot info: mtu setting result: 00
   
   I (25972) post data: e0 00 01 00 
   I (26362) llsync config net: wifi info : XuHongIoT , xuhong12345678
   I (26382) post data: e1 00 01 00 
   I (26782) llsync config net: wifi connect
   I (27022) wifi:new:<1,0>, old:<1,0>, ap:<255,255>, sta:<1,0>, prof:1
   I (27832) wifi:state: init -> auth (b0)
   I (27852) wifi:state: auth -> assoc (0)
   I (27912) wifi:state: assoc -> run (10)
   I (27932) wifi:connected with XuHongIoT, aid = 2, channel 1, BW20, bssid = 9c:9d:7e:40:e8:10
   I (27932) wifi:security: WPA2-PSK, phy: bgn, rssi: -27
   I (27942) wifi:pm start, type: 1
   
   I (28032) wifi:AP's beacon interval = 102400 us, DTIM period = 1
   I (29262) esp_netif_handlers: sta ip: 192.168.31.89, mask: 255.255.255.0, gw: 192.168.31.1
   I (29262) llsync config net: got ip:192.168.31.89
   I (29342) post data: e2 00 0d 01 00 00 09 58 75 48 6f 6e 67 49 6f 54 
   INF|1970-01-01 00:16:21|HAL_TCP_lwip.c|HAL_TCP_Connect(93): connected with TCP server: I8YSNLJF5T.iotcloud.tencentdevices.com:1883
   
   INF|1970-01-01 00:16:21|mqtt_client.c|IOT_MQTT_Construct(125): mqtt connect with id: nj3bE success
   INF|1970-01-01 00:16:21|qcloud_wifi_config_device_bind.c|_setup_mqtt_connect(332): Cloud Device Construct Success
   INF|1970-01-01 00:16:23|qcloud_wifi_config_device_bind.c|_send_token_wait_reply(221): wait for token data...
   INF|1970-01-01 00:16:23|qcloud_wifi_config_device_bind.c|_mqtt_event_handler(68): publish success, packet-id=26846
   INF|1970-01-01 00:16:23|qcloud_wifi_config_device_bind.c|_on_message_callback(101): recv msg topic: $thing/down/service/I8YSNLJF5T/ESP32_D1
   INF|1970-01-01 00:16:23|qcloud_wifi_config_device_bind.c|_on_message_callback(112): msg payload: {"method":"app_bind_token_reply","clientToken":"ESP32_D1-983694","code":0,"status":"success"}
   INF|1970-01-01 00:16:24|qcloud_wifi_config_device_bind.c|_send_token_wait_reply(249): wait for token sending result...
   INF|1970-01-01 00:16:25|mqtt_client_connect.c|qcloud_iot_mqtt_disconnect(468): mqtt disconnect!
   INF|1970-01-01 00:16:25|mqtt_client.c|IOT_MQTT_Destroy(189): mqtt release!
   INF|1970-01-01 00:16:30|qcloud_wifi_config_device_bind.c|qiot_device_bind(398): WIFI_MQTT_CONNECT_SUCCESS
   ERR|1970-01-01 00:16:30|qcloud_wifi_config_log_handle.c|push_dev_log(98): push dev log 990703|qiot_device_bind(399): WIFI_MQTT_CONNECT_SUCCESS
   ...
   
   ```
## 四、常见问题

1. 如何重置设备进去配网模式？

> 本程序已经适配了在 安信可科技`NodeMCU-ESP32`开发板上，长按开发板按键IO0在3秒以上，直到板载LED出现闪烁，即进去蓝牙配网`LLSync`模式。
>
> 相关代码在 app_main.c 文件。

2. 在哪里处理服务器下发的业务逻辑？

> 本程序已经适配了在 安信可科技`NodeMCU-ESP32`开发板上，仅可控制板载的LED灯的开关功能。
>
> 相关代码在 qcloud-llsync-config-net-esp32\main\wifi_config_sample.c 文件的 property_control_handle() 里面。

3. 在哪里处理服务器上报状态？

> 本程序已经适配了在 安信可科技`NodeMCU-ESP32`开发板上，可以参考下面代码。
>
> 相关代码在 qcloud-llsync-config-net-esp32\main\wifi_config_sample.c 文件的 property_report 里面。



#ifndef _WIFI_AP_H
#define _WIFI_AP_H


#ifdef __cplusplus
extern "C" {
#endif
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"
#include "esp_spi_flash.h"
#include "esp_task_wdt.h"

#include <sys/unistd.h>
#include <sys/stat.h>

#include "esp_wifi.h"
#include <lwip/netdb.h>
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#define PORT 8888

void wifi_init_softap();
void init_tcp_server();

void spi_fs_init(void);
void spi_fs_deinit(void);

#ifdef __cplusplus
 }
#endif

#endif /* OTA_OPS_H */

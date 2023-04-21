/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "common.h"

void app_main()
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

     wifi_init_softap();
    //spi_fs_init();
    init_tcp_server();
    while (1) {
        sleep(3);
        esp_task_wdt_reset();
    }
}

#include "common.h"

static const char *TAG = "tcp_server_task";


static void tcp_server_task(void *pvParameters)
{
    char addr_str[128];
    int addr_family;
    int ip_protocol;
    struct sockaddr_in sourceAddr;
    uint addrLen = sizeof(sourceAddr);
    char data[4096] = {0};    //读取网络数据
    char file_msg[1024];//file 文件名 文件大小
    char file_name[1024];
    int file_size ;
    FILE* fp=NULL;

    struct sockaddr_in destAddr;
    destAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    inet_ntoa_r(destAddr.sin_addr, addr_str, sizeof(addr_str) - 1);

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return;
    }
    ESP_LOGI(TAG, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&destAddr, sizeof(destAddr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        return;
    }
    ESP_LOGI(TAG, "Socket binded");

    err = listen(listen_sock, 1);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occured during listen: errno %d", errno);
        return;
    }
    ESP_LOGI(TAG, "Socket listening");
    while (1) {
        int sock = accept(listen_sock, (struct sockaddr *)&sourceAddr, &addrLen);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket accepted");

        while (1) {

            int len = recv(sock, file_msg, sizeof(file_msg) - 1, 0);
            if(len<=0||strstr(file_msg, "file"))
            {
                sscanf(file_msg, "file %s %d",file_name, &file_size);
            }else{
                ESP_LOGI(TAG, "解析文件失败\r\n");
                close(sock);
                break;
            }
            ESP_LOGI(TAG, "对方发送的文件名:%s 文件大小%d\r\n",file_name,file_size);
            inet_ntoa_r(((struct sockaddr_in *)&sourceAddr)->sin_addr.s_addr, addr_str, sizeof(addr_str) - 1);


            int err=write(sock,"GOGOGO", strlen("GOGOGO")); //告诉发送端，已经得到了文件的信息

            if (err < 0) {
                ESP_LOGE(TAG, "Error occured during sending: errno %d", errno);
                break;
            }
            fp=fopen("downlaod.bin", "wb+");//创建文件
            int dow_size = 0;//下载大小
            while (1)//不断接收数据
            {

                char data[4096] = {0};//读取网络数据cache date
                int size = read(sock,data,4096);
                dow_size += size;


                int ret = fwrite(data,size,1,fp);//写入本地文件
                if(ret <= 0){
                    fprintf(stderr, "fwrite failed\n");
                }

                if(dow_size >= file_size) //判断是否下载完毕
                {
                    printf("下载完毕 \r\n");
                    write(sock, "down_ok",strlen("down_ok")); //告诉发送端以及下载完毕 可以断开连接
                    fclose(fp);
                    close(sock);
                    break;

                }else{
                    printf("下载进度 : %d  %%\n", dow_size*100/file_size);
                }
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Close socket and restarting...");
            close(sock);
        }
    }
    vTaskDelete(NULL);
}

void init_tcp_server()
{
    ESP_ERROR_CHECK(esp_netif_init());
    xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
}

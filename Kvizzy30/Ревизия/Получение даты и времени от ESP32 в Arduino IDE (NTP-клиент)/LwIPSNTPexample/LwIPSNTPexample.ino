/** Arduino-Esp32-CAM                               *** LwIPSNTPexample.ino ***
 * 
 *       Использовать модуль LwIP SNTP для получения времени и даты с интернет-
 *  серверов с помощью SNTP - «простого сетевого протокола времени». Библиотека 
 *    LwIP SNTP обеспечивает получение текущего времени и синхронизацию таймера 
 *                                                         ESP32 с сервером NTP
 *         для плат ESP32 в версии библиотеки от Espressif Systems версии 3.0.6
 *                                        (на контроллере AI-Thinker ESP32-CAM)
 * 
 * v1.1, 12.11.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 11.11.2024
**/

#include <Arduino.h>
#include "nvs_flash.h"
#include "nvs.h"

// Готовим подключение к WiFi через имя и пароль точки доступа
#include "WiFi.h"
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

// Подключаем библиотеку для связи с сервером SNTP:

// - функция gettimeofday используется для получения текущей метки времени;
// - функция settimeofday используется для установки текущего времени при 
//   получении ответа от сервера NTP;
// - функция sntp_get_sync_status() используется для получения информации о 
//   состоянии синхронизации времени. Если время синхронизировано, то возвращается 
//   ответ SNTP_SYNC_RETURN_COMPLETED. Если состояние сброшено, то возвращается 
//   SNTP_SYC_STATUS_RESET. Кроме того, если происходит плавная синхронизация 
//   времени, то возвращается ответ SNTP_SYNC_STATUS_IN_PROGRESS;
// - функция sntp_set_sync_status() используется для установки статуса синхронизации 
//   времени. Она принимает один параметр — «sync_status», который является 
//   статусом синхронизации времени;
// - функция sntp_get_sync_mode() используется для получения режима синхронизации 
//   времени. Эта функция возвращает значение SNTP_SYNC_MODE_IMMEDIATE, если 
//   синхронизация настроена на немедленное обновление времени. В противном случае 
//   она возвращает SNTP_SYNC_MODE_SMOOTH - так как настроено плавное обновление времени;
// - функция sntp_set_sync_mode() используется для установки режима синхронизации 
//   времени. Она принимает один параметр «sync_mode», который является режимом 
//   синхронизации. Режим может быть SNTP_SYNC_MODE_IMMEDIATE или SNTP_SYNC_MODE_SMOOTH. 
//   Если выбран режим SNTP_SYNC_MODE_IMMEDIATE, системное время обновляется 
//   сразу после получения данных с сервера SNTP. Если выбран режим SNTP_SYNC_MODE_SMOOTH, 
//   устанавливается плавное обновление времени. В этом случае для уведомления 
//   о времени используется функция adjtime;
// - функция sntp_set_time_sync_notification_cb() используется для установки 
//   функции обратного вызова по уведомлению о синхронизации времени. Она 
//   принимает один параметр — функцию обратного вызова.

//   ESP32 использует два аппаратных таймера для отслеживания системного 
// времени: 
// - таймер RTC: позволяет отслеживать время в различных режимах сна, а также 
//   сохранять время при любых сбросах (за исключением сброса при включении питания, 
//   который сбрасывает таймер RTC). Отклонение частоты зависит от источников 
//   тактового сигнала таймера RTC и влияет на точность только в режимах сна, 
//   в которых время будет измеряться с точностью до 6,6667 мкс;
// - таймер с высоким разрешением: этот таймер недоступен в спящих режимах и не 
//   сохраняется после перезагрузки, но обладает большей точностью. Таймер 
//   использует источник тактовых импульсов APB_CLK (обычно 80 МГц), отклонение 
//   частоты которого составляет менее ±10 частей на миллион. Время измеряется с 
//   точностью до 1 мкс.
// Системное время можно отслеживать с помощью одного или обоих аппаратных 
// таймеров в зависимости от целей приложения и требований к точности системного 
// времени (при умалчиваемом режиме подключены оба таймера, это гарантирует, что 
// после синхронизации с сервером SNTP время будет отсчитываться с помощью обоих 
// таймеров. Когда плата ESP32 будет находиться в режиме глубокого сна, для отслеживания 
// времени будет использоваться встроенный таймер RTC, а таймер FRC1 с высоким 
// разрешением обеспечит точность времени до микросекунд, пока плата будет 
// находиться в активном режиме. 
#include "esp_sntp.h"

// Для обеспечения уровня абстракции приложения поверх стека TCP/IP и потокобезопасного
// API-интерфейса (для стека TCP/IP lwIP) подключаем ESP-NETIF
#include "esp_netif_sntp.h"
#include "esp_netif.h"

// Объявляем переменную, которая содержит количество перезагрузок ESP32. 
// RTC_DATA_ATTR указывает на хранение этой статической переменной типа int 
// в памяти RTC. Это гарантирует, что значение boot_count сохранится, когда плата 
// ESP32 перейдёт в режим глубокого сна, а затем проснётся.
RTC_DATA_ATTR static int boot_count = 0;  // !!! перезагрузка, потом разобраться

static void obtain_time(void);

// ****************************************************************************
// *      Подключить ESP32 к указанной сети Wi-Fi (непрерывно проверять       *
// *  состояние подключения до тех пор, пока оно не будет успешно установлено *
// ****************************************************************************
void initWiFi() 
{
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   {
      delay(500);
      Serial.print(".");
   }
   Serial.println(" ");
}
// ****************************************************************************
// *     Обновить системное время (принимается единственный параметр ‘tv’,    *
// *      который представляет собой время, полученное от SNTP-сервера)       *
// ****************************************************************************
/*
void sntp_sync_time(struct timeval *tv)
{
   // Устанавливаем текущее время, полученное от сервера NTP:
   // первый параметр данное с типом данных timeval,
   // второй параметр — это тип данных часового пояса, в данном случае равен NULL
   settimeofday(tv, NULL);
   // Отмечаем синхронизацию
   Serial.println("Время синхронизировано с помощью пользовательского кода!");
   // Устанавливаем статуса синхронизации времени как SNTP_SYNC_STATUS_COMPLETED
   sntp_set_sync_status(SNTP_SYNC_STATUS_COMPLETED);
}
*/
// ****************************************************************************
// *              Вывести уведомление о событии синхронизации                 *
// ****************************************************************************
void time_sync_notification_cb(struct timeval *tv)
{
   Serial.println("Время синхронизировано!");
}

static void obtain_time(void)
{
   // Инициализируем хранилище NVS 
   esp_err_t err = nvs_flash_init();
   // Если раздел NVS не содержит пустых страниц или он содержит данные в 
   // незнакомом формате, который не распознаётся текущей версией кода,
   // то стираем весь раздел и снова вызываем инициализацию
   if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) 
   {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
   }
   ESP_ERROR_CHECK(err);
   printf("Возврат nvs_flash_init() = %d: %s\n", err, esp_err_to_name(err));

   //err = ESP_ERROR_CHECK(esp_netif_init());
   // ESP_ERROR_CHECK(esp_event_loop_create_default()); - это для левого WiFi

   Serial.print("LWIP_DHCP_GET_NTP_SRV: ");
   Serial.println(LWIP_DHCP_GET_NTP_SRV);

   #if LWIP_DHCP_GET_NTP_SRV
   /**
    * Адрес NTP-сервера может быть получен через DHCP, 
    * смотрите следующие параметры настройки меню:
    * 
    * 'LWIP_DHCP_GET_NTP_SRV' - включить SNMP через DHCP
    * 'LWIP_SNMP_DEBUG' - включить отладочные сообщения
    * 
    * ПРИМЕЧАНИЕ: Этот вызов должен быть выполнен до того, 
    * как esp получит IP-адрес из DHCP,
    * в противном случае опция NTP по умолчанию будет отклонена.
   **/
      
      /*
      ESP_LOGI(TAG, "Initializing SNTP");
      //esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(CONFIG_SNTP_TIME_SERVER);
      esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
      config.start = false;                       // start SNTP service explicitly (after connecting)
      config.server_from_dhcp = true;             // accept NTP offers from DHCP server, if any (need to enable *before* connecting)
      config.renew_servers_after_new_IP = true;   // let esp-netif update configured SNTP server(s) after receiving DHCP lease
      config.index_of_first_server = 1;           // updates from server num 1, leaving server 0 (from DHCP) intact
      // configure the event on which we renew servers
      #ifdef CONFIG_EXAMPLE_CONNECT_WIFI
         config.ip_event_to_renew = IP_EVENT_STA_GOT_IP;
      #else
         config.ip_event_to_renew = IP_EVENT_ETH_GOT_IP;
      #endif
      config.sync_cb = time_sync_notification_cb; // only if we need the notification function
      esp_netif_sntp_init(&config);
      */
      
   #endif /* LWIP_DHCP_GET_NTP_SRV */

   /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
   * Read "Establishing Wi-Fi or Ethernet Connection" section in
   * examples/protocols/README.md for more information about this function.
   */
/*
#if LWIP_DHCP_GET_NTP_SRV
    ESP_LOGI(TAG, "Starting SNTP");
    esp_netif_sntp_start();
#if LWIP_IPV6 && SNTP_MAX_SERVERS > 2
    / * This demonstrates using IPv6 address as an additional SNTP server
     * (statically assigned IPv6 address is also possible)
     * /
    ip_addr_t ip6;
    if (ipaddr_aton("2a01:3f7::1", &ip6)) {    // ipv6 ntp source "ntp.netnod.se"
        esp_sntp_setserver(2, &ip6);
    }
#endif  / * LWIP_IPV6 * /

#else
    ESP_LOGI(TAG, "Initializing and starting SNTP");
#if CONFIG_LWIP_SNTP_MAX_SERVERS > 1
    / * This demonstrates configuring more than one server
     * /
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(2,
                               ESP_SNTP_SERVER_LIST(CONFIG_SNTP_TIME_SERVER, "pool.ntp.org" ) );
#else
    / *
     * This is the basic default config with one server and starting the service
     * /
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(CONFIG_SNTP_TIME_SERVER);
#endif
    config.sync_cb = time_sync_notification_cb;     // Note: This is only needed if we want
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    config.smooth_sync = true;
#endif

    esp_netif_sntp_init(&config);
#endif

    print_servers();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 15;
    while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    }
    time(&now);
    localtime_r(&now, &timeinfo);

    //ESP_ERROR_CHECK( example_disconnect() );
    esp_netif_sntp_deinit();
    */
}

void setup() 
{
   Serial.begin(115200);
   initWiFi();
   // initSNTP();
   // wait4SNTP();
}

void loop() 
{
   // После каждой перезагрузки увеличиваем счётчик загрузок
   // и выводим в терминал.
   ++boot_count;
   printf("Счетчик перезапусков = %d\n", boot_count);

   // Создаем структуру времени timeinfo. Выбираем, преобразуем время в секундах 
   // с начала эпохи в местное время и помещаем в эту структуру
   time_t now;
   struct tm timeinfo;
   time(&now);
   localtime_r(&now, &timeinfo);
   printf("Установившийся год: %d\n", timeinfo.tm_year);

   // Если правильное время ещё не установлено, вызываем функцию obtain_time(), 
   // которая получает время с сервера SNTP 
   if (timeinfo.tm_year < (2016 - 1900)) 
   {
      Serial.println("Время еще не установлено. Подключаемся к Wi-Fi и получаем время по протоколу NTP.");
      obtain_time();
      time(&now);
   }
   
   delay(1000);
}



/*
#include "NetworkEvents.h"
#include "protocol_examples_common.h"

#include "esp_sntp.h"

void app_main(void)
{
    
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    else {
        // add 500 ms error to the current system time.
        // Only to demonstrate a work of adjusting method!
        {
            ESP_LOGI(TAG, "Add a error for test adjtime");
            struct timeval tv_now;
            gettimeofday(&tv_now, NULL);
            int64_t cpu_time = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
            int64_t error_time = cpu_time + 500 * 1000L;
            struct timeval tv_error = { .tv_sec = error_time / 1000000L, .tv_usec = error_time % 1000000L };
            settimeofday(&tv_error, NULL);
        }

        ESP_LOGI(TAG, "Time was set, now just adjusting it. Use SMOOTH SYNC method.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
    }
#endif

    char strftime_buf[64];

    // Set timezone to Eastern Standard Time and print local time
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in New York is: %s", strftime_buf);

    // Set timezone to China Standard Time
    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

    if (sntp_get_sync_mode() == SNTP_SYNC_MODE_SMOOTH) {
        struct timeval outdelta;
        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS) {
            adjtime(NULL, &outdelta);
            ESP_LOGI(TAG, "Waiting for adjusting time ... outdelta = %jd sec: %li ms: %li us",
                        (intmax_t)outdelta.tv_sec,
                        outdelta.tv_usec/1000,
                        outdelta.tv_usec%1000);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }

    const int deep_sleep_sec = 10;
    ESP_LOGI(TAG, "Entering deep sleep for %d seconds", deep_sleep_sec);
    esp_deep_sleep(1000000LL * deep_sleep_sec);
}

static void print_servers(void)
{
    ESP_LOGI(TAG, "List of configured NTP servers:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i){
        if (esp_sntp_getservername(i)){
            ESP_LOGI(TAG, "server %d: %s", i, esp_sntp_getservername(i));
        } else {
            // we have either IPv4 or IPv6 address, let's print it
            char buff[INET6_ADDRSTRLEN];
            ip_addr_t const *ip = esp_sntp_getserver(i);
            if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != NULL)
                ESP_LOGI(TAG, "server %d: %s", i, buff);
        }
    }
}

static void obtain_time(void)
{
    ESP_ERROR_CHECK( nvs_flash_init() );
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK( esp_event_loop_create_default() );

#if LWIP_DHCP_GET_NTP_SRV
    / **
     * NTP server address could be acquired via DHCP,
     * see following menuconfig options:
     * 'LWIP_DHCP_GET_NTP_SRV' - enable STNP over DHCP
     * 'LWIP_SNTP_DEBUG' - enable debugging messages
     *
     * NOTE: This call should be made BEFORE esp acquires IP address from DHCP,
     * otherwise NTP option would be rejected by default.
     * /
    ESP_LOGI(TAG, "Initializing SNTP");
    //esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(CONFIG_SNTP_TIME_SERVER);
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    config.start = false;                       // start SNTP service explicitly (after connecting)
    config.server_from_dhcp = true;             // accept NTP offers from DHCP server, if any (need to enable *before* connecting)
    config.renew_servers_after_new_IP = true;   // let esp-netif update configured SNTP server(s) after receiving DHCP lease
    config.index_of_first_server = 1;           // updates from server num 1, leaving server 0 (from DHCP) intact
    // configure the event on which we renew servers
#ifdef CONFIG_EXAMPLE_CONNECT_WIFI
    config.ip_event_to_renew = IP_EVENT_STA_GOT_IP;
#else
    config.ip_event_to_renew = IP_EVENT_ETH_GOT_IP;
#endif
    config.sync_cb = time_sync_notification_cb; // only if we need the notification function
    esp_netif_sntp_init(&config);

#endif / * LWIP_DHCP_GET_NTP_SRV * /

    / * This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     * /
    //ESP_ERROR_CHECK(example_connect());

#if LWIP_DHCP_GET_NTP_SRV
    ESP_LOGI(TAG, "Starting SNTP");
    esp_netif_sntp_start();
#if LWIP_IPV6 && SNTP_MAX_SERVERS > 2
    / * This demonstrates using IPv6 address as an additional SNTP server
     * (statically assigned IPv6 address is also possible)
     * /
    ip_addr_t ip6;
    if (ipaddr_aton("2a01:3f7::1", &ip6)) {    // ipv6 ntp source "ntp.netnod.se"
        esp_sntp_setserver(2, &ip6);
    }
#endif  / * LWIP_IPV6 * /

#else
    ESP_LOGI(TAG, "Initializing and starting SNTP");
#if CONFIG_LWIP_SNTP_MAX_SERVERS > 1
    / * This demonstrates configuring more than one server
     * /
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(2,
                               ESP_SNTP_SERVER_LIST(CONFIG_SNTP_TIME_SERVER, "pool.ntp.org" ) );
#else
    / *
     * This is the basic default config with one server and starting the service
     * /
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(CONFIG_SNTP_TIME_SERVER);
#endif
    config.sync_cb = time_sync_notification_cb;     // Note: This is only needed if we want
#ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
    config.smooth_sync = true;
#endif

    esp_netif_sntp_init(&config);
#endif

    print_servers();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 15;
    while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    }
    time(&now);
    localtime_r(&now, &timeinfo);

    //ESP_ERROR_CHECK( example_disconnect() );
    esp_netif_sntp_deinit();
}
*/

// **************************************************** LwIPSNTPexample.ino ***

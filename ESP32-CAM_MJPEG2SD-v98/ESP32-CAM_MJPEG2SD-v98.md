## [ESP32-CAM_MJPEG2SD версия 9.8](https://github.com/s60sc/ESP32-CAM_MJPEG2SD)

Приложение для захвата движения камеры ESP32 для записи файлов JPEG на SD-карту в формате AVI и потоковой передачи в браузер в формате MJPEG. Если установлен микрофон, то также создается файл WAV. Файлы можно загружать по FTP или в браузер.

#### 2024-08-02: делаю очередную попытку запустить проект

- настройки обычные на сегодня:

```
// Payment:                              "Al Thinker ESP32-CAM"
// CPU Frequency:                        "240MHz (WiFi/BT)"
// Flash Frequency:                      "80MHz"
// Flash Mode:                           "QIO"
// Partition Scheme:                     "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                     "Ничего"
// Erase All Flash Before Sketch Upload: "Enabled"
// Port:                                 "COM5"

// Additional links for the Board Manager: 
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

// резерв:                                 https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
// Менеджер плат:                          esp32 by Espressif Systems 3.0.3 installed
```
- следующие замечания считаю выполненными:

необходимая плата ESP-CAM в appGlobals.h установлена, как CAMERA_MODEL_AI_THINKER;

по уровню отладки Verbose определено, что PSRAM = 4Мб;

выбрана следующая схема разделения: ESP32 - Minimal SPIFFS (...)

- проверка компиляции прошла успешно

> При первой установке приложение запустится в режиме точки доступа Wi-Fi - подключитесь к SSID: ESP-CAM_MJPEG_ ..., чтобы разрешить ввод сведений о маршрутизаторе и пароле через веб-страницу на ***192.168.4.1***. Файл данных конфигурации (за исключением паролей) создается автоматически, а веб-страницы приложения автоматически загружаются с GitHub в папку SD-карты /data при наличии подключения к Интернету.
> 

> Последующие обновления приложения или файлов в папке /data можно выполнить с помощью вкладки "Загрузка OTA". Папку /data также можно перезагрузить с GitHub с помощью кнопки Reload /data на вкладке Edit Config или с помощью клиента WebDAV.
> 

- определил WiFi сеть для просмотра стрима на смартфоне:

```
ssid     = "TP-Link_B394";
password = "18009217";
```
- запустил откомпилированное приложение и подключил к SSID: ESP-CAM_MJPEG_ ... чтобы разрешить ввод сведений о маршрутизаторе и пароле через веб-страницу на **http://192.168.4.1** (когда было OPPO, то был другой адрес =  http://192.168.255.35.

***"ESP-CAM_MJPEG_90B3DB7CE5A4 started, use 'http://192.168.4.1' to connect"***

- ввел данные по WiFi сети, подключился и подтвердил перезагрузку устройства;

- повторно запустил страницу **http://192.168.4.1**, появился экран, поработал, фото и немного стрима получилось;

- хотел перейти на смартфон, но похоже приложение только один запуск в день разрешает.

- ПРОДОЛЖУ изыскания, когда придут радиаторы и переделаю антенну

- 
```
[00:00:08.159 isNight] Night time
[00:00:10.663 resetWatchDog] WatchDog started using task: ping
ets Jun  8 2016 00:22:57

rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0030,len:1448
load:0x40078000,len:14844
ho 0 tail 12 room 4
load:0x40080400,len:4
load:0x40080404,len:3356
entry 0x4008059c


[00:00:00.474 logSetup] Setup RAM based log, size 7168, starting from 0


[00:00:00.475 logSetup] =============== ESP-CAM_MJPEG 9.9.1 ===============
[00:00:00.485 logSetup] Compiled with arduino-esp32 v3.0.3
[00:00:00.485 printResetReason] Power on reset
[00:00:00.496 printWakeupReason] Wakeup by reset
[00:00:00.543 infoSD] SD card type SDHC, Size: 14.7GB
[00:00:00.544 listFolder] Sketch size 1.7MB
[00:00:00.893 listFolder] File: /data/configs.txt, size: 6736 bytes
[00:00:00.894 listFolder] SD_MMC: 1.2MB used of 14.7GB
[00:00:00.894 loadConfig] Load config
[00:00:01.147 updateAppStatus] Enabling motion detection
[00:00:01.15M⸮⸮Y]]⸮W⸮⸮SRAM size: 4.0MB
[00:00:01.349 prepCam] Camera init OK for model OV2640 on board CAMERA_MODEL_AI_THINKER
[00:00:01.600 setWifiSTA] Wifi Station IP from DHCP
[00:00:01.626 WARN startWifi] SSID  not connected not available, use AP 
[00:00:01.629 onWiFiEvent] Wifi AP SSID: ESP-CAM_MJPEG_90B3DB7CE5A4 started, use 'http://192.168.4.1' to connect
[00:00:05.257 startPing] Started ping monitoring - On
[00:00:05.262 startWebServer] Starting web server on port: 80
[00:00:05.262 startWebServer] Remote server certificates not checked
[00:00:05.262 startSustainTasks] Started 1 HTTP sustain tasks
[00:00:05.273 prepUpload] File uploads will use FTP server
[00:00:05.273 prepTelegram] Telegram not being used
[00:00:05.319 updateAppStatus] Enabling motion detection
[00:00:05.470 prepRecording] To record new AVI, do one of:
[00:00:05.470 prepRecording] - press Start Recording on web page
[00:00:05.471 prepRecording] - move in front of camera
 
[00:00:05.481 prepRecording] Camera model OV2640 on board CAMERA_MODEL_AI_THINKER ready @ 20MHz
[00:00:05.491 checkMemory]  Free: heap 95468, block: 69620, min: 95452, pSRAM 1485600
[00:00:05.492 loop] =============== Total tasks: 17 ===============

[00:00:07.754 isNight] Night time
[00:00:10.259 resetWatchDog] WatchDog started using task: ping


Ввел данные ЦшАш и подтвердил перезагрузук



[00:00:05.481 prepRecording] Camera model OV2640 on board CAMERA_MODEL_AI_THINKER ready @ 20MHz
[00:00:05.491 checkMemory]  Free: heap 95468, block: 69620, min: 95452, pSRAM 1485600
[00:00:05.492 loop] =============== Total tasks: 17 ===============

[00:00:07.754 isNight] Night time
[00:00:10.259 resetWatchDog] WatchDog started using task: ping
[00:03:43.230 onWiFiEvent] WiFi AP client connection
[00:05:24.548 savePrefs] Saved preferences
[00:05:24.570 saveConfigVect] Config file saved 
[00:05:24.666 doRestart] Controlled restart: User requested restart 
[00:05:26.669 onWiFiEvent] Wifi Sti⸮Z⸮jո⸮ed TP-Link_B394
[00:05:26n⸮⸮H⸮⸮U⸮⸮⸮W⸮⸮⸮,U⸮T-⸮⸮⸮+⸮ˋ"⸮͍⸮⸮⸮⸮⸮ѥ⸮⸮)⸮ets Jun  8 2016 00:22:57

rst:0xc (SW_CPU_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0030,len:1448
load:0x40078000,len:14844
ho 0 tail 12 room 4
load:0x40080400,len:4
load:0x40080404,len:3356
entry 0x4008059c


[00:05:27.733 logSetup] Setup RAM based log, size 7168, starting from 2261


[00:05:27.734 logSetup] =============== ESP-CAM_MJPEG 9.9.1 ===============
[00:05:27.744 logSetup] Compiled with arduino-esp32 v3.0.3
[00:05:27.744 printResetReason] Software reset via esp_restart
[00:05:27.755 printWakeupReason] Wakeup by reset
[00:05:27.803 infoSD] SD card type SDHC, Size: 14.7GB
[00:05:27.804 listFolder] Sketch size 1.7MB
[00:05:28.153 listFolder] File: /data/configs.txt, size: 6747 bytes
[00:05:28.154 listFolder] SD_MMC: 1.2MB used of 14.7GB
[00:05:28.154 loadConfig] Load config
[00:05:28.409 updateAppStatus] Enabling motion detection
[00:05:28.418 setup] PSRAM size: 4.0MB
[00:05:28.611 prepCam] Camera init OK for model OV2640 on board CAMERA_MODEL_AI_THINKER
[00:05:28.881 setWifiSTA] Wifi Station IP from DHCP
..[00:05:29.477 onWiFiEvent] WiFi Station connection to TP-Link_B394, using hostname: ESP-CAM_MJPEG_90B3DB7CE5A4
.[00:05:30.003 onWiFiEvent] Wifi Station IP, use 'http://192.168.0.104' to connect
[00:05:30.435 on⸮⸮*⸮⸮W⸮⸮W⸮⸮+R⸮⸮⸮RQ⸮⸮ESP-CAM_MJPEG_90B3DB7CE5A4 started, use 'http://192.168.4.1' to connect
[00:05:30.656 onWiFiEvent] Station interface V6 IP addr is preferred
[00:05:34.060 startWifi] Wifi stats for TP-Link_B394 - signal stre⸮-'HKMMH⸮⸮m; Encrypion: WPA2_PSK; channel: 3
[00:05:34.062 startPing] Started ping monitoring - On⸮҂⸮Қ⸮r⸮⸮⸮⸮⸮se⸮WatchDog] WatchDog started using task: ping
[00:05:34.073 getLocalNTP] Using NTP server: pool.ntp.org
[00:05:34.084 WARN getLocalNTP] Not yet synced with NTP 
[00:05:34.084 startWebServer] Starting web server on port: 80
[00:05:34.095 startWebServer] R⸮e server certificates not checked
[00:05:34.095 startSustainTasks] Started 1 HTTP sustain tasks
[00:05:34.106 prepUpload] File uploads will use FTP server
[00:05:34.106 prepTelegram] Telegram not being used
[00:05:34.143 updateAppStatus] Enabling motion detection
⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮ɕ⸮I⸮⸮⸮ɑ⸮⸮⸮u⸮⸮⸮⸮⸮⸮⸮⸮ɑ⸮r⸮݁
Y%⸮"⸮⸮z⸮⸮⸮z⸮⸮R⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮ɕ⸮I⸮⸮⸮ɑ⸮⸮⸮u⸮j⸮ɕ⸮́⸮х⸮с⸮⸮⸮⸮ɑ⸮⸮⸮⸮⸮⸮⸮ڕ⸮⸮⸮⸮⸮⸮)m⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮ɕ⸮I⸮⸮⸮ɑ⸮⸮⸮u⸮jj⸮ٕ⸮J⸮⸮2ɽ⸮сz⸮⸮⸮⸮⸮Ʌ)R⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮ɕ⸮I⸮⸮⸮ɑ⸮⸮⸮u⸮⸮⸮⸮Ʌ⸮j⸮⸮⸮⸮⸮zYɲ⸮⸮z⸮⸮⸮⸮ɑ⸮5I}5=1}%}Q!%9-I⸮⸮⸮⸮⸮⸮⸮⸮⸮j⸮⸮)⸮⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮⸮⸮⸮5⸮⸮⸮⸮⸮u⸮2%⸮⸮⸮B⸮⸮⸮⸮⸮⸮⸮b⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮bj⸮⸮遢⸮⸮⸮⸮b⸮MI5⸮ʢª⸮⸮⸮R⸮⸮҂⸮Қ⸮r⸮⸮⸮b⸮⸮⸮u⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮х⸮⸮⸮⸮ͭ⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮R⸮⸮[00:05:34.559 wgetFile] Downloading /data/common.js from /s60sc/ESP32-CAM_MJPEG2SD/master/data/common.js
[00:05:35.202 wgetFile] Downloaded /data/common.js, size 38896 bytes
[00:05:35.806 wgetFile] Downloading /data/MJPEG2SD.htm from /s60sc/ESP32-CAM_MJPEG2SD/master/data/MJPEG2SD.htm
[00:05:36.514 isNight] Night time
[00:05:36.968 wgetFile] Downloaded /data/MJPEG2SD.htm, size 92KB
[17:46:47.672 onWiFiEvent] WiFi AP client connection
[17:47:11.039 getLocalNTP] Using NTP server: pool.ntp.org
[17:47:11.039 showLos⸮U⸮⸮V⸮ꋁ⸮⸮ɕ⸮с⸮⸮⸮⸮⸮2ɽ⸮⸮rQA⸮⸮⸮z⸮⸮z⸮⸮⸮⸮⸮⸮Ң⸮Ҋ⸮⸮⸮ѡ⸮⸮⸮⸮:⸮ER⸮[17:47:41.043 setAlarm] Alarm scheduled at 03/08/2024 01:00:00
[17:47:41.898 saveConfigVect] Config file saved 
[17:47:41.912 doAppPing] Daily rollover


```

Понажимал по всякому клавиши, протокол сом-порта:

```


[00:05:27.733 logSetup] Setup RAM based log, size 7168, starting from 2261


[00:05:27.734 logSetup] =============== ESP-CAM_MJPEG 9.9.1 ===============
[00:05:27.744 logSetup] Compiled with arduino-esp32 v3.0.3
[00:05:27.744 printResetReason] Software reset via esp_restart
[00:05:27.755 printWakeupReason] Wakeup by reset
[00:05:27.803 infoSD] SD card type SDHC, Size: 14.7GB
[00:05:27.804 listFolder] Sketch size 1.7MB
[00:05:28.153 listFolder] File: /data/configs.txt, size: 6747 bytes
[00:05:28.154 listFolder] SD_MMC: 1.2MB used of 14.7GB
[00:05:28.154 loadConfig] Load config
[00:05:28.409 updateAppStatus] Enabling motion detection
[00:05:28.418 setup] PSRAM size: 4.0MB
[00:05:28.611 prepCam] Camera init OK for model OV2640 on board CAMERA_MODEL_AI_THINKER
[00:05:28.881 setWifiSTA] Wifi Station IP from DHCP
..[00:05:29.477 onWiFiEvent] WiFi Station connection to TP-Link_B394, using hostname: ESP-CAM_MJPEG_90B3DB7CE5A4
.[00:05:30.003 onWiFiEvent] Wifi Station IP, use 'http://192.168.0.104' to connect
[00:05:30.435 on⸮⸮*⸮⸮W⸮⸮W⸮⸮+R⸮⸮⸮RQ⸮⸮ESP-CAM_MJPEG_90B3DB7CE5A4 started, use 'http://192.168.4.1' to connect
[00:05:30.656 onWiFiEvent] Station interface V6 IP addr is preferred
[00:05:34.060 startWifi] Wifi stats for TP-Link_B394 - signal stre⸮-'HKMMH⸮⸮m; Encrypion: WPA2_PSK; channel: 3
[00:05:34.062 startPing] Started ping monitoring - On⸮҂⸮Қ⸮r⸮⸮⸮⸮⸮se⸮WatchDog] WatchDog started using task: ping
[00:05:34.073 getLocalNTP] Using NTP server: pool.ntp.org
[00:05:34.084 WARN getLocalNTP] Not yet synced with NTP 
[00:05:34.084 startWebServer] Starting web server on port: 80
[00:05:34.095 startWebServer] R⸮e server certificates not checked
[00:05:34.095 startSustainTasks] Started 1 HTTP sustain tasks
[00:05:34.106 prepUpload] File uploads will use FTP server
[00:05:34.106 prepTelegram] Telegram not being used
[00:05:34.143 updateAppStatus] Enabling motion detection
⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮ɕ⸮I⸮⸮⸮ɑ⸮⸮⸮u⸮⸮⸮⸮⸮⸮⸮⸮ɑ⸮r⸮݁
Y%⸮"⸮⸮z⸮⸮⸮z⸮⸮R⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮ɕ⸮I⸮⸮⸮ɑ⸮⸮⸮u⸮j⸮ɕ⸮́⸮х⸮с⸮⸮⸮⸮ɑ⸮⸮⸮⸮⸮⸮⸮ڕ⸮⸮⸮⸮⸮⸮)m⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮ɕ⸮I⸮⸮⸮ɑ⸮⸮⸮u⸮jj⸮ٕ⸮J⸮⸮2ɽ⸮сz⸮⸮⸮⸮⸮Ʌ)R⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮ɕ⸮I⸮⸮⸮ɑ⸮⸮⸮u⸮⸮⸮⸮Ʌ⸮j⸮⸮⸮⸮⸮zYɲ⸮⸮z⸮⸮⸮⸮ɑ⸮5I}5=1}%}Q!%9-I⸮⸮⸮⸮⸮⸮⸮⸮⸮j⸮⸮)⸮⸮⸮҂⸮Қ⸮r⸮⸮⸮⸮⸮⸮⸮5⸮⸮⸮⸮⸮u⸮2%⸮⸮⸮B⸮⸮⸮⸮⸮⸮⸮b⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮bj⸮⸮遢⸮⸮⸮⸮b⸮MI5⸮ʢª⸮⸮⸮R⸮⸮҂⸮Қ⸮r⸮⸮⸮b⸮⸮⸮u⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮х⸮⸮⸮⸮ͭ⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮R⸮⸮[00:05:34.559 wgetFile] Downloading /data/common.js from /s60sc/ESP32-CAM_MJPEG2SD/master/data/common.js
[00:05:35.202 wgetFile] Downloaded /data/common.js, size 38896 bytes
[00:05:35.806 wgetFile] Downloading /data/MJPEG2SD.htm from /s60sc/ESP32-CAM_MJPEG2SD/master/data/MJPEG2SD.htm
[00:05:36.514 isNight] Night time
[00:05:36.968 wgetFile] Downloaded /data/MJPEG2SD.htm, size 92KB
[17:46:47.672 onWiFiEvent] WiFi AP client connection
[17:47:11.039 getLocalNTP] Using NTP server: pool.ntp.org
[17:47:11.039 showLos⸮U⸮⸮V⸮ꋁ⸮⸮ɕ⸮с⸮⸮⸮⸮⸮2ɽ⸮⸮rQA⸮⸮⸮z⸮⸮z⸮⸮⸮⸮⸮⸮Ң⸮Ҋ⸮⸮⸮ѡ⸮⸮⸮⸮:⸮ER⸮[17:47:41.043 setAlarm] Alarm scheduled at 03/08/2024 01:00:00
[17:47:41.898 saveConfigVect] Config file saved 
[17:47:41.912 doAppPing] Daily rollover
[17:51:00.539 wsHandler] Websocket connection: 54
[17:51:08.313 appSpecificWebHandler] JPEG: 14483B in 130ms
[17:51:23.912 isNight] Day time
[17:51:26.51⸮⸮⸮⸮smi*W⸮⸮⸮.⸮................................................ 
......................⸮........................... 
..................................[17:51:33.187 appSpecificWebHandler] JPEG: 28814B in 137ms
................ 
.......... 
[17:51:34.640 closeAvi] ******** AVI recording stats ********
[17:51:34.641 closeAvi] Recorded /20240802/20240802_175126_SVGA_20_8.avi 
[17:51:34.651 closeAvi] AVI duration: 8 secs
[17:51:34.651 closeAvi] Number of frames: 160
[17:51:34.651 closeAvi] Required FPS: 20
[17:51:34.662 closeAvi] Actual FPS: 20.2
[17:51:34.662 closeAvi] File size: 4.0MB
[17:51:34.662 closeAvi] Average frame length: 26237 bytes
[17:51:34.673 closeAvi] Average frame monitoring time: 6 ms
[17:51:34.673 closeAvi] Average frame buffering time: 0 ms
[17:51:34.683 closeAvi] Average frame storage time: 16 ms
[17:51:34.684 closeAvi] Average SD write speed: 1595 kB/s
[17:51:34.694 closeAvi] File open / completion times: 132 ms / 69 ms
[17:51:34.694 closeAvi] Busy: 48%
[17:51:34.694 checkMemory]  Free: heap 98604, block: 36852, min: 40176, pSRAM 1027440
[17:51:34.705 closeAvi] *************************************
[17:51:34.716 checkFreeStorage] Storage free space: 14.7GB
[17:51:36.998 processFrame] Capture started by Motion  
........................................ 
.................................⸮............... 
...............................⸮nn⸮.>............. 
.................................................. 
...............[17:51:47.135 appSpecificWebHandler] JPEG: 15818B in 118ms
................................... 
.................................................. 
.................................................. 
...........................[17:51:55.240 appSpecificWebHandler] JPEG: 15023B in 118ms
....................... 
..................⸮.⸮............................. 
.................................................. 
....⸮.../..................... 
[17:52:03.057 closeAvi] ******** AVI recording stats ********
[17:52:03.057 closeAvi] Recorded /20240802/20240802_175136_SVGA_20_26.avi 
[17:52:03.067 closeAvi] AVI duration: 26 secs
[17:52:03.068 closeAvi] Number of frames: 520
[17:52:03.068 closeAvi] Required FPS: 20
[17:52:03.078 closeAvi] Actual FPS: 20.0
[17:52:03.079 closeAvi] File size: 8.4MB
[17:52:03.079 closeAvi] Average frame length: 16879 bytes
[17:52:03.089 closeAvi] Average frame monitoring time: 2 ms
[17:52:03.090 closeAvi] Average frame buffering time: 0 ms
[17:52:03.100 closeAvi] Average frame storage time: 10 ms
[17:52:03.100 closeAvi] Average SD write speed: 1562 kB/s
[17:52:03.111 closeAvi] File open / completion times: 11 ms / 60 ms
[17:52:03.111 closeAvi] Busy: 26%
[17:52:03.111 checkMemory]  Free: heap 98868, block: 36852, min: 40176, pSRAM 1027440
[17:52:03.122 closeAvi] *************************************
[17:52:03.132 checkFreeStorage] Storage free space: 14.7GB
[17:52:11.855 processFrame] Capture started by Button 
.................... 
.................................................. 
............................/..................... 
...... 
[17:52:18.189 closeAvi] ******** AVI recording stats ********
[17:52:18.189 closeAvi] Recorded /20240802/20240802_175211_SVGA_20_6.avi 
[17:52:18.200 closeAvi] AVI duration: 6 secs
[17:52:18.200 closeAvi] Number of frames: 126
[17:52:18.200 closeAvi] Required FPS: 20
[17:52:18.210 closeAvi] Actual FPS: 20.0
[17:52:18.211 closeAvi] File size: 1.9MB
[17:52:18.211 closeAvi] Average frame length: 15433 bytes
[17:52:18.221 closeAvi] Average frame monitoring time: 0 ms
[17:52:18.222 closeAvi] Average frame buffering time: 0 ms
[17:52:18.232 closeAvi] Average frame storage time: 9 ms
[17:52:18.232 closeAvi] Average SD write speed: 1584 kB/s
[17:52:18.243 closeAvi] File open / completion times: 10 ms / 34 ms
[17:52:18.243 closeAvi] Busy: 20%
[17:52:18.243 checkMemory]  Free: heap 98964, block: 36852, min: 40176, pSRAM 1027440
[17:52:18.254 closeAvi] *************************************
[17:52:18.264 checkFreeStorage] Storage free space: 14.7GB
[17:52:22.156 showStream] MJPEG: 1 frames, total 15579 bytes in 0.0s @ 31.2fps
[17:52:23.897 processFrame] Capture started by Motion  
............................................ 
.................................................. 
.................................................. 
........................n......................... 
.................................................. 
.................................................. 
...................................⸮............⸮.⸮⸮nn⸮n⸮n⸮⸮⸮.......n⸮n.n>.n⸮>n⸮nnn>⸮⸮n.⸮............. 
.................................................. 
.................................................. 
...............................................⸮.. 
.................................................. 
...... 
[17:52:53.964 closeAvi] ******** AVI recording stats ********
[17:52:53.965 closeAvi] Recorded /20240802/20240802_175223_SVGA_20_30.avi 
[17:52:53.975 closeAvi] AVI duration: 30 secs
[17:52:53.975 closeAvi] Number of frames: 600
[17:52:53.975 closeAvi] Required FPS: 20
[17:52:53.986 closeAvi] Actual FPS: 20.0
[17:52:53.986 closeAvi] File size: 9.6MB
[17:52:53.986 closeAvi] Average frame length: 16778 bytes
[17:52:53.997 closeAvi] Average frame monitoring time: 2 ms
[17:52:53.997 closeAvi] Average frame buffering time: 0 ms
[17:52:54.007 closeAvi] Average frame storage time: 9 ms
[17:52:54.007 closeAvi] Average SD write speed: 1653 kB/s
[17:52:54.018 closeAvi] File open / completion times: 10 ms / 67 ms
[17:52:54.018 closeAvi] Busy: 25%
[17:52:54.018 checkMemory]  Free: heap 99048, block: 36852, min: 40176, pSRAM 1027440
[17:52:54.029 closeAvi] *************************************
[17:52:54.039 checkFreeStorage] Storage free space: 14.7GB
[17:52:54.898 processFrame] Capture started by Motion  
......./.................................... 
.................................................. 
.................................................. 
.................................................. 
...[17:53:04.644 showStream] MJPEG: 125 frames, total 2.1MB in 6.5s @ 19.2fps
... 
[17:53:04.942 closeAvi] ******** AVI recording stats ********
[17:53:04.942 closeAvi] Recorded /20240802/20240802_175254_SVGA_20_10.avi 
[17:53:04.953 closeAvi] AVI duration: 10 secs
[17:53:04.953 closeAvi] Number of frames: 200
[17:53:04.953 closeAvi] Required FPS: 20
[17:53:04.963 closeAvi] Actual FPS: 20.0
[17:53:04.964 closeAvi] File size: 3.3MB
[17:53:04.964 closeAvi] Average frame length: 17340 bytes
[17:53:04.974 closeAvi] Average frame monitoring time: 2 ms
[17:53:04.975 closeAvi] Average frame buffering time: 0 ms
[17:53:04.985 closeAvi] Average frame storage time: 8 ms
[17:53:04.985 closeAvi] Average SD write speed: 1890 kB/s
[17:53:04.996 closeAvi] File open / completion times: 10 ms / 42 ms
[17:53:04.996 closeAvi] Busy: 24%
[17:53:04.996 checkMemory]  Free: heap 98180, block: 36852, min: 40176, pSRAM 1027440
[17:53:05.007 closeAvi] *************************************
[17:53:05.017 checkFreeStorage] Storage free space: 14.7GB
[17:53:06.898 processFrame] Capture started by Motion  
...........................⸮................ 
....................................⸮............. 
...................⸮.............................. 
........⸮....... 
[17:53:15.028 closeAvi] ******** AVI recording stats ********
[17:53:15.029 closeAvi] Recorded /20240802/20240802_175306_SVGA_20_8.avi 
[17:53:15.039 closeAvi] AVI duration: 8 secs
[17:53:15.039 closeAvi] Number of frames: 160
[17:53:15.040 closeAvi] Required FPS: 20
[17:53:15.050 closeAvi] Actual FPS: 20.0
[17:53:15.050 closeAvi] File size: 4.0MB
[17:53:15.051 closeAvi] Average frame length: 25978 bytes
[17:53:15.061 closeAvi] Average frame monitoring time: 2 ms
[17:53:15.061 closeAvi] Average frame buffering time: 0 ms
[17:53:15.071 closeAvi] Average frame storage time: 15 ms
[17:53:15.072 closeAvi] Average SD write speed: 1661 kB/s
[17:53:15.082 closeAvi] File open / completion times: 10 ms / 107 ms
[17:53:15.082 closeAvi] Busy: 38%
[17:53:15.082 checkMemory]  Free: heap 97028, block: 36852, min: 40176, pSRAM 1027440
[17:53:15.093 closeAvi] *************************************
[17:53:15.103 checkFreeStorage] Storage free space: 14.7GB
[17:53:51.972 processFrame] Capture started by Motion  
.................................. 
.................................................. 
.................................................. 
..............⸮................................... 
.................................................. 
⸮..........................................⸮.. 
[17:54:06.018 closeAvi] ******** AVI recording stats ********
[17:54:06.018 closeAvi] Recorded /20240802/20240802_175351_SVGA_20_14.avi 
[17:54:06.028 closeAvi] AVI duration: 14 secs
[17:54:06.029 closeAvi] Number of frames: 280
[17:54:06.029 closeAvi] Required FPS: 20
[17:54:06.039 closeAvi] Actual FPS: 20.0
[17:54:06.040 closeAvi] File size: 7.8MB
[17:54:06.040 closeAvi] Average frame length: 29054 bytes
[17:54:06.050 closeAvi] Average frame monitoring time: 2 ms
[17:54:06.050 closeAvi] Average frame buffering time: 0 ms
[17:54:06.061 closeAvi] Average frame storage time: 14 ms
[17:54:06.061 closeAvi] Average SD write speed: 1917 kB/s
[17:54:06.071 closeAvi] File open / completion times: 10 ms / 46 ms
[17:54:06.071 closeAvi] Busy: 36%
[17:54:06.082 checkMemory]  Free: heap 98624, block: 36852, min: 40176, pSRAM 1027440
[17:54:06.082 closeAvi] *************************************
[17:54:06.093 checkFreeStorage] Storage free space: 14.7GB
[17:54:09.965 isNight] Night time
```




## [ESP32-CAM_MJPEG2SD версия 9.8](https://github.com/s60sc/ESP32-CAM_MJPEG2SD)

Приложение для захвата движения камеры ESP32 для записи файлов JPEG на SD-карту в формате AVI и потоковой передачи в браузер в формате MJPEG. Если установлен микрофон, то также создается файл WAV. Файлы можно загружать по FTP или в браузер.


- настройки обычные на сегодня: 2025-02-05

```
// Payment:                              "Al Thinker ESP32-CAM"
// CPU Frequency:                        "240MHz (WiFi/BT)"
// Flash Frequency:                      "80MHz"
// Flash Mode:                           "QIO"
// Partition Scheme:                     "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                     "Ничего"
// Erase All Flash Before Sketch Upload: "Enabled"

// Additional links for the Board Manager: 
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

// резерв:                                 https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
// Менеджер плат:                          esp32 by Espressif Systems 3.0.7 installed
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

- ??? повторно запустил страницу **http://192.168.4.1**, появился экран, поработал, фото и немного стрима получилось;

- ??? хотел перейти на смартфон, но похоже приложение только один запуск в день разрешает.

***1 запуск:***

```
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
[00:00:01.156 SRAM size: 4.0MB
[00:00:01.349 prepCam] Camera init OK for model OV2640 on board CAMERA_MODEL_AI_THINKER
[00:00:01.600 setWifiSTA] Wifi Station IP from DHCP
[00:00:01.626 WARN startWifi] SSID  not connected not available, use AP 
[00:00:01.629 onWiFiEvent] Wifi AP SSID: ESP-CAM_MJPEG_90B3DB7CE5A4 started, use 'http://192.168.4.1' to connect

[00:00:07.754 isNight] Night time
[00:00:10.259 resetWatchDog] WatchDog started using task: ping
[00:03:43.230 onWiFiEvent] WiFi AP client connection
[00:05:24.548 savePrefs] Saved preferences
[00:05:24.570 saveConfigVect] Config file saved 
[00:05:24.666 doRestart] Controlled restart: User requested restart 
[00:05:26.669 onWiFiEvent] Wifi Sti⸮Z⸮jո⸮ed TP-Link_B394

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
[00:05:29.477 onWiFiEvent] WiFi Station connection to TP-Link_B394, using hostname: ESP-CAM_MJPEG_90B3DB7CE5A4
[00:05:30.003 onWiFiEvent] Wifi Station IP, use 'http://192.168.0.104' to connect
[00:05:30.435 ESP-CAM_MJPEG_90B3DB7CE5A4 started, use 'http://192.168.4.1' to connect
[00:05:30.656 onWiFiEvent] Station interface V6 IP addr is preferred
[00:05:34.060 startWifi] Wifi stats for TP-Link_B394 - signal stre⸮-'HKMMH⸮⸮m; Encrypion: WPA2_PSK; channel: 3
[00:05:34.062 startPing] Started ping monitoring - OnWatchDog] WatchDog started using task: ping
[00:05:34.073 getLocalNTP] Using NTP server: pool.ntp.org
[00:05:34.084 WARN getLocalNTP] Not yet synced with NTP 
[00:05:34.084 startWebServer] Starting web server on port: 80
[00:05:34.095 startWebServer] R⸮e server certificates not checked
[00:05:34.095 startSustainTasks] Started 1 HTTP sustain tasks
[00:05:34.106 prepUpload] File uploads will use FTP server
[00:05:34.106 prepTelegram] Telegram not being used
[00:05:34.143 updateAppStatus] Enabling motion detection
[00:05:34.559 wgetFile] Downloading /data/common.js from /s60sc/ESP32-CAM_MJPEG2SD/master/data/common.js
[00:05:35.202 wgetFile] Downloaded /data/common.js, size 38896 bytes
[00:05:35.806 wgetFile] Downloading /data/MJPEG2SD.htm from /s60sc/ESP32-CAM_MJPEG2SD/master/data/MJPEG2SD.htm
[00:05:36.514 isNight] Night time
[00:05:36.968 wgetFile] Downloaded /data/MJPEG2SD.htm, size 92KB
[17:46:47.672 onWiFiEvent] WiFi AP client connection
[17:47:11.039 getLocalNTP] Using NTP server: pool.ntp.org
[17:47:11.039 showLos setAlarm] Alarm scheduled at 03/08/2024 01:00:00
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
[00:05:30.435 on ESP-CAM_MJPEG_90B3DB7CE5A4 started, use 'http://192.168.4.1' to connect
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
```

***2 запуск:***

```
[00:00:00.677 listFolder] Sketch size 1.7MB
[00:00:01.028 listFolder] File: /data/configs.txt, size: 6761 bytes
[00:00:01.030 listFolder] File: /data/common.js, size: 46192 bytes
[00:00:01.033 listFolder] File: /data/MJPEG2SD.htm, size: 97KB
[00:00:01.044 listFolder] SD_MMC: 76.2MB used of 14.7GB
[00:00:01.044 loadConfig] Load config
[00:00:01.289 updateAppStatus] Enabling motion detection
[00:00:01.296 setup] PSRAM size: 4.0MB
[00:00:01.489 prepCam] Camera init OK for model OV2640 on board CAMERA_MODEL_AI_THINKER
[00:00:01.805 setWifiSTA] Wifi Station IP from DHCP
.[00:00:01.952 onWiFiEvent] WiFi Station connection to TP-Link_B394, using hostname: ESP-CAM_MJPEG_90B3DB7CE5A4
.[00:00:02.489 onWiFiEvent] Wifi Station IP, use 'http://192.168.0.105' to connect
[00:00:02.834 onWiFiEvent] Wifi AP SSID: ESP-CAM_MJPEG_90B3DB7CE5A4 started, use 'http://192.168.4.1' to connect
[00:00:03.535 onWiFiEvent] Station interface V6 IP addr is preferred
[00:00:06.457 startWifi] Wifi stats for TP-Link_B394 - signal strength: -49 dBm; Encryption: WPA2_PSK; channel: 11
[00:00:06.459 startPing] Started ping monitoring - On
[00:00:06.475 startWebServer] Starting web server on port: 80
[00:00:06.475 startWebServer] Remote server certificates not checked
[00:00:06.476 resetWatchDog] WatchDog started using task: ping
[00:00:06.486 getLocalNTP] Using NTP server: pool.ntp.org

[00:00:06.487 WARN getLocalNTP] Not yet synced with NTP 
[00:00:06.487 startSustainTasks] Started 1 HTTP sustain tasks
[00:00:06.498 prepUpload] File uploads will use FTP server
[00:00:06.508 prepTelegram] Telegram not being used
[00:00:06.546 updateAppStatus] Enabling motion detection
[00:00:06.695 prepRecording] To record new AVI, do one of:
[00:00:06.695 prepRecording] - press Start Recording on web page
[00:00:06.695 prepRecording] - move in front of camera
 
[00:00:06.706 prepRecording] Camera model OV2640 on board CAMERA_MODEL_AI_THINKER ready @ 20MHz
[00:00:06.716 checkMemory]  Free: heap 91432, block: 65524, min: 91420, pSRAM 1485952
[00:00:06.716 loop] =============== Total tasks: 17 ===============

[06:49:34.698 getLocalNTP] Using NTP server: pool.ntp.org
[06:49:34.699 showLocalTime] Got current time from NTP: 05/02/2025 06:49:34 with tz: GMT0
[06:50:04.478 setAlarm] Alarm scheduled at 06/02/2025 01:00:00
[06:50:05.614 getExtIP] External IP: 185.90.100.110
[06:50:05.616 doAppPing] Daily rollover
[06:58:48.351 onWiFiEvent] WiFi AP client connection
[06:58:51.357 WARN sendChunks] Failed to send /data/MJPEG2SD.htm to browser 
[06:59:12.856 wsHandler] Websocket connection: 53
[06:59:49.202 processFrame] Capture started by Motion  
.................................................. 
[06:59:55.240 closeAvi] ******** AVI recording stats ********
[06:59:55.241 closeAvi] Recorded /20250205/20250205_065949_SVGA_20_6.avi 
[06:59:55.251 closeAvi] AVI duration: 6 secs
[06:59:55.251 closeAvi] Number of frames: 120
[06:59:55.251 closeAvi] Required FPS: 20
[06:59:55.262 closeAvi] Actual FPS: 20.0
[06:59:55.262 closeAvi] File size: 1.7MB
[06:59:55.262 closeAvi] Average frame length: 14855 bytes
[06:59:55.273 closeAvi] Average frame monitoring time: 2 ms
[06:59:55.273 closeAvi] Average frame buffering time: 0 ms
[06:59:55.283 closeAvi] Average frame storage time: 9 ms
[06:59:55.283 closeAvi] Average SD write speed: 1458 kB/s
[06:59:55.293 closeAvi] File open / completion times: 10 ms / 41 ms
[06:59:55.294 closeAvi] Busy: 25%
[06:59:55.294 checkMemory]  Free: heap 96696, block: 65524, min: 50136, pSRAM 1027884
[06:59:55.304 closeAvi] *************************************
[06:59:55.315 checkFreeStorage] Storage free space: 14.7GB
[07:00:33.201 isNight] Night time
[07:00:35.199 isNight] Day time
```




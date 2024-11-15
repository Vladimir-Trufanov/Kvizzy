// kakoj-obem-operativnoj-pamyati-dostupen.ino

// 2024-07-20
// 
// Payment:                              "Al Thinker ESP32-CAM"
// CPU Frequency:                        "240MHz (WiFi/BT)"
// Flash Frequency:                      "80MHz"
// Flash Mode:                           "QIO"
// Partition Scheme:                     "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
// Core Debug Level:                     "Verbose"
// Erase All Flash Before Sketch Upload: "Enabled"
// Port:                                 "COM5"

// Additional links for the Board Manager: https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json

// Менеджер плат:         esp32 by Espressif Systems 3.0.1 installed
// Платы в данном пакете: ESP32 Dev Board, ESP32-S2 Dev Board, ESP32-S3 Dev Board, ESP32-C3 Dev Board, Arduino Nano ESP32.

/*
Скетч использует 271993 байт (13%) памяти устройства. Всего доступно 1966080 байт.
Глобальные переменные используют 17080 байт (5%) динамической памяти, оставляя 310600 байт для локальных переменных. Максимум: 327680 байт.
esptool.py v4.6
Serial port COM5
Connecting....
Chip is ESP32-D0WD (revision v1.0)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: 10:52:1c:75:c3:90
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Erasing flash (this may take a while)...
Chip erase completed successfully in 8.4s
Compressed 19744 bytes to 13604...
Writing at 0x00001000... (100 %)
Wrote 19744 bytes (13604 compressed) at 0x00001000 in 0.5 seconds (effective 305.0 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 146...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (146 compressed) at 0x00008000 in 0.0 seconds (effective 524.1 kbit/s)...
Hash of data verified.
Compressed 8192 bytes to 47...
Writing at 0x0000e000... (100 %)
Wrote 8192 bytes (47 compressed) at 0x0000e000 in 0.1 seconds (effective 837.1 kbit/s)...
Hash of data verified.
Compressed 272352 bytes to 149579...
Writing at 0x00010000... (10 %)
Writing at 0x0001bfa9... (20 %)
Writing at 0x00024616... (30 %)
Writing at 0x00029875... (40 %)
Writing at 0x0002ee49... (50 %)
Writing at 0x000344f4... (60 %)
Writing at 0x0003da74... (70 %)
Writing at 0x00046c54... (80 %)
Writing at 0x0004c410... (90 %)
Writing at 0x00051ac1... (100 %)
Wrote 272352 bytes (149579 compressed) at 0x00010000 in 3.6 seconds (effective 608.7 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
*/

#include <Arduino.h>

void setup() 
{
  /*
  log_d("-> Total  heap:    %d", ESP.getHeapSize());
  log_d("-> Free   heap:    %d", ESP.getFreeHeap());
  log_d("-> Total  PSRAM:   %d", ESP.getPsramSize());
  log_d("-> Free   PSRAM:   %d", ESP.getFreePsram());
  */
}

void loop() {}

/*

Протокол работы на COM5:

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

[     1][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x400d3a88
[    12][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x400d3a58
[    26][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x400d3a28
[    39][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x400d39f8
[    53][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x400d3a88
[    66][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x400d3a58
[    79][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x400d3a28
[    93][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x400d39f8
[   106][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x400d3a88
[   120][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x400d3a58
[   133][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x400d3a28
[   147][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x400d39f8
[   162][D][esp32-hal-cpu.c:264] setCpuFrequencyMhz(): PLL: 480 / 2 = 240 Mhz, APB: 80000000 Hz
[   614][I][esp32-hal-psram.c:90] psramInit(): PSRAM enabled
[   637][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 3 successfully set to type UART_RX (2) with bus 0x3ffbdb60
[   648][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 1 successfully set to type UART_TX (3) with bus 0x3ffbdb60
=========== Before Setup Start ===========
Chip Info:
------------------------------------------
  Model             : ESP32
  Package           : D0WD-Q5
  Revision          : 100
  Cores             : 2
  Frequency         : 240 MHz
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : Yes
  BT Low Energy     : Yes
  IEEE 802.15.4     : No
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   386756 B ( 377.7 KB)
  Free Bytes        :   356672 B ( 348.3 KB)
  Allocated Bytes   :    23016 B (  22.5 KB)
  Minimum Free Bytes:   351020 B ( 342.8 KB)
  Largest Free Block:   118772 B ( 116.0 KB)
------------------------------------------
SPIRAM Memory Info:
------------------------------------------
  Total Size        :  4194304 B (4096.0 KB)
  Free Bytes        :  4192124 B (4093.9 KB)
  Allocated Bytes   :        0 B (   0.0 KB)
  Minimum Free Bytes:  4192124 B (4093.9 KB)
  Largest Free Block:  4128756 B (4032.0 KB)
  Bus Mode          : QSPI
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         :  4194304 B (4 MB)
  Block Size        :    65536 B (  64.0 KB)
  Sector Size       :     4096 B (   4.0 KB)
  Page Size         :      256 B (   0.2 KB)
  Bus Speed         : 80 MHz
  Bus Mode          : QIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1920.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x001F0000, size:  1920.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x003D0000, size:   128.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Software Info:
------------------------------------------
  Compile Date/Time : Jul 20 2024 08:40:44
  Compile Host OS   : windows
  ESP-IDF Version   : v5.1.4-51-g442a798083-dirty
  Arduino Version   : 3.0.1
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : ESP32_DEV
  Arduino Variant   : esp32
  Arduino FQBN      : esp32:esp32:esp32cam:CPUFreq=240,FlashMode=qio,PartitionScheme=min_spiffs,FlashFreq=80,DebugLevel=verbose,EraseFlash=all
============ Before Setup End ============
[  1195][D][kakoj-obem-operativnoj-pamyati-dostupen.ino:70] setup(): Total heap: 386756
[  1203][D][kakoj-obem-operativnoj-pamyati-dostupen.ino:71] setup(): Free heap: 356320
[  1213][D][kakoj-obem-operativnoj-pamyati-dostupen.ino:72] setup(): Total PSRAM: 4194304
[  1223][D][kakoj-obem-operativnoj-pamyati-dostupen.ino:73] setup(): Free PSRAM: 4192124
=========== After Setup Start ============
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   386756 B ( 377.7 KB)
  Free Bytes        :   356320 B ( 348.0 KB)
  Allocated Bytes   :    23272 B (  22.7 KB)
  Minimum Free Bytes:   350648 B ( 342.4 KB)
  Largest Free Block:   118772 B ( 116.0 KB)
------------------------------------------
SPIRAM Memory Info:
------------------------------------------
  Total Size        :  4194304 B (4096.0 KB)
  Free Bytes        :  4192124 B (4093.9 KB)
  Allocated Bytes   :        0 B (   0.0 KB)
  Minimum Free Bytes:  4192124 B (4093.9 KB)
  Largest Free Block:  4128756 B (4032.0 KB)
------------------------------------------
GPIO Info:
------------------------------------------
  GPIO : BUS_TYPE[bus/unit][chan]
  --------------------------------------  
     1 : UART_TX[0]
     3 : UART_RX[0]
============ After Setup End =============
 
*/

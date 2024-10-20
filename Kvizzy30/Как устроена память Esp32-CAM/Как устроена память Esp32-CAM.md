## [Как устроена память Esp32-CAM](#)


#### [RAM - распределение памяти в ESP32](https://kotyara12.ru/iot/esp32_memory/) 

#### [FLASH - настройка таблицы разделов для ESP32](https://kotyara12.ru/iot/esp32_flash/)

#### [NVS - энергонезависимая библиотека хранения параметров](https://kotyara12.ru/iot/esp32_nvs/)

### Библиография

#### [ESP32 Programmers’ Memory Model](https://developer.espressif.com/blog/esp32-programmers-memory-model/)

#### [Создаем задачу FreeRTOS: динамический и статический способ](https://kotyara12.ru/iot/esp32_tasks/)

#### [Типы памяти стабильной версии Esp32 - v5.3.1](https://docs.espressif.com/projects/esp-idf/en/v5.3.1/esp32/api-guides/memory-types.html)

#### [ESP32: устройство системы и памяти](https://microsin.net/programming/arm/esp32-system-and-memory.html)

Примечание: здесь приведен перевод раздела "System and Memory" технического руководства [esp32_technical_reference_manua.pdf](#). Незнакомые термины и сокращения см. в Словарике, в конце статьи.

#### [ESP32 Technical Reference Manual Version 5.2](esp32_technical_reference_manual_en.pdf)

#### [Словарик](#)

```
AHB Advanced High-speed Bus   - продвинутая высокоскоростная шина.

APB Advanced Peripheral Bus   - продвинутая шина периферийных устройств.

DMA                           - прямой доступ к памяти, где центральный процессор не участвует.

DPORT                         - порт шины данных.

CPU Central Processor Unit     - вычислительное ядро.

ECO Engineering Change Orders  - указания по исправлению ошибок кристалла (в зависимости от версии).

IO                             - ввод/вывод.

MMU Memory Management Unit     - блок управления памятью.

MUX                            - мультиплексор.

PID                            - пропорциональный интегрально-дифференциальный (ПИД) регулятор.

PSRAM                          - псевдостатическая оперативная память.

ROM Read-Only Memory           - память только для чтения, ПЗУ.

RTC Real Tile Clock            - часы реального времени.

SRAM                           - внутреннее ОЗУ с ускоренным доступом.

ULP ultra-low-power processor  - процессор со сверхнизким потреблением Deep Sleep.
```
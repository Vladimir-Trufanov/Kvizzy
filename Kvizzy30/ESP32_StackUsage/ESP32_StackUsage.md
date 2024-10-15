## [ESP32_StackUsage 5.3.1 - ](#)

ESP32 - это автономная электронная плата на двух процессорах от Espressif, выполняющая функции целого устройства (контроллера - SoC) и размещённая на одной интегральной схеме с установленной многозадачной операционной системой FreeRTOS. 

### [Using FreeRTOS with ESP32 and Arduino](https://wolles-elektronikkiste.de/en/using-freertos-with-esp32-and-arduino)

#### Определение требований к объему памяти для задачи FreeRTOS

В следующем скетче вычисляется объем памяти (стека), требующийся для задачи. Так как скетч подготовлен для ESP32-CAM, то для эксперимента используется контрольный светодиод и вспышка со своими штатными контактами 4 и 33.

При объявлении задач резервируется достаточный объем памяти (но различный для каждой задачи для пущей убедительности), а затем используется функция uxTaskGetStackHighWaterMark() для запроса оставшейся свободной памяти в зарезервированной области. 

Чтобы сообщить функции, uxTaskGetStackHighWaterMark() по какой задаче мы хотим определить свободную память, мы передаем ей дескриптор этой задачи (дескрипторы задач были определены ранее с помощью TaskHandle_t name и назначаете своим задачам в xTaskCreate().

> Следует заметить, что при реализации запроса  о требованиях к памяти внутри исследуемой задачи, это привело бы к неточному результату. Поэтому запрос по памяти выделен в отдельную задачу.

!!! В представленном скетче так же выводится в последовательный порт наименование задачи  с помощью функции pcTaskGetName().

При достаточном уменьшении размера стека вызывается перезапуск контроллера со следующей ошибкой:

```
Guru Meditation Error: Core  0 panic'ed (Unhandled debug exception). 
Debug exception reason: Stack canary watchpoint triggered (Blink 1) 
```

#### [ Скетч "Вычислить объем памяти (стека), требующийся для задачи"](ex5-1-33_high-watermark/ex5-1-33_high-watermark.ino)

### Библиография

#### [1. Использование памяти в ESP32](https://dzen.ru/a/YylKJGqX2lCeJ56r)

#### [2. FreeRTOS: использование стека и проверка стека на переполнение](https://microsin.net/programming/arm-troubleshooting-faq/freertos-stack-usage-and-stack-overflow-checking.html)

#### [3. uxTaskGetStackHighWaterMark, uxTaskGetStackHighWaterMark2](https://freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark)

#### [4. Customization](https://freertos.org/Documentation/02-Kernel/03-Supported-devices/02-Customization)


#### [FreeRTOS — возможности трассировки](https://kit-e.ru/freertos-10/)https://microsin.net/programming/arm/freertos-part6.html

#### [FreeRTOS: практическое применение, часть 6 (устранение проблем)](https://microsin.net/programming/arm/freertos-part6.html)


###### [в начало](#kvizzy)

0

1

2

3

4

5

6

7

8

9

0

1

2

3

4

5





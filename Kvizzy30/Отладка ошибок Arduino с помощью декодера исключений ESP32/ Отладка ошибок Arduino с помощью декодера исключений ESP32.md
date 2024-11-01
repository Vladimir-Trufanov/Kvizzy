## [ Отладка ошибок Arduino с помощью декодера исключений ESP32](https://www.donskytech.com/esp32-exception-decoder/)

#### [Debugging Arduino Errors with the ESP32 Exception Decoder - Отладка ошибок Arduino с помощью декодера исключений ESP32 ](https://www.donskytech.com/esp32-exception-decoder/)

#### Что такое ошибка Guru Meditation: Core 1 panic'ed…

Ошибка Guru Meditation, исходящая от вашего ESP32, является общей ошибкой, причины ее разнообразны и могут зависеть от конкретной схемы / программы. Обычно он показывает вам общую причину ошибки, некоторые дампы регистров и объекты обратной трассировки.

```
Guru Meditation Error: Core 1 panic'ed (LoadProhibited). Exception was unhandled.
Core 1 register dump:
PC : 0x400014e8 PS : 0x00060830 A0 : 0x800d133c A1 : 0x3ffb96d0
A2 : 0xa341aa26 A3 : 0xa341aa24 A4 : 0x000000ff A5 : 0x0000ff00
A6 : 0x00ff0000 A7 : 0xff000000 A8 : 0x00000000 A9 : 0x00000001
A10 : 0x00000001 A11 : 0x00060823 A12 : 0x00060820 A13 : 0x00000020
A14 : 0x00000000 A15 : 0x00000000 SAR : 0x0000000a EXCCAUSE: 0x0000001c
EXCVADDR: 0xa341aa24 LBEG : 0x4000c2e0 LEND : 0x4000c2f6 LCOUNT : 0xffffffff

ELF file SHA256: 0000000000000000

Backtrace: 0x400014e8:0x3ffb96d0 0x400d1339:0x3ffb96e0 0x400d136d:0x3ffb9700 0x400d0509:0x3ffb9720 0x40089792:0x3ffb9750
```

Глядя на журналы терминала, можете ли вы догадаться, в чём именно заключается ошибка? Думаю, если бы вы были героем из вселенной Marvel, то смогли бы сделать эту ошибку понятной для человека. Процесс преобразования этих значений в понятный человеку язык называется «декодированием», и мы будем использовать инструмент под названием [EspExceptionDecoder](https://github.com/me-no-dev/EspExceptionDecoder) в Arduino.

> Прежде чем мы продолжим, позвольте мне напомнить вам, что на данный момент (апрель 2023 г.) поддержка инструментов и плагинов в Arduino IDE 2 по-прежнему невозможна. Декодер EspException — это инструмент, написанный на Java, и плагин, который мы можем использовать для декодирования исключения. Поскольку плагины по-прежнему не поддерживаются в Arduino IDE 2, вы можете следовать этой статье, скачав старую (устаревшую) версию Arduino IDE 1.8.19 здесь.
 
#### Имитация ошибки ESP32

Давайте создадим очень простую программу Arduino, которая будет выдавать исключение во время выполнения.

```
#include<Arduino.h>

bool throwError = true;
int dynamicValue;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  delay(5000);
  if (throwError)
    dynamicValue = 0;
  else
    dynamicValue = 1;

  float error = 1 / dynamicValue;
  Serial.println(error);
  
}
```

В функции loop() Здесь мы моделируем некоторую логику, но в итоге мы делим 1 на ноль, что невозможно и приведёт к исключению во время выполнения.

К счастью, у нас есть инструмент, который может помочь нам проанализировать проблему, и я покажу вам, как это сделать в Arduino IDE.

#### Как установить декодер исключений ESPException в Arduino IDE?


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





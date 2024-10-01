## [KlimovHelp - по мотивам сайта Александра Климова](#) 

### [Пингуем сервер: ESP32Ping](https://github.com/marian-craciunescu/ESP32Ping)

***2024-10-01 Не пингуется!***

***www.kwinflat.ru пингуется!***

> Note that this is a port from [https://github.com/dancol90/ESP8266Ping](https://github.com/dancol90/ESP8266Ping) With this library an ESP32Ping can ping a remote machine and know if it's reachable. It provide some basic measurements on ping messages (avg response time).
> 

Сначала включите библиотеку в свой эскиз вместе с библиотекой WiFi:
```
#include <WiFi.h>
#include <ESP32Ping.h>
```

Далее просто вызовите Ping.ping() функцию

```
IPAddress ip (192, 168, 0, 1); // The remote ip to ping
bool ret = Ping.ping(ip);
```

ret будет true, если удаленный сервер ответил на запросы, false, если недоступен. Библиотека также поддерживает имя хоста, просто передайте строку вместо ip-адреса:

```
bool ret = Ping.ping("www.google.com");
```

Кроме того, функция принимает второй целочисленный параметр count, который указывает, сколько pings должно быть отправлено:

```
bool ret = Ping.ping(ip_or_host, 10);
```

После вызова Ping.ping() среднее время отклика (в миллисекундах) можно получить с помощью

```
float avg_time_ms = Ping.averageTime();
```

### [NTP-сервис точного времени](https://developer.alexanderklimov.ru/arduino/m5stack/ntp.php)

С помощью библиотеки NTPClient (доступен через менеджер библиотек) мы можем соединиться с NTP-сервисом точного времени и узнать время.

Данные выводятся на экран и в монитор порта, поэтому можно использовать пример и на голой ESP32, убрав лишние зависимости. В скетче я установил смещение на 3 часа, так как время берётся по Гринвичу. Возможно вам придётся изменить это значение.

Данные обновляются раз в секунду. На самом деле не стоит так часто опрашивать сервер, узнавайте время только когда это необходимо.

> ***Узнать точное время => Find out the exact time => FindExactTime
> ***

### [Узнаём погоду через OpenWeatherMap](https://developer.alexanderklimov.ru/arduino/esp32/webclient.php#weather)

#### [API текущих данных о погоде](https://openweathermap.org/current)

> ***Узнать погоду => find out the weather => FindWeather


```
$url =
  'https://api.openweathermap.org/data/2.5/weather?'.$dacha.'&appid='.$appid;

// Указываем координаты дачи в Лососинном (по данным OpenWeatherMap в координатах следует задавать 4 знака после точки)

$dacha='lat=61.7019&lon=34.1545'; // 61.701941,34.154539

// Указываем API-key
$appid='080b7a11c05216a4b86317b92370b484';

// Назначаем URL о погоде по координатам
// https://api.openweathermap.org/data/2.5/weather?lat=61.701941&lon=34.154539&appid=080b7a11c05216a4b86317b92370b484
                                                               080b7a11c05216a4b86317b92370b484

https://api.openweathermap.org/data/3.0/onecall?lat=33.44&lon=-94.04&exclude=hourly,daily&appid=080b7a11c05216a4b86317b92370b484

https://api.openweathermap.org/data/2.5/weather?lat=44.34&lon=10.99&appid=080b7a11c05216a4b86317b92370b484

// это Вилга
https://api.openweathermap.org/data/2.5/weather?lat=61.70&lon=34.15&appid=080b7a11c05216a4b86317b92370b484

{
    "coord": {
        "lon": 34.1545,
        "lat": 61.7019
    },
    "weather": [
        {
            "id": 803,
            "main": "Clouds",
            "description": "broken clouds",
            "icon": "04n"
        }
    ],
    "base": "stations",
    "main": {
        "temp": 277.85,
        "feels_like": 275.04,
        "temp_min": 277.85,
        "temp_max": 277.85,
        "pressure": 1016,
        "humidity": 93,
        "sea_level": 1016,
        "grnd_level": 995
    },
    "visibility": 10000,
    "wind": {
        "speed": 3.36,
        "deg": 244,
        "gust": 10.24
    },
    "clouds": {
        "all": 80
    },
    "dt": 1727631451,
    "sys": {
        "type": 1,
        "id": 8929,
        "country": "RU",
        "sunrise": 1727581526,
        "sunset": 1727623302
    },
    "timezone": 10800,
    "id": 473661,
    "name": "Vilga",
    "cod": 200
}

// !!! Следует давать 4 знака после запятой
https://api.openweathermap.org/data/2.5/weather?lat=61.7019&lon=34.1545&appid=080b7a11c05216a4b86317b92370b484

```

### [Библиотека ArduinoJson](https://developer.alexanderklimov.ru/arduino/libraries/arduinojson.php)


### Библиография

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





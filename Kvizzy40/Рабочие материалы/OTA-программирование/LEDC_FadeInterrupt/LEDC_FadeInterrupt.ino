/** Arduino, Esp32-CAM ***************************** LEDC_FadeInterrupt.ino ***
 * 
 *  https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html
 *
 *  --------
 * 
**/
/* LEDC Fade Arduino Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

// use 12 bit precision for LEDC timer
#define LEDC_TIMER_12_BIT 12

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ 5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN 4

// Определяем начальный режим, целевой режим и максимальное время затухания
#define LEDC_START_DUTY  (0)
#define LEDC_TARGET_DUTY (4095)
#define LEDC_FADE_TIME   (1000)

bool fade_ended = false;  // status of LED fade
bool fade_in = true;

void ARDUINO_ISR_ATTR LED_FADE_ISR() 
{
  fade_ended = true;
}

void setup() 
{
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  // Устанавливаем таймер с заданной частотой и разрешением и прикрепляем его 
  // к светодиодному контакту с автоматически выбранным каналом
  // bool ledcAttach(uint8_t pin, uint32_t freq, uint8_t resolution);
  // pin - вывод LEDC, freq - частота шим, resolution - разрешение для канала LEDC, диапазон составляет 1-14 бит (1-20 бит для ESP32);
  // функция вернёт true в случае успешной настройки. Если возвращается false, значит произошла ошибка и канал LEDC не был настроен.
  ledcAttach(LED_PIN, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);

  // Настраиваем и включаем режим затухания светодиода (режим от 0 до 4095)
  // bool ledcFade(uint8_t pin, uint32_t start_duty, uint32_t target_duty, int max_fade_time_ms);
  // pin - вывод LEDC, start_duty - выберите начальный режим затухания (частота), 
  // target_duty - целевой режим затухания (частота), max_fade_time_ms - максимальное время затухания;
  // функция вернет true если конфигурация выполнена успешно. Если false - возниклат ошибка 
  // и затухание LEDC не было настроено / запущено.
  ledcFade(LED_PIN, LEDC_START_DUTY, LEDC_TARGET_DUTY, LEDC_FADE_TIME);
  Serial.println("Затухание LED стартовало в Setup.");

  // Wait for fade to end
  delay(LEDC_FADE_TIME);

  // Setup and start fade off led and use ISR (duty from 4095 to 0)
  // Устанавливаем и запускаем режим затухания светодиода, используя ISR (режим от 4095 до 0)
  // bool ledcFadeWithInterrupt(uint8_t pin, uint32_t start_duty, uint32_t target_duty, int max_fade_time_ms, void (*userFunc)(void));
  ledcFadeWithInterrupt(LED_PIN, LEDC_TARGET_DUTY, LEDC_START_DUTY, LEDC_FADE_TIME, LED_FADE_ISR);
  Serial.println("Затухание LED завершилось. Запущено прерывание.");
}

void loop() 
{
  // Check if fade_ended flag was set to true in ISR
  if (fade_ended) 
  {
    Serial.println("Прерывание !!!");
    fade_ended = false;

    // Check what fade should be started next
    if (fade_in) 
    {
      ledcFadeWithInterrupt(LED_PIN, LEDC_START_DUTY, LEDC_TARGET_DUTY, LEDC_FADE_TIME, LED_FADE_ISR);
      //Serial.println("LED Fade in started.");
      fade_in = false;
    } 
    else 
    {
      ledcFadeWithInterrupt(LED_PIN, LEDC_TARGET_DUTY, LEDC_START_DUTY, LEDC_FADE_TIME, LED_FADE_ISR);
      //Serial.println("LED Fade out started.");
      fade_in = true;
    }
  }
}

// ************************************************* LEDC_FadeInterrupt.ino ***

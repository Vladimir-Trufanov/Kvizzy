// Проверка WDT ProverkaWDT.ino

#include <GyverWDT.h>
/*
  Пример тестирующий поддержку всех функций watchdog на вашем устройстве
  > После 10 секунд отсчета программа стартует заного -> поддерживаются все функции
  > После таймаута устройство зависает, светодиод на D13 начинает мигать -> поддерживается только INTERRUPT_MODE
  В случае bootloop у вас будет 10 секунд на перепрошивку устройства после подачи питания
  Для добавления поддержки всего функционала watchdog загрузите optiboot или откажитесь от загрузчика
*/
void setup() 
{
  Serial.begin(9600);
  Serial.println("Program started , wait 10 seconds");
  delay(10000);                                             // 10 секунд на перепрошивку в случае bootloop
  
  Watchdog.enable(RESET_MODE, WDT_PRESCALER_1024);          // Режим сторожевого сброса , таймаут ~8с
  Serial.println("watchdog enabled");
  
  while (1) {                                               // Бесконечный цикл , эмуляция "зависания"
    if (!(millis() % 1000)) {                               // Каждую секунду
      Serial.println((uint16_t)((millis() / 1000) - 10));   // Вывести время после включения watchdog в секундах
      delay(10);
    }
  }
}
void loop() {}

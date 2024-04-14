/*  1-knopka-svetodiod.ino  */

const int PinKey = 2;  // вывод с кнопкой
const int PinLed = 10; // вывод со светодиодом
bool      ActLed = 0;  // состояние светодиода

void setup()
{
   //  Конфигурируем вывод PinKey как вход
   pinMode(PinKey, INPUT );
   //  Конфигурируем вывод PinLed как выход
   pinMode(PinLed, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
   //  Если кнопка нажата (единица на входе PinKey)
   if(digitalRead(PinKey))
   {
      // Меняем значение переменной ActLed
      ActLed = !ActLed; // ActLed = не ActLed
      // Устанавл. состояние ActLed на выходе PinLed
      digitalWrite(PinLed, ActLed);
      digitalWrite(LED_BUILTIN, ActLed);
      // Ждём пока кнопка не будет отпущена ...
      // Код в цикле while(условие) будет выполнять-
      // ся до тех пор, пока условие не станет ложью
      while (digitalRead(PinKey))
      {
         // Так как в условии цикла while считыва-
         // ется состояние кнопки, то пока она 
         // нажата, будет выполняться функция delay4
         delay(30);
         // функция delay(30) создаёт паузу на 30мс 
         // это позволяет подавить эффект дребезга.
      }
   }
}

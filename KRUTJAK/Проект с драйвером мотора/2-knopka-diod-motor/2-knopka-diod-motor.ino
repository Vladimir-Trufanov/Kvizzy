/*  2-knopka-diod-motor.ino  */

const int PinKey = 7;  // вывод с кнопкой
const int PinLed = 10; // вывод со светодиодом
bool      ActLed = 0;  // состояние светодиода

// Определяем выводы подключения 2 - направление, 3 - скорость
#define DIR_PIN 2
#define PWM_PIN 3

// Создаём переменную направления
bool dir = false;


void setup()
{
   //  Конфигурируем вывод PinKey как вход
   pinMode(PinKey, INPUT );
   //  Конфигурируем вывод PinLed как выход
   pinMode(PinLed, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT);
   
   // Устанавливаем выводы в режиме выходов
   pinMode(DIR_PIN, OUTPUT);
   pinMode(PWM_PIN, OUTPUT);
}

void loop()
{
   
   ActLed=isButton(ActLed);
   if (ActLed==true) 
   {
      digitalWrite(LED_BUILTIN,true);
      
      //iloop();
          // Устанавливаем ШИМ
    analogWrite(PWM_PIN, 255);
    // Устанавливаем направление, заданное переменной dir
    digitalWrite(DIR_PIN, dir);

      
   }
   else
   {
      digitalWrite(LED_BUILTIN,false);
      // Выключаем мотор
      analogWrite(PWM_PIN, 0);
   }
   
}

// Нажата ли кнопка
bool isButton(bool ActLed)
{
   //  Если кнопка нажата (единица на входе PinKey)
   if(digitalRead(PinKey))
   {
      // Меняем значение переменной ActLed
      ActLed = !ActLed; // ActLed = не ActLed
      // Устанавл. состояние ActLed на выходе PinLed
      digitalWrite(PinLed, ActLed);
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
   return ActLed;
}

void iloop()
{
    // Устанавливаем ШИМ
    analogWrite(PWM_PIN, 255);
    // Устанавливаем направление, заданное переменной dir
    digitalWrite(DIR_PIN, dir);
    // Ждём 2 секунды
    delay(2000);
    // Выключаем мотор
    analogWrite(PWM_PIN, 0);
    // Ждём секунду
    delay(1000);
    // Меняем направление
    dir = !dir;
}

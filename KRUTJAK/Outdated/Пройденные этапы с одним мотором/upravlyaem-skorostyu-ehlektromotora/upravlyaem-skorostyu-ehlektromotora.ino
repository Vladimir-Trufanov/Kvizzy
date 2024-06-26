/** Arduino UNO *************** *** upravlyaem-skorostyu-ehlektromotora.ino ***
 * 
 * Изучить простейшую схему управления скоростью вращения электромотора с помощью
 * переменного резистора с использованием ШИМ, широтно-импульсной модуляции.
 * 
 * В скетче используется полевой транзистор IRFZ24N. Это позволяет не менять 
 * мощности двигателя при изменении скорости. Скетч отслеживает положение потенциометра
 * и преобразовывает его в сигнал ШИМ, который с пина контроллера D10 попадает 
 * на затвор полевого транзистора. Транзистор работает в режиме ключа и выполняет 
 * роль драйвера, позволяя управлять двигателем, не нагружая выход Arduino.
 * 
 * Скорость вращения электромотора зависит от коэффициента заполнения ШИМ на 
 * выходе PinPWM. Коэффициент заполнения ШИМ устанавливается скетчем в зависимости
 * от уровня напряжения на входе PinRes. Уровень напряжения на входе PinRes 
 * зависит от угла поворота потенциометра.
 * 
 * Таким образом, поворотом ручки потенциометра изменяется скорость вращения электромотора.
 * 
 * v1.1, 12.03.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 10.03.2024
**/

const int PinRes = A0;  // аналоговый вход, к которому подключен потенциометр
const int PinPWM = 10;  // цифровой (ШИМ) выход, к которому подключен затвор полевого транзистора
      int ValRes = 0;   // считанное значение напряжения с аналогового контакта (0-1023)
      int ValPWM = 0;   // расчитанное значение ШИМ для отправки на затвор (0-255)

void setup() 
{
   pinMode(PinPWM, OUTPUT); // назначили, как выход, контакт 10 Arduino 
}

void loop() 
{
   ValRes = analogRead(PinRes);           // считали напряжение с потенциометра
   ValPWM = map(ValRes, 0,1023, 0,255);   // расчитали напряжение ШИМ для отправки на затвор транзистора
   analogWrite(PinPWM,ValPWM);            // установили скорость мотора в дискретах
}

// ******************************** upravlyaem-skorostyu-ehlektromotora.ino ***

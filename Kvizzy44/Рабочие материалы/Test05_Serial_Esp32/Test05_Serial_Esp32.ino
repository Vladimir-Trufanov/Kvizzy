/** Arduino C/C++ ********************************* Test05_Serial_Esp32.ino ***
 * 
 *       Выполнить проверку стандартного последовательного обмена с IDE Arduino
 *                                                          для платы ESP32-CAM 
 *                                               
 * v1.0, 01.09.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 01.09.2024
**/

// Инициируем прием кодов и заполнение строки
int incomingByte = 0; 
String data = "";

void setup() 
{
  Serial.begin(115200);             // opens serial port, sets data rate to 115200 bps
}

void loop() 
{
  // Проверяем, есть ли байты в последовательном порту
  if (Serial.available() > 0) 
  {
      // Читаем очередной байт
      incomingByte = Serial.read();
      // Получаем по коду символ
      char character = (char)incomingByte;
      // Выводим код полученного символа
      Serial.print("Получен: ");
      Serial.println(incomingByte);
      // Если не "перевод строки" то складываем
      // символ в формируемую строку
      if (character != '\n')
      {
         data.concat(character);
      }
      // Иначе выводим текст
      else
      {
         // Выводим текст в последовательный порт
         Serial.println(data);
         // Готовим прием нового текста
         data = "";
      }
  }
}
// ************************************************ Test05_Serial_Esp32.ino ***

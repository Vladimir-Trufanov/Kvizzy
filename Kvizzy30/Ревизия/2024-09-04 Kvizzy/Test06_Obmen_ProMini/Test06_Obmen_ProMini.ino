/** Arduino C/C++ ******************************** Test06_Obmen_ProMini.ino ***
 * 
 *                Выполнить проверку последовательного синхронного обмена между 
 *                 ESP32-CAM (Slave-партнер) и Arduino Pro Mini (Master-мастер) 
 *                                               
 * v1.0, 02.09.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 02.09.2024
 * 
 * Тест работает следующим образом:
 * 
 * 1. Мастер чистит обе строки дисплея и ожидает 30 секунд для подготовки 
 *    к работе партнера.
 * 
 * 2. Master отправляет команду-символ "#" партнеру, показывает команду в первой 
 *    строке дисплея и ждет ответа от партнера.
 *    
 * 3. Партнер всегда ждет команду от мастера. Получив команду-символ, возвращает 
 *    его мастеру и дублирует информацию в стандартный последовательный порт.
 *    
 * 4. Мастер, получив ответ от партнера, выводит его во второй строке дисплея и
 *    ждет 30 секунд.
 *    
 * 5. Master чистит экран дисплея  и отправляет следующую команду-символ "@" 
 *    партнеру, показывает команду в первой cтроке дисплея и ждет ответа от 
 *    партнера. Так обмен продолжается.
 *    
 * То есть мастер поочередно отправляет команды "#" или "@" партнеру, а партнер 
 * возвращает переданный символ.
 *    
 **/

// Подключаем библиотеку LCD_1602_RUS (наследницу LiquidCrystal_I2C.h)
#include <LCD_1602_RUS.h> 
// Создаём объект для LCD-дисплея
LCD_1602_RUS lcd(0x27,16,2);
// Создаем объект последовательного обмена и
// назначаем пины 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4,3); // RX=4,TX=3

// Инициируем переменную отправляемых команд
char Command;
// Инициируем переменные по приему кодов 
int incomingByte = 0; 
char character = '~';
String Text="";

// ****************************************************************************
// *                            Почистить строки дисплея                      *
// ****************************************************************************
void ClearMonitor()
{
   lcd.setCursor(0,0);               // установили курсор в начало 1 строки
   lcd.print("                ");    // почистили первую строку
   lcd.setCursor(0,1);               // установили курсор в начало 2 строки
   lcd.print("                ");    // почистили строку
}
// ****************************************************************************
// *                  Выполнить задержку перед следующей командой             *
// ****************************************************************************
void Delay30()
{
   for (int i = 0; i < 9; i++) 
   {
      lcd.setCursor(14,0);               
      lcd.print(i+1);  
      delay(1000);
   }
}
// ****************************************************************************
// *                          Отправить команду партнеру                      *
// ****************************************************************************
void SendComm(char Command)
{
   // Чистим экран, отправляем команду партнеру, 
   // показывает команду в первой строке дисплея
   ClearMonitor();          // почистили экран
   lcd.setCursor(0,0);      // установили курсор в начало 1 строки
   mySerial.print(Command); // отправили команду
   Text="";
   Text.concat(Command);
   lcd.print(Text);         // вывели команду на монитор
}

void setup() 
{
   lcd.init();              // проинициализировали lcd    
   lcd.backlight();         // включили подсветку
   ClearMonitor();          // почистили экран
   mySerial.begin(115200);  // инициализировали последовательный порт
   Delay30();
}

void loop() 
{
   // Чистим экран, отправляем команду '#' партнеру, показывает команду в первой 
   // строке дисплея и ждет ответа от партнера.
   Command='#';
   SendComm(Command);
      lcd.setCursor(0,1);               // установили курсор в начало 2 строки
      lcd.print("Ждем команду! #");         // вывели команду на монитор
   Delay30();
   // Чистим экран, отправляем команду '@' партнеру, показывает команду в первой 
   // строке дисплея и ждет ответа от партнера.
   Command='@';
   SendComm(Command);
      lcd.setCursor(0,1);               // установили курсор в начало 2 строки
      lcd.print("Ждем команду! @");         // вывели команду на монитор
   Delay30();
   
  if (mySerial.available() > 0) 
  {
      lcd.setCursor(0,1);               // установили курсор в начало 2 строки
      lcd.print("Команда пришла!");         // вывели команду на монитор
      // Читаем очередной байт
      incomingByte = mySerial.read();
      // Получаем по коду символ
      character = (char)incomingByte;

      lcd.setCursor(0,1);               // установили курсор в начало 2 строки
      Text="";
      Text.concat(character);
      lcd.print(Text);         // вывели команду на монитор
  }


  /*
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
         // Чистим экран дисплея и выводим текст в первой строке
         ClearMonitor();       // почистили экран
         lcd.setCursor(0,0);   // установили курсор в начало 1 строки
         lcd.print(data);      // вывели введенный текст
         // Готовим прием нового текста
         data = "";
      }
  }
  */
}
// *********************************************** Test06_Obmen_ProMini.ino ***

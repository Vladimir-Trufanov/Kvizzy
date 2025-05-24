#include <Regexp.h>

// Вызывается при каждом совпадении
void replace_callback (const char * match,                 // текущий фрагмент, который найден по соответствию
                       const unsigned int length,          // длина фрагмента
                       char * & replacement,               // текст, которым заменяем соответствия
                       unsigned int & replacement_length,  // длина замещающего текста
                       const MatchState & ms)              // хранилище найденных фрагментов
{
  // Показываем очередной найденный фрагмент
  Serial.print("Match = ");
  Serial.write((byte *) match, length);
  Serial.println (); 

  replacement = "Nick";
  replacement_length = 4;
}  

void setup ()
{
  Serial.begin (115200);
  Serial.println ();
  unsigned long count;

  // Загружаем в буфер текст, в котором будет осуществляться поиск
  char buf [100] = "The quick brown fox jumps over the lazy wolf";
  // Создаем объект поиска соответствий
  MatchState ms (buf);
  // Выводим исходное содержимое буфера
  Serial.print ("Начальная  строка: "); Serial.println (buf);
  // Формируем регулярное выражение ("соответствие")
  const char * match="%a+";
  // Выполняем поиск по соответствию
  count = ms.GlobalReplace (match, replace_callback);
  // Показываем результаты
  Serial.print ("Изменённая строка: "); Serial.println (buf);
  Serial.print ("Найдено "); Serial.print (count); Serial.println (" соответствий.");
  /*
  Начальная  строка: The quick brown fox jumps over the lazy wolf
  Match = The
  Match = quick
  Match = brown
  Match = fox
  Match = jumps
  Match = over
  Match = the
  Match = lazy
  Match = wolf
  Изменённая строка: Nick Nick Nick Nick Nick Nick Nick Nick Nick
  Найдено 9 соответствий.
  */
  
  // Записываем в буфер новый текст
  strcpy (buf, "But does it get goat's blood out?");
  Serial.println("");
  Serial.print ("Начальная  строка: "); Serial.println (buf);
  // Пересчитываем длину содержимого буфера
  ms.Target (buf);    
  // Делаем замены по соответствию из набора на *
  count = ms.GlobalReplace ("[aeiou]", "*");
  // Показываем результаты
  Serial.print ("Изменённая строка: "); Serial.println (buf);
  Serial.print ("Найдено "); Serial.print (count); Serial.println (" соответствий.");
  /*
  Начальная  строка: But does it get goat's blood out?
  Изменённая строка: B*t d**s *t g*t g**t's bl**d **t?
  Найдено 11 соответствий.
  */
}  

void loop () {}

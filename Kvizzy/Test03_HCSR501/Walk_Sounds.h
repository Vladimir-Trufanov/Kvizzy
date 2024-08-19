/** Arduino Pro Mini                                      *** Walk_Sounds.h ***
 * 
 * Библиотека звуковых вставок в приложение 
 * (на основе https://github.com/robsoncouto/arduino-songs)
 * 
 * v1.0, 19.08.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 19.08.2024
**/

#ifndef Walk_Sounds
#define Walk_Sounds
 
#pragma once            // обеспечили разовое подключение файла
#include <Arduino.h>    // подключили общие функции Arduino

// Первичные константы для частот, длительностей и темпов звукоизвлечения
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

// В сформированном списке мелодии 4 означает четвертную ноту, 8 - восьмую,
// 16 - шестнадцатую и так далее. Отрицательные числа используются для 
// обозначения "пунктирных" нот (нот с точкой), где -4 означает пунктирную 
// четвертную ноту, то есть четвертная плюс восьмая!



int Baby_Elephant_Walk[] = 
{

  // Baby Elephant Walk
  // Score available at https://musescore.com/user/7965776/scores/1862611
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_FS4,8, NOTE_G4,8, REST,4, REST,2,
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_G4,-2, NOTE_A4,8, NOTE_DS4,1,
  
  NOTE_A4,8,
  NOTE_E4,8, NOTE_C4,8, REST,4, REST,2,
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_FS4,8, NOTE_G4,8, REST,4, REST,4, REST,8, NOTE_G4,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_B4,8, NOTE_G4,8, REST,8, NOTE_G4,8,
   
  NOTE_C5,4, NOTE_C5,4, NOTE_AS4,16, NOTE_C5,16, NOTE_AS4,16, NOTE_G4,16, NOTE_F4,8, NOTE_DS4,8,
  NOTE_FS4,4, NOTE_FS4,4, NOTE_F4,16, NOTE_G4,16, NOTE_F4,16, NOTE_DS4,16, NOTE_C4,8, NOTE_G4,8,
  NOTE_AS4,8, NOTE_C5,8, REST,4, REST,2,
};

#define array_count sizeof(Baby_Elephant_Walk)/sizeof(Baby_Elephant_Walk[0])


/*
int melody[] = 
{

  // Baby Elephant Walk
  // Score available at https://musescore.com/user/7965776/scores/1862611
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_FS4,8, NOTE_G4,8, REST,4, REST,2,
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_G4,-2, NOTE_A4,8, NOTE_DS4,1,
  
  NOTE_A4,8,
  NOTE_E4,8, NOTE_C4,8, REST,4, REST,2,
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_FS4,8, NOTE_G4,8, REST,4, REST,4, REST,8, NOTE_G4,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_B4,8, NOTE_G4,8, REST,8, NOTE_G4,8,
   
  NOTE_C5,4, NOTE_C5,4, NOTE_AS4,16, NOTE_C5,16, NOTE_AS4,16, NOTE_G4,16, NOTE_F4,8, NOTE_DS4,8,
  NOTE_FS4,4, NOTE_FS4,4, NOTE_F4,16, NOTE_G4,16, NOTE_F4,16, NOTE_DS4,16, NOTE_C4,8, NOTE_G4,8,
  NOTE_AS4,8, NOTE_C5,8, REST,4, REST,2,
};
*/
// Определяем размер мелодии исходя из того, что каждое целое состоит из двух байт (16 бит), а
// на каждую ноту приходится два значения (высота тона и длительность), то есть
// четыре байта
//int notes = sizeof(melody) / sizeof(melody[0]) / 2;
// Определяем продолжительность всей ноты в мс
//int wholenote = (60000 * 4) / tempo;
// Инициируем позицию текущей ноты и длительность 
//int divider = 0, noteDuration = 0;

class WalkSounds 
{
   private:
      int tempo = 132;    // темп воспроизведения 
      int buzzer = 12;    // пин зуммера
      int notes;          // размер (количество байт) мелодии
      int divider;        // позиция текущей ноты
      int wholenote;      // темп мелодии
      int noteDuration;   // продолжительность ноты
      int melody;
   public:


/*
#define Baby_Elephant_Walk

int Baby_Elephant_Walk[] = 
{

  // Baby Elephant Walk
  // Score available at https://musescore.com/user/7965776/scores/1862611
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_FS4,8, NOTE_G4,8, REST,4, REST,2,
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_G4,-2, NOTE_A4,8, NOTE_DS4,1,
  
  NOTE_A4,8,
  NOTE_E4,8, NOTE_C4,8, REST,4, REST,2,
  NOTE_C4,-8, NOTE_E4,16, NOTE_G4,8, NOTE_C5,8, NOTE_E5,8, NOTE_D5,8, NOTE_C5,8, NOTE_A4,8,
  NOTE_FS4,8, NOTE_G4,8, REST,4, REST,4, REST,8, NOTE_G4,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_B4,8, NOTE_G4,8, REST,8, NOTE_G4,8,
   
  NOTE_C5,4, NOTE_C5,4, NOTE_AS4,16, NOTE_C5,16, NOTE_AS4,16, NOTE_G4,16, NOTE_F4,8, NOTE_DS4,8,
  NOTE_FS4,4, NOTE_FS4,4, NOTE_F4,16, NOTE_G4,16, NOTE_F4,16, NOTE_DS4,16, NOTE_C4,8, NOTE_G4,8,
  NOTE_AS4,8, NOTE_C5,8, REST,4, REST,2,
};
*/
      // Управлять скоростью мотора и направлением его вращения
      void Sound(); 
      // Управлять скоростью мотора и направлением его вращения
      void Soundi(int x[], int ssize); 
      // Протестировать мотор
      void Test(); 
};



/*
void WalkSounds() 
{
   // Перебираем ноты мелодии, помня что массив включает пары: ноты + длительности
   for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) 
   {
      // Вычисляем длительность текущей ноты
      divider = melody[thisNote + 1];
      if (divider > 0) 
      {
         noteDuration = (wholenote) / divider;
      } 
      else if (divider < 0) 
      {
         // Уточняем длительность для ноты с точкой
         noteDuration = (wholenote) / abs(divider);
         noteDuration *= 1.5; // increases the duration in half for dotted notes
      }
      // Воспроизводим ноту только в течение 90% ее продолжительности, оставляя 
      // 10% в качестве паузы
      tone(buzzer, melody[thisNote], noteDuration * 0.9);
      // Выдерживаем время продолжительности текущей ноты, прежде чем 
      // играть следующую ноту
      delay(noteDuration);
      // Останавливаем генерацию сигнала перед следующей нотой
      noTone(buzzer);
   }
}
*/
#endif

// ********************************************************** Walk_Sounds.h ***

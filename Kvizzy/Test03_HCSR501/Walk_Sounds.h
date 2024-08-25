/** Arduino Pro Mini                                      *** Walk_Sounds.h ***
 * 
 * Библиотека звуковых вставок в приложение 
 * (на основе https://github.com/robsoncouto/arduino-songs)
 * 
 * v1.2, 25.08.2024                                   Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 19.08.2024
**/

#ifndef Walk_Sounds
#define Walk_Sounds
 
#pragma once                // обеспечили разовое подключение файла
#include <Arduino.h>        // подключили общие функции Arduino

#include "Walk_Defines.h"   // подключили определения наборов мелодий

class WalkSounds 
{
   private:
      int buzzer;          // пин зуммера
      // Воспроизвести музыкальный фрагмент по указанному массиву звуков    
      // с заданным темпом                          
      void Soundi(int *Melody, int MelodySize, int OneSize, int tempo = 132);
 
   public:
      WalkSounds(int _buzzer = 11) 
      {
         buzzer = _buzzer;
      }

      // **********************************************************************
      // *    Воспроизвести музыкальные фрагменты по работе с пир-датчиком    *
      // **********************************************************************

      // Отрабатываем звуковую вставку "Успокаиваем PIR-датчик"
      #ifdef UspokaivaemPirDatchik
      void Sound_UspokaivaemPirDatchik(int tempo = 80)
      {
         Soundi(uspokaivaem_pir_datchik,sizeof(uspokaivaem_pir_datchik),sizeof(uspokaivaem_pir_datchik[0]),tempo);
      }
      #endif
      // Отрабатываем звуковую вставку "PIR-датчик готов"
      #ifdef PirDatchikGotov
      void Sound_PirDatchikGotov(int tempo = 144)
      {
         Soundi(pir_datchik_gotov,sizeof(pir_datchik_gotov),sizeof(pir_datchik_gotov[0]),tempo);
      }
      #endif
      // Отрабатываем звуковую вставку "Движение есть"
      #ifdef DvizhenieEst
      void Sound_DvizhenieEst(int tempo = 225)
      {
         Soundi(dvizhenie_est,sizeof(dvizhenie_est),sizeof(dvizhenie_est[0]),tempo);
      }
      #endif
      // Отрабатываем звуковую вставку "Ловим движение"
      #ifdef LovimDvizhenie
      void Sound_LovimDvizhenie(int tempo = 120)
      {
         Soundi(lovim_dvizhenie,sizeof(lovim_dvizhenie),sizeof(lovim_dvizhenie[0]),tempo);
      }
      #endif

      // **********************************************************************
      // *                 Воспроизвести предопределенные мелодии             *
      // **********************************************************************

      #ifdef Asabranca
      void Sound_Asabranca(int tempo = 120)
      {
         Soundi(Asa_branca,sizeof(Asa_branca),sizeof(Asa_branca[0]),tempo);
      }
      #endif

      #ifdef Greensleeves
      void Sound_Greensleeves(int tempo = 70)
      {
         Soundi(Greens_leeves,sizeof(Greens_leeves),sizeof(Greens_leeves[0]),tempo);
      }
      #endif

      // **********************************************************************
      // *          Обеспечить демонстрацию передачи массива в метод          *
      // *                 (подсчитать сумму элементов массива)               *
      // **********************************************************************
      #ifdef ExampleTransferArray
         int sumFunction(int *intArray, int arrSize)
         {
            // Определяем переменную для суммирования
            int sum = 0;  
            // Находим размер массива, разделив его вес
            // на вес одного элемента (тут у нас int)
            arrSize = arrSize / sizeof(int); 
            // Подсчитываем сумму 
            for (byte i = 0; i < arrSize; i++) 
            {
               sum += intArray[i];
            }
            return sum;
         }
      #endif
};

#endif

// ********************************************************** Walk_Sounds.h ***

/** Arduino, Esp32-CAM ********************************* Kvizzy30_Message.h ***
 * 
 *        Перечисление сообщений управления встроенными светодиодами Esp32-CAM,  
 *                                           контрольным и вспышкой, через сайт
 *                 и массив сообщений для заполнения буфера вывода на периферию
 * 
 * v1.0.0, 24.12.2024                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 24.12.2024
**/

#pragma once 

#include <QueMessage.h>  // заголовочный файл класса TQueMessage                      //

// Определяем источник сообщений  
#define tmk_APP "KVZ"    // пример по обработке очередей

// Определяем перечисления примера обработки очередей 
typedef enum {
   SendFromISR,          // 0 
   SendFromTask,         // 1 
   StructNoSend,         // 2 
   TaskNoQueue,          // 3
   SendLongMess,         // 4 maxi=255+0
}; 

// Формируем список сообщений приложения
tmessAPP amessAPP[] = 
{
    {SendFromISR,  tvm_1intmes, "Передано %s сообщение из прерывания"},
    {SendFromTask, tvm_2intmes, "Передано %s сообщение из задачи на %s миллисекунде"},
    {StructNoSend, tvm_1intmes, "Не удалось отправить структуру после %d тиков"},
    {TaskNoQueue,  tvm_simpmes, "Очереди структур нет в задаче"},
    {SendLongMess, tvm_simpmes, "Максимально длинное сообщение из 255 байт"},
};

// Вычисляем размер списка
int SizeMess=sizeof(amessAPP)/sizeof(tmessAPP);

// ***************************************************** Kvizzy30_Message.h ***
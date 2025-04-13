/** Arduino, Esp32-CAM ********************************* Kvizzy40_Message.h ***
 * 
 *        Перечисление сообщений управления встроенными светодиодами Esp32-CAM,  
 *                                           контрольным и вспышкой, через сайт
 *                 и массив сообщений для заполнения буфера вывода на периферию
 * 
 * v2.0.0, 13.04.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 24.12.2024
**/

#pragma once 

#include "QueMessage.h"    // заголовочный файл класса TQueMessage                   

// Определяем источник сообщений  
#define tmk_APP   "KVZ"    // Kvizzy
#define tmk_HTTP  "HTTP" 
#define tmk_Queue "EUE" 

// Определяем перечисления 
typedef enum {
   WifiEstablished,        // 0 
   SendFromISR,            // 1 
   SendFromTask,           // 2 
   NoSendISRled33,         // 3
   NoSend33_MODE0,         // 4
}; 
// Определяем перечисления ошибок http-запросов
#define tmk_HTTP "HTTP" 
typedef enum {http400=400, http403=403, http404=404, http405=405, http408=408, http500=500, http502=502, http504=504, http997=997}; 

// kviStream.readPhoto
#define tmk_readPhoto "RDF" 
typedef enum {rdf010=10, rdf011=11, rdf012=12}; 
#define rdfNotOpenPhotoFile      "Не удалось открыть файл фото для чтения"       // Couldn't open the photo file for reading
#define rdfSizePhotoLargeBuffer  "Размер файла фото:%s больше размера буфера:%s" // The size of the photo file is larger than the buffer size
#define rdfPhotoHasBeenRead      "Cчитан файл фото: размер %s"                   // The photo file has been read

// kviStream.writePhoto
#define tmk_writePhoto "WRF" 
typedef enum {wrf020=20, wrf021=21}; 
#define wrfErrOpenWritePhoto     "Ошибка открытия файла в режиме записи"         // Error opening a file in recording mode
char   *wrfSavedPhoto =          "Сохранено изображение [%s]: %s";

// Формируем список сообщений приложения
tmessAPP amessAPP[] = 
{
    {WifiEstablished, tvm_simpmes, "Соединение с Wi-Fi установлено"},    
    {SendFromISR,     tvm_1intmes, "Передано %s сообщение из прерывания"},
    {SendFromTask,    tvm_2intmes, "Передано %s сообщение из задачи на %s миллисекунде"},
    {NoSendISRled33,  tvm_simpmes, "Не удалось отправить сообщение из прерывания led33"},
    {NoSend33_MODE0,  tvm_simpmes, "Не отправлено на State - режим контр.светодиода выключен"},

    {rdf010,          tvm_simpmes, rdfNotOpenPhotoFile},      // "Не удалось открыть файл фото для чтения"
    {rdf011,          tvm_2intmes, rdfSizePhotoLargeBuffer},  // "Размер файла фото:%s больше размера буфера:%s" 
    {rdf012,          tvm_1intmes, rdfPhotoHasBeenRead},      // "Cчитан файл фото: размер %s"

    {wrf020,          tvm_simpmes, wrfErrOpenWritePhoto},     // "Ошибка открытия файла в режиме записи"
    {wrf021,          tvm_simpmes, wrfSavedPhoto},            // "Сохранено изображение [%s]: %s" 

    {http400,         tvm_simpmes, "Плохой запрос и он не может быть обработан из-за синтаксической ошибки"},
    {http403,         tvm_simpmes, "Запрещено, сервер отказывает в выполнении вашего запроса"},
    {http404,         tvm_3strmes, "Не найдена страница: %s"},
    {http405,         tvm_simpmes, "Метод не допускается для заданного ресурса"},
    {http408,         tvm_simpmes, "Время ожидания истекло, браузер не отправил информацию на сервер за отведенное время"},
    {http500,         tvm_simpmes, "Внутренняя ошибка сервера, запрос не может быть обработан"},
    {http502,         tvm_simpmes, "Плохой шлюз, сервер получил неправильный ответ при попытке передачи запроса"},
    {http504,         tvm_simpmes, "Истекло время ожидания шлюза, вышестоящий сервер не ответил за установленное время"},
    {http997,         tvm_simpmes, "Нет подключения к WiFi перед передачей POST-запроса"},
    /*
    998  = array('998 Незарегистрированная ошибка', 'Код ошибки '.$status.' незарегистрирован');
    999 => array('999 Неопределенная ошибка', 'Код ошибки HTTP/HTTPS неправильный'),
    */
};

// Вычисляем размер списка
int SizeMess=sizeof(amessAPP)/sizeof(tmessAPP);

// ***************************************************** Kvizzy40_Message.h ***

/** Arduino, Esp32-CAM **************************************** kviStream.h ***
 * 
 *               Сделать фото и отправить Base24 изображения на страницу Stream  
 * 
 * v4.1.0, 18.04.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2025 tve                               Дата создания: 26.02.2025
 *
**/

#pragma once            
#include <Arduino.h>
#include "OV2640.h"
#include <base64.h>

// Инициируем объект преобразования изображения base64
base64 b;

// ****************************************************************************
// *                         Записать фото на CD-карту                        *
// ****************************************************************************
#define defSavedPhoto "Сохранено изображение [%s]: %s"
String writePhoto(uint8_t *payload,uint16_t len,time_t nTime,int nFrame,int nCikl)
{
  char bufm[128];                // буфер сообщения
  String Result="nowritePhoto";  // возвращаемый результат
   
  // Определяем имя файла фотографии в каталоге карты microSD
  String path = "/pic"+String(nTime)+"_"+String(nFrame)+".jpg";
  // Сохраняем фотографию на карту microSD
  fs::FS & fs = SD_MMC; 
  File file = fs.open(path, FILE_WRITE);
  // Если не удалось открыть файл для записи
  if(!file)
  {
    Serial.println(wrfErrOpenWritePhoto);
    return Result;
  } 
  // Записываем изображение
  else 
  {
    file.write(payload, len); 
    sprintf(bufm,defSavedPhoto,String(nCikl),path.c_str()); // "Сохранено изображение [%s]: %s"
    wrfSavedPhoto=bufm;
    Serial.println(wrfSavedPhoto); 
  }
  file.close();
  return path;
}

// ****************************************************************************
// *        Проверить размер файла фотографии, загрузить фото в буфер,        *
// *          преобразовать изображение в строку Base64 и вернуть её          *
// ****************************************************************************
String readPhoto(String path) 
{
  int bufSize=20000;      // максимальный размер файла изображения 
  uint8_t bufi[bufSize];  // буфер изображения
  char bufm[128];         // буфер сообщения

  String stringOne="noBase64";  

  // Открываем файл фото для чтения
  fs::FS & fs = SD_MMC; 
  File file = fs.open(path);
  // Если не удалось открыть файл фото для чтения
  if (!file) 
  {
    Serial.println(rdfNotOpenPhotoFile); // "Не удалось открыть файл фото для чтения" 
    return stringOne;
  }
  // Если размер файла превышает размер буфера
  int rlen = file.available();
  if (rlen>bufSize)
  {
    sprintf(bufm,rdfSizePhotoLargeBuffer,String(rlen),String(bufSize)); // "Размер файла фото:%s больше размера буфера:%s"
    Serial.println(bufm); 
    return stringOne;
  }
  // Загружаем фото в буфер, преобразовываем изображение в строку Base64
  file.read(bufi, rlen); 
  stringOne = b.encode(bufi, rlen);
  sprintf(bufm,rdfPhotoHasBeenRead,String(rlen)); // "Cчитан файл фото: размер %s" 
  Serial.println(bufm); 
  return stringOne;
}
// ****************************************************************************
// *    Отправить кадр на страницу сайта "https://probatv.ru/Stream40/"       *
// ****************************************************************************
void sendhttp(time_t nTime, int nFrame, String path) 
{
  // Объявляем строку ответного сообщения со страницы сайта
  String inMess;
  // Если есть WiFi, отправляем сообщение
  if ((WiFi.status() == WL_CONNECTED)) 
  {
    // Готовим запрос к странице "https://probatv.ru/Stream40/"
    HTTPClient http;
    String ehttp=urlHome+"/Stream40/";  
    http.begin(ehttp);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String frame="data:image/jpeg;base64,"+readPhoto(path);

    //String frame="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAYAAACNMs+9AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAEnQAABJ0Ad5mH3gAAADhSURBVChTVZA9TwJBEIb350pCL1BYoIUFITbE2tAYCxPobGwvEgpsIIaDgkJjtBLI5di7e8w7e0ugmMzXu8/MjivzAgqgOrHyPJfGWVHmgUPO3yrFf35bbIC67yyRZXvY7hi32rwO7kMexVUtFIV0Cb8/JNddvkZji7PZu9HDaI8RRFrfdJm3Lo9eNXvkRTzkYafkje1dn49mg0XjAoYPVhM5ELWDdkmXRpE4UiWyXQtwUlel57F3y8tVBzYrmCY2dvL8FCD6jE5l98r24VMixLgWqe00OQpFVjP6eHhflfwDul9tENLGFW4AAAAASUVORK5CYII=";
    //frame="data:image/jpeg;base64,"+frame;

    String queryString = "src="+frame;      
    // Добавляем время с начала эпохи в параметры
    String stime="&time="+String(nTime);
    queryString=queryString+stime;
    // Добавляем номер кадра в параметры
    String sframe="&frame="+String(nFrame);
    queryString=queryString+sframe;
     
    int tQuery_httpCode = http.POST(queryString); 
    if (tQuery_httpCode > 0) 
    {
      // Не получилось выбрать заголовки
      /*
      for(int i = 0; i< http.headers(); i++)
      {
        Serial.print("http.header(i) = ");
        Serial.println(i);
        Serial.println(http.header(i));
      }
      */
      // Если запрос успешно отправлен
      if (tQuery_httpCode == HTTP_CODE_OK) 
      {
        inMess = http.getString();
        Serial.println("Запрос успешно отправлен: "); Serial.println(inMess);
      }
      // Если ошибка после того, как HTTP-заголовок был отправлен
      // и заголовок ответа сервера был обработан
      else 
      {
        // Если сообщение о ненайденной странице, указываем её
        if (tQuery_httpCode==301)      inMess="Ошибка 301 к странице Stream40: документ перенесен на новый URI";
        else if (tQuery_httpCode==404) inMess="Ошибка 404 к странице Stream40: страница не найдена";
        else                           inMess="Ошибка "+String(tQuery_httpCode)+" запроса страницы Stream40";
        Serial.println(inMess);
      }
    }
    // Если ошибка при отправке POST-запроса
    //    Ошибка POST-запроса: "read Timeout"       - "истекло время ожидания чтения"
    //    Ошибка POST-запроса: "connection refused" - "В соединении отказано"
    else 
    {
      inMess=http.errorToString(tQuery_httpCode);
      Serial.printf("Ошибка POST-запроса: %s\n", inMess.c_str());
    }
    http.end();
  }
  // Если "Нет подключения к WiFi перед передачей POST-запроса"
  else
  {
    inMess="Нет подключения к WiFi перед передачей POST-запроса";
    Serial.println(inMess);
  }
}

// * Задача FreRTOS ***********************************************************
// *           Сделать фото и отправить Base24 изображения на страницу Stream *
// ****************************************************************************
void vStream(void* pvParameters) 
{
  time_t nTime;      // время отправки фрэйма (секунда с начала эпохи)
  time_t nTimeOld;   // время отправки предыдущего фрэйма
  int nFrame=0;      // номер кадра в секунде
  int nCikl=0;       // счетчик фотографирований за сеанс
  // Инициируем метки времени
  time(&nTime);                    // время отправки фрэйма (секунда с начала эпохи)
  time(&nTimeOld);                 // время отправки предыдущего фрэйма
  // Инициируем камеру 
  OV2640 cam;
  cam.init(esp32cam_aithinker_config);
  Serial.println("Камера инициализирована!");
  // Зацикливаем задачу
  while (true) 
  {
    //Serial.println("*** vStream ***");
    #ifdef tmr_STREAM
      // Пересчитываем время и номер кадра
      time(&nTime); 
      if (nTime==nTimeOld) nFrame=nFrame+1;
      else {nTimeOld=nTime; nFrame=0;}
      // Фотографируем (при необходимости со вспышкой) и увеличиваем счетчик фотографий в сеансе
      // digitalWrite(BUILTIN_LED, HIGH);
      cam.run();
      nCikl++;
      // digitalWrite(BUILTIN_LED, LOW);
      // Записываем фото на CD-карту                 
      String imgname = writePhoto(cam.getfb(),cam.getSize(),nTime,nFrame,nCikl);
      // Отправляем кадр на страницу сайта "https://probatv.ru/Stream40/"
      // sendhttp(nTime, nFrame, "/vga640x480.jpg");
      sendhttp(nTime, nFrame, imgname);
    #endif 
    // Отмечаем флагом, что цикл задачи успешно завершен   
    fwdtStream = true;
    // Пропускаем интервал 2971 мсек
    vTaskDelay(2971/portTICK_PERIOD_MS); 
  }
}

// ************************************************************ kviStream.h ***

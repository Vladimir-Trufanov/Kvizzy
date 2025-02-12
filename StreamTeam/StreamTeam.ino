/** Arduino, Esp32-CAM ************************************* StreamTeam.ino ***
 * 
 *     Преобразовать изображения ESP32-Cam формата JPEG в AVI-файлы и сохранить
 *       их на SD-карте, далее выбрать и передать их в браузер в формате MJPEG,
 *                               а также выполнить другие сопутствующие задачи.
 * 
 * v3.4.1, 11.02.2025                                 Автор:      Труфанов В.Е.
 * s60sc 2020 - 2024 and tve 2025                     Дата создания: 31.05.2024
 *
 * Задачи приложения:
 * 
 * Задача по форматированию строк и ведению журнала, 
 * хранящегося в медленной оперативной памяти RTC (не более 8 КБАЙТ):
 * xTaskCreate(logTask,"logTask",1024*3,NULL,LOG_PRI=1,&logHandle);
 * Задача используется, как семафор-мьютекс

**/

#include "appGlobals.h"

void setup() 
{
   logSetup();
   // Подготавливаем хранилище на SD-карте
   if (startStorage()) 
   {
      // Load saved user configuration
      if (loadConfig()) 
      {
         // Инициализируем камеру. 
         // Перед этим функцией psramFound() из файла esp32-hal-psram.c проверяем  
         // spiramDetected — статическую изменяемую переменную, которая указывает на то, 
         // что внешняя SPIRAM включена (если внешняя поддержка SPIRAM включена в sdkconfig, 
         // то при обнаружении её включения spiramDetected становится равной true)
         if (psramFound()) 
         {
            LOG_INF("PSRAM size: %s", fmtSize(ESP.getPsramSize()));
            if (ESP.getPsramSize() > 3 * ONEMEG) prepCam();
            else snprintf(startupFailure, SF_LEN, STARTUP_FAIL "Insufficient PSRAM for app");
         }
         else snprintf(startupFailure, SF_LEN, STARTUP_FAIL "Need PSRAM to be enabled");
      }
   }
  
   #ifdef DEV_ONLY
      devSetup();
   #endif

   // connect wifi or start config AP if router details not available
   startWifi();

   startWebServer();
   if (strlen(startupFailure)) LOG_WRN("%s", startupFailure);
   else 
   {
      // start rest of services
      startSustainTasks(); 
      #if INCLUDE_SMTP
         prepSMTP(); 
      #endif
      #if INCLUDE_FTP_HFS
         prepUpload();
      #endif
      prepPeripherals();
      #if INCLUDE_AUDIO
         prepAudio(); 
      #endif
      #if INCLUDE_TELEM
         prepTelemetry();
      #endif
      #if INCLUDE_TGRAM
         prepTelegram();
      #endif
      prepRecording(); 
      checkMemory();
   } 
}

void loop() 
{
   // confirm not blocked in setup
   LOG_INF("=============== Total tasks: %u ===============\n", uxTaskGetNumberOfTasks() - 1);
   delay(1000);
   vTaskDelete(NULL); // free 8k ram
}

// *********************************************************** Kvizzy30.ino ***


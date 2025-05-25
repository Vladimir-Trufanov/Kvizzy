/** Arduino, Esp32-CAM ***************************************** jsonBase.h ***
 * 
 *                                          Обеспечить работу с документом JSON
 * 
 * v4.4.0, 25.05.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>
#include <ArduinoJson.h>

// --- устарело
// Реестр json-сообщений на страницу State
//const String s4_HIGH    = "{\"led4\":[{\"status\":\"shimHIGH\"}]}";                 // "вспышка включена"
//const String s4_LOW     = "{\"led4\":[{\"status\":\"shimLOW\"}]}";                  // "вспышка ВЫКЛЮЧЕНА"
//const String s4_MODE0   = "{\"led4\":[{\"regim\":0}]}";                             // "режим работы вспышки отключен"
//const String s4_MODEini = "{\"led4\":[{\"regim\":1,\"light\":10,\"time\":2000}]}";  // "начальный режим работы вспышки"
// --- устарело

// Назначаем параметры действующего режима работы вспышки
bool Led4Start=false;    // true - изменён режим работы вспышки 
int jlight=10;           // процент времени свечения в цикле 
int jtime=2000;          // длительность цикла "горит - не горит" (мсек)      
int inlight=10;          // принятый процент времени свечения в цикле 
int intime=2000;         // принятая длительность цикла "горит - не горит" (мсек)      
int nLight=5000000;      // время (5 сек) свечения в цикле, далее пересчитывается 
int nNoLight=5000000;    // время (5 сек) НЕ свечения в цикле, далее пересчитывается  

// Назначаем интервалы подачи сообщений от контроллера 
bool intrvStart=false;   // true - изменены интервалы отправки сообщений
int jmode4=  7007;       // режим работы Led4 
int jimg=    1001;       // подача изображения 
int jtempvl= 3003;       // температура и влажность 
int jlumin=  2002;       // освещённость камеры
int jbar=    5005;       // атмосферное давление

int mode4=  7007;        // принятый режим работы Led4 
int img=    1001;        // принятая подача изображения 
int tempvl= 3003;        // принятые температура и влажность 
int lumin=  2002;        // принятая освещённость камеры
int bar=    5005;        // принятое атмосферное давление

// Запрос изменений - {\"common\":0} 
const String s_COMMON = "{\"common\":0}"; 
// Режим работы вспышки - "{\"led4\":{\"light\":10,\"time\":2000}}"                                    
String s_MODE4 = "{\"led4\":{\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}}"; 
// Интервалы подачи сообщений от контроллера - "{\"intrv\":{\"mode4\":7007,\"img\":1001,\"tempvl\":3003,\"lumin\":2002,\"bar\":5005}}"
String s_INTRV = "{\"intrv\":{\"mode4\":"+String(jmode4)+",\"img\":"+String(jimg)+",\"tempvl\":"+String(jtempvl)+",\"lumin\":"+String(jlumin)+",\"bar\":"+String(jbar)+"}}"; 

// ****************************************************************************
// *        Инициировать параметры действующего режима работы вспышки и       *
// *                интервалов подачи сообщений от контроллера                *
// ****************************************************************************
void iniPMem(Preferences Prefs) 
{
  // Извлекаем постоянные данные
  Prefs.begin("KvizzyPrefs", false);
  jlight=Prefs.getInt("jlight",10);
  jtime=Prefs.getInt("jtime",2000);

  jmode4=Prefs.getInt("jmode4",7007);
  jimg=Prefs.getInt("jtime",jimg);
  jtime=Prefs.getInt("jtime",2000);
  jtime=Prefs.getInt("jtime",2000);
  jtime=Prefs.getInt("jtime",2000);
  jtime=Prefs.getInt("jtime",2000);
  Prefs.end();
  // Рассчитываем времена свечения и несвечения вспышки
  nLight=jtime*jlight/100;  // 2000*10/100=200
  nNoLight=jtime-nLight;    // 2000-200=1800
  nLight=nLight*1000;       // 200000 мкс
  nNoLight=nNoLight*1000;   // 1800000 мкс
  // Формируем json-сообщение для отправки на State
  s_MODE4 = "{\"led4\":{\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}}"; 
}

String jempty = "{}"; // пустая json-строка
String sjson;         // выборка из json-документа

/*
sla6en9edged
browscap=/home/kwinflatht/browscap.ini
browscap=/home/u542632/browscap.ini
*/

// ************************************************************* jsonBase.h ***

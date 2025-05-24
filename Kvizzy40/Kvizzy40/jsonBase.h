/** Arduino, Esp32-CAM ***************************************** jsonBase.h ***
 * 
 *                                          Обеспечить работу с документом JSON
 * 
 * v1.4.2, 12.05.2025                                 Автор:      Труфанов В.Е.
 * Copyright © 2024 tve                               Дата создания: 26.10.2024
**/

#pragma once            
#include <Arduino.h>
#include <ArduinoJson.h>

// --- устарело
// Реестр json-сообщений на страницу State
const String s4_HIGH    = "{\"led4\":[{\"status\":\"shimHIGH\"}]}";                 // "вспышка включена"
const String s4_LOW     = "{\"led4\":[{\"status\":\"shimLOW\"}]}";                  // "вспышка ВЫКЛЮЧЕНА"
const String s4_MODE0   = "{\"led4\":[{\"regim\":0}]}";                             // "режим работы вспышки отключен"
const String s4_MODEini = "{\"led4\":[{\"regim\":1,\"light\":10,\"time\":2000}]}";  // "начальный режим работы вспышки"
// --- устарело

// Назначаем параметры действующего режима работы вспышки
bool Led4Start=false;    // true - изменён режим работы вспышки          - задачей vLed4 в kviLed4.h
bool Led4Chang=false;    // true - обнаружено изменение режима работы led4 задачей vLead в kviLead.h
int jlight=10;           // процент времени свечения в цикле 
int jtime=2000;          // длительность цикла "горит - не горит" (мсек)      
int inlight=10;          // принятый процент времени свечения в цикле 
int intime=2000;         // принятая длительность цикла "горит - не горит" (мсек)      
int nLight=5000000;      // время (5 сек) свечения в цикле, далее пересчитывается 
int nNoLight=5000000;    // время (5 сек) НЕ свечения в цикле, далее пересчитывается  

// Назначаем интервалы подачи сообщений от контроллера 
bool intrvStart=false;   // true - изменены интервалы отправки сообщений
bool intrvChang=false;   // true - обнаружено изменение интервалов задачей vLead в kviLead.h
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
String s4_MODE = "{\"led4\":[{\"regim\":1,\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}]}";  // ---устарело
// Интервалы подачи сообщений от контроллера - "{\"intrv\":{\"mode4\":7007,\"img\":1001,\"tempvl\":3003,\"lumin\":2002,\"bar\":5005}}"
String s_INTRV = "{\"intrv\":{\"mode4\":"+String(jmode4)+",\"img\":"+String(jimg)+",\"tempvl\":"+String(jtempvl)+",\"lumin\":"+String(jlumin)+",\"bar\":"+String(jbar)+"}}"; 

// ****************************************************************************
// *        Инициировать параметры действующего режима работы вспышки         *
// ****************************************************************************
void iniPMem(Preferences Prefs) 
{
  //Prefs.begin("KvizzyPrefs", false);
  //Led4Start=Prefs.getBool("Led4Start",true);
  //Prefs.putBool("Led4Start",Led4Start);
  //jlight=Prefs.getInt("jlight",10);
  //Prefs.putInt("jlight",jlight);
  //jtime=Prefs.getInt("jtime",2000);
  //Prefs.putInt("jtime",jtime);
  //Prefs.end();

  // Извлекаем постоянные данные
  Prefs.begin("KvizzyPrefs", false);
  Led4Start=Prefs.getBool("Led4Start",true);
  jlight=Prefs.getInt("jlight",10);
  jtime=Prefs.getInt("jtime",2000);
  Prefs.end();
  // Рассчитываем времена свечения и несвечения вспышки
  nLight=jtime*jlight/100;  // 2000*10/100=200
  nNoLight=jtime-nLight;    // 2000-200=1800
  nLight=nLight*1000;       // 200000 мкс
  nNoLight=nNoLight*1000;   // 1800000 мкс
  // Формируем json-сообщение для отправки на State
  s4_MODE = "{\"led4\":[{\"regim\":1,\"light\":"+String(jlight)+",\"time\":"+String(jtime)+"}]}";
}

String jempty = "{}"; // пустая json-строка
String sjson;         // выборка из json-документа

/*
sla6en9edged
browscap=/home/kwinflatht/browscap.ini
browscap=/home/u542632/browscap.ini
*/

class TJsonBase
{
   public:

   // Определить объект (конструктор класса)
   TJsonBase();
   // Создать объект и строку всего JSON-документа
   void Create();
   // Выбрать из JSON-документа режим и состояние вспышки
   String jsongetLed4();
   // ---Определить объект (конструктор класса)        
   void jsonset();
   // Показать текущее состояние json-документа            
   void ViewDoc();
   // Обновить json-документ
   void UpdateDoc(String sjson);
   
   private:

   JsonDocument doc;      // JSON-документ
   String AllJson;        // строка всего документа
};
// ****************************************************************************
// *                  Определить объект (конструктор класса)                  *
// ****************************************************************************
TJsonBase::TJsonBase()
{
}
// ****************************************************************************
// *               Создать объект и строку всего JSON-документа               *
// ****************************************************************************
void TJsonBase::Create()
{
   /*
   // Идентификаторы и типы контроллеров "стремящегося к умному хозяйства" (ControllersType)
   tidctrl typectrl
      1,'Esp32-CAM'
      2,'Arduino Pro Mini'
      3,'Esp01'
   // Идентификаторы и типы устройств (DevicesType)
   tiddev typedev
      1,'inLed'      // светодиод c обратной логикой
      2,'Led'        // светодиод
      3,'Core32'     // ядро Esp32
   // Идентификаторы и типы датчиков (SensorsType)
   tidsens typesens
      1,'DHT11'         
      2,'DHT22'       
   */
   // Включаем в документ данные контроллера
   doc["tidctrl"] = 1;                         // идентификатор типа контроллера
   doc["namectrl"] = "Esp32-CAM во двор дачи"; //
   doc["nicctrl"] = "myjoy";                   // nic контроллера
   // Нулевое "0" ядро контроллера
   JsonArray core0 = doc.createNestedArray("core0");
   JsonObject core_0 = core0.createNestedObject();
   core_0["nicdev"] = "core0";      // nic устройства
   core_0["tiddev"] = 3;            // идентификатор типа устройства
   core_0["idcore"] = 0;            // идентификатор ядра
   core_0["stacksize"] = 0;         // выделенный размер стека
   core_0["minstack"] = 0;          // минимальный отмеченный размер
   // Первое "1" ядро контроллера
   JsonArray core1 = doc.createNestedArray("core1");
   JsonObject core_1 = core1.createNestedObject();
   core_1["nicdev"] = "core1";      // nic устройства
   core_1["tiddev"] = 3;            // идентификатор типа устройства
   core_1["idcore"] = 1;            // идентификатор ядра
   core_1["stacksize"] = 0;         // выделенный размер стека
   core_1["minstack"] = 0;          // минимальный отмеченный размер
   // Вспышка, режим работы, состояние
   JsonArray led4 = doc.createNestedArray("led4");
   JsonObject led_4 = led4.createNestedObject();
   led_4["nicdev"] = "led4";       // nic устройства
   led_4["tiddev"] = 1;            // идентификатор типа устройства
   led_4["light"]  = 10;           // процент времени свечения в цикле     
   led_4["time"]   = 2000;         // длительность цикла "горит - не горит" (мсек)     
   led_4["regim"]  = 1;            // режим работы: 0 - выключен, 1 - включён     
   led_4["status"] = "shimLOW";    // текущее состояние светодиода     
   // Выбираем весь json-документ в строку
   serializeJson(doc,AllJson);
   //serializeJsonPretty(doc,Serial); 
}
// ****************************************************************************
// *    Выбрать из JSON-документа режим и состояние контрольного светодиода   *
// ****************************************************************************
String TJsonBase::jsongetLed4()
/*
{
  "led4": [
    {
      "nicdev": "led4",
      "tiddev": 1,
      "light": 10,
      "time": 2000,
      "regim": 1,
      "status": "shimLOW"
    }
  ]
}

sjson={"led4":[{"nicdev":"led4","tiddev":1,"light":10,"time":2000,"regim":1,"status":"shimLOW"}]}
*/
{
   // Инициируем возвращаемую json-строку   
   String sjson = jempty;
   // Формируем json по состоянию контрольного светодиода 
   JsonDocument filter;
   filter["led4"][0]["nicdev"]= true;  // nic устройства
   filter["led4"][0]["tiddev"]= true;  // идентификатор типа устройства
   filter["led4"][0]["light"] = true;  // процент времени свечения в цикле 
   filter["led4"][0]["time"]  = true;  // длительность цикла "горит - не горит" (мсек)
   filter["led4"][0]["regim"] = true;  // режим работы: 0 - выключен, 1 - включён  
   filter["led4"][0]["status"]= true;  // текущее состояние светодиода     
   JsonDocument doc;
   // Заполняем фильтр данными из всего документа
   deserializeJson(doc,AllJson,DeserializationOption::Filter(filter));
   // Формируем json-строку с информацией по контрольному светодиоду
   serializeJson(doc,sjson);
   return sjson;
}
/*
String getEsp32CAM(String sjson) 
{
   JsonDocument filter;
   filter["namectrl"] = true;  // "Esp32-CAM во двор дачи";
   filter["idctrl"] = true; 
   filter["tidctrl"] = true; 
   filter["typectrl"]  = true; // 'Esp32-CAM'
   JsonDocument doc;
   DeserializationError error = deserializeJson(doc, sjson, DeserializationOption::Filter(filter));
   // Test if parsing succeeds.
   if (error) 
   {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      // return;
   }
   String str = "";
   serializeJson(doc,str);
   return str;
}
*/
// ****************************************************************************
// *                  Показать текущее состояние json-документа               *
// ****************************************************************************
void TJsonBase::ViewDoc()
{
   serializeJsonPretty(doc,Serial);
}
// ****************************************************************************
// *                             Обновить json-документ                       *
// ****************************************************************************
void TJsonBase::UpdateDoc(String sjson)
{
   // Показываем предыдущее состояние json-документа
   // ViewDoc(); 

   // "контрольный светодиод включен" -            {"led4":[{"status":"shimHIGH"}]}
   if (sjson==s4_HIGH)                             doc["led4"][0]["status"] = "shimHIGH";

   // "контрольный светодиод ВЫКЛЮЧЕН" -           {"led33":[{"status":"shimLOW"}]}
   else if (sjson==s4_LOW)                         doc["led4"][0]["status"] = "shimLOW";

   // "режим контрольного светодиода выключен" -   {"led33":[{"regim":0}]}
   else if (sjson==s4_MODE0)                       doc["led4"][0]["regim"] = 0;
   else 
   { 
      // Safe! Continue usual tasks.
   }
   // Пересобираем json (строку) всего документа
   serializeJson(doc,AllJson);
   // Показываем текущее состояние json-документа
   // ViewDoc(); 
   // Serial.println(AllJson);
}

// ************************************************************* jsonBase.h ***

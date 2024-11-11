// SynchronizingESP32clock.ino


//  makerguides.com: synchronizing ESP32 time with SNTP server

#include "WiFi.h"
#include "esp_sntp.h"


// Вводим имя и пароль точки доступа
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

void initWiFi() 
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
  }
}

void notify(struct timeval* t) 
{
  Serial.println("synchronized");
}

void initSNTP() 
{  
  sntp_set_sync_interval(1 * 60 * 60 * 1000UL);  // 1 hour
  sntp_set_time_sync_notification_cb(notify);
  esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
  esp_sntp_setservername(0, "pool.ntp.org");
  esp_sntp_init();
  setTimezone();
}

void wait4SNTP() 
{
  while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) 
  {
    delay(100);
    Serial.println("waiting ...");
  }
}

void setTimezone() 
{  
  setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1);
  tzset();
}

void printTime() 
{
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup() 
{
  Serial.begin(115200);
  initWiFi();
  initSNTP();
  wait4SNTP();
}

void loop() 
{
  printTime();
  delay(10000);
}

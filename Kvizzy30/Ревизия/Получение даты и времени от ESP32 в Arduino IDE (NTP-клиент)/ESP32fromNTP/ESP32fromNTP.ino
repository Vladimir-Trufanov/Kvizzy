// ESP32fromNTP.ino
time_t now;
char strftime_buf[64];
struct tm timeinfo;

void setup() 
{
   Serial.begin(115200);
   while (!Serial) continue;
   Serial.println("Последовательный порт работает!");

   // Set timezone to China Standard Time
   setenv("TZ", "MSK-3", 1);
   tzset();


   esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
   esp_netif_sntp_init(&config);
   if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) 
   {
      printf("Не удалось обновить системное время в течение 10 секунд");
   }

   
}

void loop() 
{

   time(&now);

   localtime_r(&now, &timeinfo);
   strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

   Serial.print("The current date/time в Петрозаводске is: ");
   Serial.println(strftime_buf);

   delay(1007);
}

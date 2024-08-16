//сохранение файлов с помощью SD.h

#include "esp_camera.h"
#include "FS.h"                // SD Card ESP32
#include <SPI.h>
#include <SD.h>
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <WiFi.h>
#include "time.h"
#include <WiFiUdp.h>
#include <ESP_Mail_Client.h>

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "";
const char* password = "";

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "@gmail.com"
#define AUTHOR_PASSWORD ""
#define RECIPIENT_EMAIL "@gmail.com"

// NTP Server
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 10800;

// Pin definition for CAMERA_MODEL_AI_THINKER
// Change pin definition if you're using another ESP32 with camera module
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Stores the camera configuration parameters
camera_config_t config;

SMTPSession smtp;                                     // The SMTP Session object used for Email sending
void smtpCallback(SMTP_Status status);                // Callback function to get the Email sending status

int incomingByte = 0;     // для хранения полученного байта

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);          // disable brownout detector

  Serial.begin(115200);

  delay(1000);

  // Initialize MicroSD
  Serial.print("Initializing the MicroSD card module... ");
  initMicroSDCard();

  //connect to WiFi
  Serial.println("Initializing...");
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nConnected to %s IP address: ", ssid);

  // Initialize and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("Initializing Date and Time from NTP Server: ");
  Serial.println("");

  // disconnect WiFi as it's no longer needed
  /*WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.print("Initializing the camera module...");*/

  // Initialize the camera
  Serial.print("Initializing the camera module...");
  configInitCamera();
  Serial.println("Ok!");
}

void loop() {

  if (Serial.available() > 0) {                             // если есть доступные данные

    while (WiFi.status() != WL_CONNECTED) {                 // проверяем не отвалилась ли сетка
      delay(500);
      Serial.print(".");
    }

    char inBy = Serial.read();                                   // считываем байт

    if (inBy == '#') {

      struct tm timeinfo;                                     // Get date and time
      char now[20];
      getLocalTime(&timeinfo);
      strftime(now, 20, "%Y%m%d_%H%M%S", &timeinfo);
      String fName = "p" + String(now) + ".jpg";              // Формируем имя и путь к файлу
      String path = "/" + fName;
      Serial.printf("Picture file name: %s\n", path.c_str());
      takeSavePhoto(path);                                    // Сохраняем фото на карточке

      smtp.debug(0);                                          // отладка включена
      smtp.callback(smtpCallback);                            /* Set the callback function to get the sending results */
      ESP_Mail_Session session;                               /* Declare the session config data */

      session.server.host_name = SMTP_HOST;                   /* Set the session config */
      session.server.port = SMTP_PORT;
      session.login.email = AUTHOR_EMAIL;
      session.login.password = AUTHOR_PASSWORD;
      session.login.user_domain = "mydomain.net";

      SMTP_Message message;                                   /* Declare the message class */

      /* Enable the chunked data transfer with pipelining for large message if server supported */
      message.enable.chunking = true;

      /* Set the message headers */
      message.sender.name = "ESP Mail";
      message.sender.email = AUTHOR_EMAIL;

      message.subject = "Test sending Email with attachments and inline images from SD card and Flash";
      message.addRecipient("user1", "@gmail.com");

      message.html.content = "PRIVET";

      message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
      message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
      message.addHeader("Message-ID: <user1@gmail.com>");

      SMTP_Attachment att;                                              // The attachment data item

      char charBufVar1[35];
      fName.toCharArray(charBufVar1, 35);

      char charBufVar2[35];
      path.toCharArray(charBufVar2, 35);

      att.descr.filename = charBufVar1;
      att.descr.mime = "image/jpg"; //binary data
      att.file.path = charBufVar2;
      att.file.storage_type = esp_mail_file_storage_type_sd;
      att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

      message.addAttachment(att);

      if (!smtp.connect(&session))
        return;

      if (!MailClient.sendMail(&smtp, &message, true))                  // Start sending the Email and close the session
        Serial.println("Error sending Email, " + smtp.errorReason());

      ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());

      delay(15000);

      Serial.print('@');

    }

  }
}

void configInitCamera() {
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; //YUV422,GRAYSCALE,RGB565,JPEG

  // Select lower framesize if the camera doesn't support PSRAM
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10; //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // Initialize the Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void initMicroSDCard() {
  // Start Micro SD card
  Serial.println("Starting SD Card");
  if (!MailClient.sdBegin(14, 2, 15, 13)) {
    Serial.println("SD Card Mount Failed");
    return;
  }
}

void takeSavePhoto(String path) {
  // Take Picture with Camera
  camera_fb_t  * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  // Save picture to microSD card
  File file = SD.open(path.c_str(), FILE_WRITE);
  file.write(fb->buf, fb->len);
  file.close();

  esp_camera_fb_return(fb);
}


void smtpCallback(SMTP_Status status)             // Callback function to get the Email sending status
{
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success())
  {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}

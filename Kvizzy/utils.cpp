/** Arduino, Esp32-CAM ****************************************** utils.cpp ***
 *
 *         Общие утилиты для поддержки: Wi-Fi, NTP, удаленного ведения журнала,
 *                                 кодировки base64, режима ожидания устройства
 *                        - some functions based on code contributed by gemi254
 *                        
 * v3.4.1, 11.02.2025                                 Автор:      Труфанов В.Е.
 * s60sc 2021 - 2023 and tve 2025                     Дата создания: 31.05.2024
**/

#include "appGlobals.h"

RTC_NOINIT_ATTR uint32_t crashLoop;     // признак обнаруженного аварийного цикла недостаточного питания
char startupFailure[SF_LEN] = {0};      // разрешение сообщать о любых сбоях при запуске через браузер для удаленных устройств

static SemaphoreHandle_t logSemaphore = NULL;  // флаг того, что сообщение журнала отформатировано
static SemaphoreHandle_t logMutex     = NULL;  // мьютекс контроля доступа к форматировщику сообщений журнала
TaskHandle_t             logHandle    = NULL;
bool                     monitorOpen  = true;  // флаг того, что последовательный порт открыт

// Определяем переменные ведения журнала на основе оперативной памяти в медленной памяти RTC 
// (не инициализированные)
RTC_NOINIT_ATTR char messageLog[RAM_LOG_LEN];  // массив символов журнала сообщений
RTC_NOINIT_ATTR uint16_t mlogEnd;              // адрес последнего байта в журнале сообщений

static void initBrownout(void);


/*
bool dbgVerbose = false;
bool timeSynchronized = false;
bool dataFilesChecked = false;
size_t alertBufferSize = 0;
byte* alertBuffer = NULL; // buffer for telegram / smtp alert image

/************************** Wifi **************************/

//#include <esp_task_wdt.h>
 
/** Do not hard code anything below here unless you know what you are doing **/
/** Use the web interface to configure wifi settings **/

/*
char hostName[MAX_HOST_LEN] = ""; // Default Host name
char ST_SSID[MAX_HOST_LEN]  = ""; //Default router ssid
char ST_Pass[MAX_PWD_LEN] = ""; //Default router passd

// leave following blank for dhcp
char ST_ip[MAX_IP_LEN]  = ""; // Static IP
char ST_sn[MAX_IP_LEN]  = ""; // subnet normally 255.255.255.0
char ST_gw[MAX_IP_LEN]  = ""; // gateway to internet, normally router IP
char ST_ns1[MAX_IP_LEN] = ""; // DNS Server, can be router IP (needed for SNTP)
char ST_ns2[MAX_IP_LEN] = ""; // alternative DNS Server, can be blank

// Access point Config Portal SSID and Pass
char AP_SSID[MAX_HOST_LEN] = "";
char AP_Pass[MAX_PWD_LEN] = "";
char AP_ip[MAX_IP_LEN]  = ""; // Leave blank to use 192.168.4.1
char AP_sn[MAX_IP_LEN]  = "";
char AP_gw[MAX_IP_LEN]  = "";

// basic HTTP Authentication access to web page
char Auth_Name[MAX_HOST_LEN] = ""; 
char Auth_Pass[MAX_PWD_LEN] = "";

int responseTimeoutSecs = 10; // time to wait for FTP or SMTP response
bool allowAP = true;  // set to true to allow AP to startup if cannot connect to STA (router)
uint32_t wifiTimeoutSecs = 30; // how often to check wifi status
static bool APstarted = false;
esp_ping_handle_t pingHandle = NULL;
bool usePing = true;

static void startPing();

static void setupMdnsHost() {  
  // set up MDNS service 
  char mdnsName[MAX_IP_LEN]; // max mdns host name length
  snprintf(mdnsName, MAX_IP_LEN, hostName);
  if (MDNS.begin(mdnsName)) {
    // Add service to MDNS
    MDNS.addService("http", "tcp", HTTP_PORT);
    MDNS.addService("https", "tcp", HTTPS_PORT);
    //MDNS.addService("ws", "udp", 83);
    //MDNS.addService("ftp", "tcp", 21);    
    LOG_INF("mDNS service: http://%s.local", mdnsName);
  } else LOG_WRN("mDNS host: %s Failed", mdnsName);
  debugMemory("setupMdnsHost");
}

static const char* wifiStatusStr(wl_status_t wlStat) {
  switch (wlStat) {
    case WL_NO_SHIELD: return "wifi not initialised";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "not available, use AP";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "unable to connect";
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)    
    case WL_STOPPED: return "wifi stopped";
#endif
    default: return "Invalid WiFi.status";
  }
}

const char* getEncType(int ssidIndex) {
  switch (WiFi.encryptionType(ssidIndex)) {
    case (WIFI_AUTH_OPEN): return "Open";
    case (WIFI_AUTH_WEP): return "WEP";
    case (WIFI_AUTH_WPA_PSK): return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK): return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK): return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE): return "WPA2_ENTERPRISE";
    case (WIFI_AUTH_MAX): return "AUTH_MAX";
    default: return "Not listed";
  }
}

static void onWiFiEvent(WiFiEvent_t event) {
  // callback to report on wifi events
  switch (event) {
    case ARDUINO_EVENT_WIFI_READY: break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE: break;
    case ARDUINO_EVENT_WIFI_STA_START: LOG_INF("Wifi Station started, connecting to: %s", ST_SSID); break;
    case ARDUINO_EVENT_WIFI_STA_STOP: LOG_INF("Wifi Station stopped %s", ST_SSID); break;
    case ARDUINO_EVENT_WIFI_AP_START: {
      if (strlen(AP_SSID) && !strcmp(WiFi.softAPSSID().c_str(), AP_SSID)) {
        LOG_INF("Wifi AP SSID: %s started, use '%s://%s' to connect", WiFi.softAPSSID().c_str(), useHttps ? "https" : "http", WiFi.softAPIP().toString().c_str());
        APstarted = true;
      }
      break;
    }
    case ARDUINO_EVENT_WIFI_AP_STOP: {
      if (!strcmp(WiFi.softAPSSID().c_str(), AP_SSID)) {
        LOG_INF("Wifi AP stopped: %s", AP_SSID);
        APstarted = false;
      }
      break;
    }
    case ARDUINO_EVENT_WIFI_STA_GOT_IP: LOG_INF("Wifi Station IP, use '%s://%s' to connect", useHttps ? "https" : "http", WiFi.localIP().toString().c_str()); break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP: LOG_INF("Wifi Station lost IP"); break;
    case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED: break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED: LOG_INF("WiFi Station connection to %s, using hostname: %s", ST_SSID, hostName); break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED: LOG_INF("WiFi Station disconnected"); break;
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED: LOG_INF("WiFi AP client connection"); break;
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: LOG_INF("WiFi AP client disconnection"); break;
    case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED: break;
    case ARDUINO_EVENT_WIFI_AP_GOT_IP6: LOG_INF("AP interface V6 IP addr is preferred"); break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6: LOG_INF("Station interface V6 IP addr is preferred"); break;
    default: LOG_WRN("WiFi Unhandled event %d", event); break;
  }
}

static void setWifiAP() {
  if (!APstarted) {
    // Set access point with static ip if provided
    if (strlen(AP_ip) > 1) {
      LOG_INF("Set AP static IP :%s, %s, %s", AP_ip, AP_gw, AP_sn);  
      IPAddress _ip, _gw, _sn, _ns1 ,_ns2;
      _ip.fromString(AP_ip);
      _gw.fromString(AP_gw);
      _sn.fromString(AP_sn);
      // set static ip
      WiFi.softAPConfig(_ip, _gw, _sn);
    } 
    WiFi.softAP(AP_SSID, AP_Pass);
    debugMemory("setWifiAP");
  }
}

static void setWifiSTA() {
  // set station with static ip if provided
  if (strlen(ST_ip) > 1) {
    IPAddress _ip, _gw, _sn, _ns1, _ns2;
    if (!_ip.fromString(ST_ip)) LOG_WRN("Failed to parse IP: %s", ST_ip);
    else {
      _ip.fromString(ST_ip);
      _gw.fromString(ST_gw);
      _sn.fromString(ST_sn);
      _ns1.fromString(ST_ns1);
      _ns2.fromString(ST_ns2);
      // set static ip
      WiFi.config(_ip, _gw, _sn, _ns1); // need DNS for SNTP
      LOG_INF("Wifi Station set static IP");
    } 
  } else LOG_INF("Wifi Station IP from DHCP");
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  WiFi.enableIPv6(USE_IP6); 
#endif
  WiFi.begin(ST_SSID, ST_Pass);
  debugMemory("setWifiSTA");
}

bool startWifi(bool firstcall) {
  // start wifi station (and wifi AP if allowed or station not defined)
  if (firstcall) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.persistent(false); // prevent the flash storage WiFi credentials
    WiFi.setAutoReconnect(false); // Set whether module will attempt to reconnect to an access point in case it is disconnected
    WiFi.softAPdisconnect(true); // kill rogue AP on startup
    WiFi.setHostname(hostName);
    delay(100);
    WiFi.onEvent(onWiFiEvent);
  }
  setWifiSTA();
  // connect to Wifi station
  uint32_t startAttemptTime = millis();
  // Stop trying on failure timeout, will try to reconnect later by ping
  wl_status_t wlStat = WL_NO_SSID_AVAIL;
  if (strlen(ST_SSID)) {
    while (wlStat = WiFi.status(), wlStat != WL_CONNECTED && millis() - startAttemptTime < 5000)  {
      logPrint(".");
      delay(500);
    }
  }
  if (wlStat == WL_NO_SSID_AVAIL || allowAP) setWifiAP(); // AP allowed if no Station SSID eg on first time use 
  if (wlStat != WL_CONNECTED) LOG_WRN("SSID %s not connected %s", ST_SSID, wifiStatusStr(wlStat));
#if CONFIG_IDF_TARGET_ESP32S3
  setupMdnsHost(); // not on ESP32 as uses 6k of heap
#endif
  // show stats of requested SSID
  int numNetworks = WiFi.scanNetworks();
  for (int i=0; i < numNetworks; i++) {
    if (!strcmp(WiFi.SSID(i).c_str(), ST_SSID))
      LOG_INF("Wifi stats for %s - signal strength: %d dBm; Encryption: %s; channel: %u",  ST_SSID, WiFi.RSSI(i), getEncType(i), WiFi.channel(i));
  }
  if (pingHandle == NULL) startPing();
  return wlStat == WL_CONNECTED ? true : false;
}

void resetWatchDog() {
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  // Code for version 3.x
  // use ping task as watchdog in case of freeze
  static bool watchDogStarted = false;
  if (watchDogStarted) esp_task_wdt_reset();
  else {
    esp_task_wdt_deinit(); 
    esp_task_wdt_config_t twdt_config = {
      .timeout_ms = (wifiTimeoutSecs * 1000 * 2),
      .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
      .trigger_panic = true, // panic abort on watchdog alert (contains wdt_isr)
    };
    esp_task_wdt_init(&twdt_config);
    esp_task_wdt_add(NULL);
    if (esp_task_wdt_status(NULL) == ESP_OK) {
      watchDogStarted = true;
      esp_task_wdt_reset();
      LOG_INF("WatchDog started using task: %s", pcTaskGetName(NULL));
    } else LOG_ERR("WatchDog failed to start");
  }
#else
  // Code for version 2.x
  // use ping task as watchdog in case of freeze
  static bool watchDogStarted = false;
  if (watchDogStarted) esp_task_wdt_reset();
  else {
    esp_task_wdt_init(wifiTimeoutSecs * 2, true); // panic abort on watchdog alert (contains wdt_isr)
    esp_task_wdt_add(NULL);
    watchDogStarted = true;
    LOG_INF("WatchDog started using task: %s", pcTaskGetName(NULL));
  }
#endif
}

static void statusCheck() {
  // regular status checks
  doAppPing();
  if (!timeSynchronized) getLocalNTP();
  if (!dataFilesChecked) dataFilesChecked = checkDataFiles();
#if INCLUDE_MQTT
  if (mqtt_active) startMqttClient();
#endif
}
*/

/******************************************************************************
 *       Сбросить признак обнаруженного аварийного цикла недостаточного питания
**/
void resetCrashLoop() 
{
   crashLoop = 0;
}

/*
static void pingSuccess(esp_ping_handle_t hdl, void *args) {
  //uint32_t elapsed_time;
  //esp_ping_get_profile(hdl, ESP_PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
  if (DEBUG_MEM) {
    static uint32_t minStack = UINT32_MAX;
    uint32_t freeStack = (uint32_t)uxTaskGetStackHighWaterMark(NULL);
    if (freeStack < minStack) {
      minStack = freeStack;
      if (freeStack < MIN_STACK_FREE) LOG_WRN("Task ping stack space only: %u", freeStack);
      else LOG_INF("Task ping stack space reduced to: %u", freeStack);
    }
  }
  resetWatchDog();
  if (dataFilesChecked) resetCrashLoop();
  statusCheck();
}

static void pingTimeout(esp_ping_handle_t hdl, void *args) {
  // a ping check is used because esp may maintain a connection to gateway which may be unuseable, which is detected by ping failure
  // but some routers may not respond to ping - https://github.com/s60sc/ESP32-CAM_MJPEG2SD/issues/221
  // so setting usePing to false ignores ping failure if connection still present
  resetWatchDog();
  if (strlen(ST_SSID)) {
    wl_status_t wStat = WiFi.status();
    if (wStat != WL_NO_SSID_AVAIL && wStat != WL_NO_SHIELD) {
      if (usePing) {
        LOG_WRN("Failed to ping gateway, restart wifi ...");
        startWifi(false);
      } else {
        if (wStat == WL_CONNECTED) statusCheck(); // treat as ok
        else {
          LOG_WRN("Disconnected, restart wifi ...");
          startWifi(false);
        }
      }
    }
  }
}

static void startPing() {
  IPAddress ipAddr = WiFi.gatewayIP();
  ip_addr_t pingDest; 
  IP_ADDR4(&pingDest, ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  esp_ping_config_t pingConfig = ESP_PING_DEFAULT_CONFIG();
  pingConfig.target_addr = pingDest;  
  pingConfig.count = ESP_PING_COUNT_INFINITE;
  pingConfig.interval_ms = wifiTimeoutSecs * 1000;
  pingConfig.timeout_ms = 5000;
  pingConfig.task_stack_size = PING_STACK_SIZE;
  pingConfig.task_prio = 1;
  // set ping task callback functions 
  esp_ping_callbacks_t cbs;
  cbs.on_ping_success = pingSuccess;
  cbs.on_ping_timeout = pingTimeout;
  cbs.on_ping_end = NULL; 
  cbs.cb_args = NULL;
  esp_ping_new_session(&pingConfig, &cbs, &pingHandle);
  esp_ping_start(pingHandle);
  LOG_INF("Started ping monitoring - %s", usePing ? "On" : "Off");
  debugMemory("startPing");
}

void stopPing() {
  if (pingHandle != NULL) {
    esp_ping_stop(pingHandle);
    esp_ping_delete_session(pingHandle);
    pingHandle = NULL;
  }
}

#define EXT_IP_HOST "api.ipify.org"
char extIP[MAX_IP_LEN] = "Not assigned"; // router external IP
bool doGetExtIP = true;

void getExtIP() {
  // Get external IP address
  if (doGetExtIP) { 
    WiFiClientSecure hclient;
    if (remoteServerConnect(hclient, EXT_IP_HOST, HTTPS_PORT, "", GETEXTIP)) {
      HTTPClient https;
      int httpCode = HTTP_CODE_NOT_FOUND;
      if (https.begin(hclient, EXT_IP_HOST, HTTPS_PORT, "/", true)) {
        char newExtIp[MAX_IP_LEN] = "";
        httpCode = https.GET();
        if (httpCode == HTTP_CODE_OK) {
          strncpy(newExtIp, https.getString().c_str(), sizeof(newExtIp) - 1);  
          if (strcmp(newExtIp, extIP)) {
            // external IP changed
            strncpy(extIP, newExtIp, sizeof(extIP) - 1);
            updateStatus("extIP", extIP);
            updateStatus("save", "0");
            externalAlert("External IP changed", extIP);
          } else LOG_INF("External IP: %s", extIP);
        } else LOG_WRN("External IP request failed, error: %s", https.errorToString(httpCode).c_str());    
        if (httpCode != HTTP_CODE_OK) doGetExtIP = false;
        https.end();     
      }
      remoteServerClose(hclient);
    }
  }
}
*/

/************** generic WiFiClientSecure functions ******************/
/*
static uint8_t failCounts[REMFAILCNT] = {0};

void remoteServerClose(WiFiClientSecure& sclient) {
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
  if (sclient.available()) sclient.clear();
#else
  if (sclient.available()) sclient.flush();
#endif
  if (sclient.connected()) sclient.stop();
}

bool remoteServerConnect(WiFiClientSecure& sclient, const char* serverName, uint16_t serverPort, const char* serverCert, uint8_t connIdx) {
  // Connect to server if not already connected or previously disconnected
  if (sclient.connected()) return true;
  else {
    if (failCounts[connIdx] >= MAX_FAIL) {
      if (failCounts[connIdx] == MAX_FAIL) {
        LOG_WRN("Abandon %s connection attempt", serverName);
        failCounts[connIdx] = MAX_FAIL + 1;
      }
    } else {
      if (ESP.getFreeHeap() > TLS_HEAP) {
        // not connected, so try for a period of time
        if (useSecure && strlen(serverCert)) sclient.setCACert(serverCert);
        else sclient.setInsecure(); // no cert check
    
        uint32_t startTime = millis();
        while (!sclient.connected()) {
          if (sclient.connect(serverName, serverPort)) break;
          if (millis() - startTime > responseTimeoutSecs * 1000) break;
          delay(2000);
        }
        if (sclient.connected()) {
          failCounts[connIdx] = 0;
          return true;
        }
        else {
          // failed to connect in allocated time
          // 'Memory allocation failed' indicates lack of heap space
          // 'Generic error' can indicate DNS failure
          char errBuf[100] = "Unknown server error";
          int errNum = sclient.lastError(errBuf, sizeof(errBuf));
          LOG_WRN("Timed out connecting to server: %s, Err: %d, %s", serverName, errNum, errBuf);
        }
      } else LOG_WRN("Insufficient heap %s for %s TLS session", fmtSize(ESP.getFreeHeap()), serverName);
      failCounts[connIdx]++;
    }
  }
  return false;
}

void remoteServerReset() {
  // reset fail counts
  for (uint8_t i = 0; i < REMFAILCNT; i++) failCounts[i] = 0;
}
*/

/************************** NTP  **************************/

/*
// Needs to be a time zone string from: https://raw.githubusercontent.com/nayarsystems/posix_tz_db/master/zones.csv
char timezone[FILE_NAME_LEN] = "GMT0";
char ntpServer[MAX_HOST_LEN] = "pool.ntp.org";
uint8_t alarmHour = 1;

time_t getEpoch() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec;
}

void dateFormat(char* inBuff, size_t inBuffLen, bool isFolder) {
  // construct filename from date/time
  time_t currEpoch = getEpoch();
  if (isFolder) strftime(inBuff, inBuffLen, "/%Y%m%d", localtime(&currEpoch));
  else strftime(inBuff, inBuffLen, "/%Y%m%d/%Y%m%d_%H%M%S", localtime(&currEpoch));
}

static void showLocalTime(const char* timeSrc) {
  time_t currEpoch = getEpoch();
  char timeFormat[20];
  strftime(timeFormat, sizeof(timeFormat), "%d/%m/%Y %H:%M:%S", localtime(&currEpoch));
  LOG_INF("Got current time from %s: %s with tz: %s", timeSrc, timeFormat, timezone);
  timeSynchronized = true;
}

bool getLocalNTP() {
  // get current time from NTP server and apply to ESP32
  LOG_INF("Using NTP server: %s", ntpServer);
  configTzTime(timezone, ntpServer);
  if (getEpoch() > 10000) {
    showLocalTime("NTP");    
    return true;
  }
  else {
    LOG_WRN("Not yet synced with NTP");
    return false;
  }
}

void syncToBrowser(uint32_t browserUTC) {
  // Synchronize to browser clock if out of sync
  if (!timeSynchronized) {
    struct timeval tv;
    tv.tv_sec = browserUTC;
    settimeofday(&tv, NULL);
    setenv("TZ", timezone, 1);
    tzset();
    showLocalTime("browser");
  }
}

void formatElapsedTime(char* timeStr, uint32_t timeVal, bool noDays) {
  // elapsed time that app has been running
  uint32_t secs = timeVal / 1000; //convert milliseconds to seconds
  uint32_t mins = secs / 60; //convert seconds to minutes
  uint32_t hours = mins / 60; //convert minutes to hours
  uint32_t days = hours / 24; //convert hours to days
  secs = secs - (mins * 60); //subtract the converted seconds to minutes in order to display 59 secs max
  mins = mins - (hours * 60); //subtract the converted minutes to hours in order to display 59 minutes max
  hours = hours - (days * 24); //subtract the converted hours to days in order to display 23 hours max
  if (noDays) sprintf(timeStr, "%02lu:%02lu:%02lu", hours, mins, secs);
  else sprintf(timeStr, "%lu-%02lu:%02lu:%02lu", days, hours, mins, secs);
}

static time_t setAlarm(uint8_t alarmHour) {
  // calculate future alarm datetime based on current datetime
  // ensure relevant timezone identified (default GMT0)
  time_t currEpoch = getEpoch();
  struct tm* timeinfo = localtime(&currEpoch);
  // set alarm date & time for next given hour
  int nextDay = 0; // try same day then next day
  do {
    timeinfo->tm_mday += nextDay;
    timeinfo->tm_hour = alarmHour;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
    nextDay = 1;
  } while (mktime(timeinfo) < getEpoch());
  char inBuff[30];
  strftime(inBuff, sizeof(inBuff), "%d/%m/%Y %H:%M:%S", timeinfo);
  LOG_INF("Alarm scheduled at %s", inBuff);
  // return future alarm time as epoch seconds
  return mktime(timeinfo);
}

bool checkAlarm() {
  // call from appPing() to check if daily alarm time at given hour has occurred
  static time_t rolloverEpoch = 0;
  if (timeSynchronized && getEpoch() >= rolloverEpoch) {
    // alarm time reached
    rolloverEpoch = setAlarm(alarmHour); // set next alarm time
    return true;
  }
  return false;
}
*/

/********************** misc functions ************************/

/*
bool changeExtension(char* fileName, const char* newExt) {
  // replace original file extension with supplied extension (buffer must be large enough)
  size_t inNamePtr = strlen(fileName);
  // find '.' before extension text
  while (inNamePtr > 0 && fileName[inNamePtr] != '.') inNamePtr--;
  inNamePtr++;
  size_t extLen = strlen(newExt);
  memcpy(fileName + inNamePtr, newExt, extLen);
  fileName[inNamePtr + extLen] = 0;
  return (inNamePtr > 1) ? true : false;
}

void showProgress(const char* marker) {
  // show progess as dots 
  static uint8_t dotCnt = 0;
  logPrint(marker); // progress marker
  if (++dotCnt >= DOT_MAX) {
    dotCnt = 0;
    logLine();
  }
}

bool calcProgress(int progressVal, int totalVal, int percentReport, uint8_t &pcProgress) {
  // calculate percentage progress, only report back on percentReport boundary
  uint8_t percentage = (progressVal * 100) / totalVal;
  if (percentage >= pcProgress + percentReport) {
    pcProgress = percentage;
    return true;
  } else return false;
}

bool urlEncode(const char* inVal, char* encoded, size_t maxSize) {
  int encodedLen = 0;
  char hexTable[] = "0123456789ABCDEF";
  while (*inVal) {
    if (isalnum(*inVal) || strchr("$-_.+!*'(),:@~#", *inVal)) *encoded++ = *inVal;
    else {
      encodedLen += 3; 
      if (encodedLen >= maxSize) return false;  // Buffer overflow
      *encoded++ = '%';
      *encoded++ = hexTable[(*inVal) >> 4];
      *encoded++ = hexTable[*inVal & 0xf];
    }
    inVal++;
  }
  *encoded = 0;
  return true;
}

void urlDecode(char* inVal) {
  // replace url encoded characters
  std::string decodeVal(inVal); 
  std::string replaceVal = decodeVal;
  std::smatch match; 
  while (regex_search(decodeVal, match, std::regex("(%)([0-9A-Fa-f]{2})"))) {
    std::string s(1, static_cast<char>(std::strtoul(match.str(2).c_str(),nullptr,16))); // hex to ascii 
    replaceVal = std::regex_replace(replaceVal, std::regex(match.str(0)), s);
    decodeVal = match.suffix().str();
  }
  strcpy(inVal, replaceVal.c_str());
}

void listBuff (const uint8_t* b, size_t len) {
  // output buffer content as hex, 16 bytes per line
  if (!len || !b) LOG_WRN("Nothing to print");
  else {
    for (size_t i = 0; i < len; i += 16) {
      int linelen = (len - i) < 16 ? (len - i) : 16;
      for (size_t k = 0; k < linelen; k++) logPrint(" %02x", b[i+k]);
      puts(" ");
    }
  }
}

size_t isSubArray(uint8_t* haystack, uint8_t* needle, size_t hSize, size_t nSize) {
  // find a subarray (needle) in another array (haystack)
  size_t h = 0, n = 0; // Two pointers to traverse the arrays
  // Traverse both arrays simultaneously
  while (h < hSize && n < nSize) {
    // If element matches, increment both pointers
    if (haystack[h] == needle[n]) {
      h++;
      n++;
      // If needle is completely traversed
      if (n == nSize) return h; // position of end of needle
    } else {
      // if not, increment h and reset n
      h = h - n + 1;
      n = 0;
    }
  }
  return 0; // not found
}

void removeChar(char* s, char c) {
  // remove specified character from string
  int writer = 0, reader = 0;
  while (s[reader]) {
    if (s[reader] != c) s[writer++] = s[reader];
    reader++;       
  }
  s[writer] = 0;
}

void replaceChar(char* s, char c, char r) {
  // replace specified character in string
  int reader = 0;
  while (s[reader]) {
    if (s[reader] == c) s[reader] = r;
    reader++;       
  }
}
*/

/******************************************************************************
 *                       Преобразовать представление размера некоторой величины 
 *                  к более удобочитаемому виду (байты, Кбайты, Mбайты, Гбайты)                                              
**/
char* fmtSize (uint64_t sizeVal)
{
  static char returnStr[20];
  if (sizeVal < 50 * 1024) sprintf(returnStr, "%llu байт", sizeVal);
  else if (sizeVal < ONEMEG) sprintf(returnStr, "%lluKB", sizeVal / 1024);
  else if (sizeVal < ONEMEG * 1024) sprintf(returnStr, "%0.1fMB", (double)(sizeVal) / ONEMEG);
  else sprintf(returnStr, "%0.1fGB", (double)(sizeVal) / (ONEMEG * 1024));
  return returnStr;
}

/*
void checkMemory(const char* source ) {
  LOG_INF("%s Free: heap %u, block: %u, min: %u, pSRAM %u", source, ESP.getFreeHeap(), ESP.getMaxAllocHeap(), ESP.getMinFreeHeap(), ESP.getFreePsram());
  if (ESP.getFreeHeap() < WARN_HEAP) LOG_WRN("Free heap only %u, min %u", ESP.getFreeHeap(), ESP.getMinFreeHeap());
  if (ESP.getMaxAllocHeap() < WARN_ALLOC) LOG_WRN("Max allocatable heap block is only %u", ESP.getMaxAllocHeap());
}

uint32_t checkStackUse(TaskHandle_t thisTask, int taskIdx) {
  // get minimum free stack size for task since started
  static uint32_t minStack[20]; 
  uint32_t freeStack = 0;
  if (thisTask != NULL) {
    freeStack = (uint32_t)uxTaskGetStackHighWaterMark(thisTask);
    if (!minStack[taskIdx]) {
      minStack[taskIdx] = freeStack; // initialise
      LOG_INF("Task %s on core %d, initial stack space %u", pcTaskGetTaskName(thisTask), xPortGetCoreID(), freeStack);
    }
    if (freeStack < minStack[taskIdx]) {
      minStack[taskIdx] = freeStack;
      if (freeStack < MIN_STACK_FREE) LOG_WRN("Task %s on core %d, stack space only: %u", pcTaskGetTaskName(thisTask), xPortGetCoreID(), freeStack);
      else LOG_INF("Task %s on core %d, stack space reduced to %u", pcTaskGetTaskName(thisTask), xPortGetCoreID(), freeStack);
    }
  }
  return freeStack;
}
*/

void debugMemory(const char* caller) 
{
   /*
   if (DEBUG_MEM) 
   {
      logPrint("%s > Free: heap %u, block: %u, min: %u, pSRAM %u\n", caller, ESP.getFreeHeap(), ESP.getMaxAllocHeap(), ESP.getMinFreeHeap(), ESP.getFreePsram());
      delay(FLUSH_DELAY);
   }
   */
}

/*
void doRestart(const char* restartStr) {
  LOG_ALT("Controlled restart: %s", restartStr);
#ifdef ISCAM
  appShutdown();
#endif
  resetCrashLoop();
  flush_log(true);
  delay(2000);
  ESP.restart();
}

uint16_t smoothAnalog(int analogPin, int samples) {
  // get averaged analog pin value 
  uint32_t level = 0; 
  if (analogPin > 0) {
    for (int j = 0; j < samples; j++) level += analogRead(analogPin); 
    level /= samples;
  }
  return level;
}

void setupADC() {
  analogSetAttenuation(ADC_ATTEN);
  analogReadResolution(ADC_BITS);
}

float smoothSensor(float latestVal, float smoothedVal, float alpha) {
  // simple Exponential Moving Average filter 
  // where alpha between 0.0 (max smooth) and 1.0 (no smooth)
  return (latestVal * alpha) + smoothedVal * (1.0 - alpha);
}
*/

/**                                                     Удаленный показ журнала
 * ----------------------------------------------------------------------------                                                     
 * Выбор режима ведения журнала в пользовательском интерфейсе: 
 *    false: вывод журнала на последовательный порт или на веб-монитор
 *    true: дополнительно сохранение журнал на SD-карту. 
 * Для того, чтобы показать (загрузить) созданный журнал нажмите кнопку 
 *    "Показать журнал" в браузере.
 * Для того, чтобы очистить содержимое журнала, на веб-странице журнала 
 *    нажмите ссылку "Очистить журнал"
**/
 
#define MAX_OUT 200                            // размер выходного буфера строк журнала
static va_list arglist;                        // нефиксированный набор параметров форматирования строки
static char fmtBuf[MAX_OUT];                   // буфер неотформатированных (исходных строк журнала)
static char outBuf[MAX_OUT];                   // выходной буфер строк журнала
char alertMsg[MAX_OUT];
static int logWait = 100;                      // интервал ожидания освобождения семафора для строки журнала (ms)
bool useLogColors = false;  // true to colorise log messages (eg if using idf.py, but not arduino)
bool wsLog = false;

#define WRITE_CACHE_CYCLE 5

bool sdLog = false;                            // изначально запрет лога на CSD
int logType = 0;                               // which log contents to display (0:ram, 1:sd, 2:ws)
static FILE* log_remote_fp = NULL;
static uint32_t counter_write = 0;

/******************************************************************************
 *                                                    Очистить журнал сообщений
**/
static void ramLogClear() 
{
   mlogEnd = 0;
   memset(messageLog, 0, RAM_LOG_LEN);
}
/******************************************************************************
 *                          Разместить строку журнала в оперативной памяти rtc,
 * похоже здесь ошибка с размещением крайнего сообщения в буфере !!! 2025-02-12
**/
static void ramLogStore(size_t msgLen) 
{
   // Если места в журнале уже не будет хватать,
   // разбиваем последнее сообщение на 2 части и первую размещаем 
   // в конце журнала, а вторую переносим в начало
   if (mlogEnd + msgLen >= RAM_LOG_LEN) 
   {
      // Определяем размер свободного кусочка в памяти,
      // здесь размещаем первую часть сообщения
      uint16_t firstPart = RAM_LOG_LEN - mlogEnd;
      memcpy(messageLog + mlogEnd, outBuf, firstPart);
      // В начало журнала переносим оставшуюся часть сообщения
      msgLen -= firstPart;
      memcpy(messageLog, outBuf + firstPart, msgLen);
      // Настраиваемся на новое начало сообщения
      mlogEnd = 0;
   } 
   // если места хватает, дописываем сообщение в конец буфера
   else memcpy(messageLog + mlogEnd, outBuf, msgLen);
   mlogEnd += msgLen;
}

/*
void flush_log(bool andClose) {
  if (log_remote_fp != NULL) {
    fsync(fileno(log_remote_fp));  
    fflush(log_remote_fp);
    if (andClose) {
      LOG_INF("Closed SD file for logging");
      fclose(log_remote_fp);
      log_remote_fp = NULL;
    } else delay(1000);
  }  
}

static void remote_log_init_SD() {
#if !CONFIG_IDF_TARGET_ESP32C3
  STORAGE.mkdir(DATA_DIR);
  // Open remote file
  log_remote_fp = NULL;
  log_remote_fp = fopen("/sdcard" LOG_FILE_PATH, "a");
  if (log_remote_fp == NULL) {LOG_WRN("Failed to open SD log file %s", LOG_FILE_PATH);}
  else {
    logPrint(" \n");
    LOG_INF("Opened SD file for logging");
  }
#endif
}

void reset_log() {
  if (logType == 0) ramLogClear();
  if (logType == 2) {
    if (log_remote_fp != NULL) flush_log(true); // Close log file
    STORAGE.remove(LOG_FILE_PATH);
    remote_log_init_SD();
  }
  if (logType != 1) LOG_INF("Cleared %s log file", logType == 0 ? "RAM" : "SD"); 
}

void remote_log_init() {
  // setup required log mode
  if (sdLog) {
    flush_log(false);
    remote_log_init_SD(); // store log on sd card
  } else flush_log(true);
}
*/

/******************************************************************************
 *                   Регулировать ведение журнала приложения в отдельной задаче  
 *                                для уменьшения размера стека в других задачах
 *                   (то есть вместо мьютекса используем прямое уведомление для 
 *                                 расформатирования выходной строки сообщения)              
**/
static void logTask(void *arg) 
{
   while(true) 
   {
      // Ожидаем поступления уведомления для форматирования очередной строки
      // неограниченное время (уведомление используетя, как двоичный семафор и
      // сбросится в ноль перед завершением работы ulTaskNotifyTake())
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
      // Выводим отформатированный список аргументов в выходной буфер,
      // преобразуя каждую запись в списке аргументов в соответствии со 
      // спецификатором формата
      vsnprintf(outBuf,MAX_OUT,fmtBuf,arglist);
      // Выходим из функции с переменным списком параметров 
      va_end(arglist);
      // Занимаем семафор для вывода строки журнала
      xSemaphoreGive(logSemaphore);
   }
}
/******************************************************************************
 *                                     Обеспечить вывод текущей строки в журнал
 *                                            в защищенном режиме через мьютекс
**/
void logPrint(const char *format, ...) 
{
   // Запускаем задачу logTask в защищенном режиме после захвата мьютекса
   // для форматирования сообщения, а затем выводим по мере необходимости
   if (xSemaphoreTake(logMutex, pdMS_TO_TICKS(logWait)) == pdTRUE) 
   {
      // Перемещаем входную строку в промежуточный буфер для форматирования
      strncpy(fmtBuf, format, MAX_OUT);
      // Инициализируем список аргументов для последующего форматирования
      va_start(arglist,format); 
      // Увеличиваем на 1 приоритет задачи ведения журнала
      vTaskPrioritySet(logHandle, uxTaskPriorityGet(NULL) + 1);
      // Отправляем уведомление, как двоичный семафор, задаче ведения журнала 
      xTaskNotifyGive(logHandle);
      // Завершаем формирование выходной строки переходом на новую
      // и завершающим нулём
      outBuf[MAX_OUT - 2] = '\n'; 
      outBuf[MAX_OUT - 1] = 0; 
      // Освобождаем семафор - дожидаемся завершения задачи ведения журнала logTask
      xSemaphoreTake(logSemaphore, portMAX_DELAY);    
      // Cохраняем строку в оперативной памяти rtc
      size_t msgLen = strlen(outBuf);
      ramLogStore(msgLen); 
      // Выводим строку на консоль мониторинга для браузера, 
      // если она подключена
      if (outBuf[msgLen - 2] == '~') 
      {
         // set up alert message for browser
         outBuf[msgLen - 2] = ' ';
         strncpy(alertMsg, outBuf, MAX_OUT - 1);
         alertMsg[msgLen - 2] = 0;
      }
      // Выводим строку в последовательный порт,
      // если он уже открыт
      if (monitorOpen) Serial.print(outBuf); 
      // Иначе выделяем время для других задач
      else delay(10); 
      // Выкладываем строку на SD, когда требуется
      if (sdLog) 
      {
         if (log_remote_fp != NULL) 
         {
            // output to SD if file opened
            fwrite(outBuf, sizeof(char), msgLen, log_remote_fp); // log.txt
            // periodic sync to SD
            if (counter_write++ % WRITE_CACHE_CYCLE == 0) fsync(fileno(log_remote_fp));
         } 
      }
      // output to web socket if open
      if (msgLen > 1) 
      {
         /*
         outBuf[msgLen - 1] = 0; // lose final '/n'
         if (wsLog) wsAsyncSend(outBuf);
         */
      }
      // Отпускаем семафор после завершения вывода
      xSemaphoreGive(logMutex);
   } 
}
/******************************************************************************
 *                        Завершить вывод текущей строки и перейти на следующую
**/
void logLine() 
{
  logPrint(" \n");
}
/******************************************************************************
 *                                       Подготовить ведение журнала приложения
 *                                       
 *    HardwareSerial::setDebugOutPut(bool) настраивает последовательный порт (UARTx) 
 * для вывода отладочной информации с помощью Arduino log_x() где x — d для отладки, 
 * e для ошибок или i для информационных сообщений.
 *    По умолчанию сразу после загрузки для ведения журнала используется UART0 
 * (последовательный порт консоли). Если пользователь хочет отключить все log_x() 
 * выходные данные, Serial.setDebugOutput(false) сделает это. Serial.end().
 *    Кроме того, он выполняет и другие задачи, например, отключает контакты UART.
 *    С другой стороны, Serial1.setDebugOutput(true) позволяет настроить отправку 
 * всех сообщений журнала в UART1. Только 1 последовательный порт может выводить 
 * сообщения журнала. Будет использоваться последний последовательный порт, для которого  
 * DebugOutput установлено значение true.
 *    Например, необходимо использовать UART0 для подключения к внешнему устройству, 
 * но также видеть логи в UART1. Код для этого будет следующим:
 *    
 * Serial.begin(9600);           // подключено к внешнему аппаратному обеспечению UART
 * Serial1.begin(115200);        // порт для наших сообщений журнала
 * Serial1.setDebugOutput(true); // позволяет отправлять сообщения журнала в UART1, а не в UART0
 * log_i("Это сообщение журнала отправляется в Serial1!");
 * 
 * ESP.getFreeHeap() — функция, которая возвращает размер свободной памяти
 * 
 * esp_log_level_set — функция устанавливает уровень логирования для указанного тега.
 * Если лог для указанного компонента уже был разрешён, то эта функция поменяет 
 * предыдущую настройку. Доступно несколько макросов для различных уровней детализации:
 * ESP_LOGE - ошибка (самый низкий), ESP_LOGW - предупреждение, ESP_LOGI - информация,
 * ESP_LOGD - отлаживание, ESP_LOGV - подробно (самый высокий).
 *    
 * esp_log_level_set("*", ESP_LOG_ERROR);        // установить для всех компонентов уровень ERROR
 * esp_log_level_set("wifi", ESP_LOG_WARN);      // включить журналы WARN из стека WiFi
 * esp_log_level_set("dhcpc", ESP_LOG_INFO);     // включить журналы INFO из клиента DHCP
 * 
 * xSemaphoreCreateBinary() — это функция FreeRTOS, которая создаёт двоичный семафор 
 * и возвращает дескриптор, по которому к этому семафору можно обращаться.
 * Семафор создаётся в «пустом» состоянии, поэтому, чтобы его можно было «взять» 
 * вызовом xSemaphoreTake() (то есть для того, чтобы задача могла на семафоре разблокироваться), 
 * семафор надо сначала «предоставить» с помощью вызова API-функции xSemaphoreGive().
 * В случае проблем с выделением памяти функция возвратит NULL. Эта API-функция не требует 
 * никаких параметров и возвращает переменную типа SemaphoreHandle_t.
 * 
 * char* esp_log_system_timestamp(void); - функция возвращает системную временную 
 * метку для использования в выходных данных журнала в формате "ЧЧ:ММ:СС.ссс". 
 * Системное время инициализируется равным * 0 при запуске, но его можно установить
 * на правильное время с помощью SNTP-синхронизации или вручную с помощью стандартных 
 * функций времени POSIX. В настоящее время это не используется при ведении журнала 
 * из больших двоичных объектов (т.е. библиотеки Wi-Fi и Bluetooth), но все равно 
 * будет формироваться тиканье таймера FreeRTOS.
 * 
**/
void logSetup() 
{
   Serial.begin(115200);
   // DBG_ON=false, отключаем вывод данных с помощью log_x() 
   Serial.setDebugOutput(DBG_ON);
   printf("\n\n");
   // DEBUG_MEM=false, отключаем трассировку использования памяти
   if (DEBUG_MEM) printf("init > Free: heap %lu\n", ESP.getFreeHeap());
   // Подавляем сообщения до самого низкого уровня - ESP_LOG_ERROR 
   if (!DBG_ON) esp_log_level_set("*", ESP_LOG_NONE);
   // При повторном запуске основного цикла без перезагрузки
   // выдаем сообщение о недостаточном питании 
   if (crashLoop == MAGIC_NUM) snprintf(startupFailure, SF_LEN, STARTUP_FAIL "Обнаружен аварийный цикл недостаточного питания");
   crashLoop = MAGIC_NUM;
   // Cоздаём семафор для защиты процедуры форматирования строки журнала 
   logSemaphore = xSemaphoreCreateBinary(); 
   // Создаём мьютекс для защиты процедуры вывода строки сообщения в послед.порт, на SD, в браузер, в сокет и т.д.
   logMutex = xSemaphoreCreateMutex();   
   // Освобождаем мьютех для захвата очередным сообщением  
   xSemaphoreGive(logSemaphore);
   // Отпускаем семафор для форматирования строк журнала
   xSemaphoreGive(logMutex);
   // Создаем задачу по форматированию строк и ведению лога
   xTaskCreate(logTask, 
      "logTask",       // имя задачи
      LOG_STACK_SIZE,  // размер стека = 1024 * 3
      NULL,            // входной параметр задачи
      LOG_PRI,         // приоритет задачи = 1
      &logHandle);     // ссылка на заголовок задачи
   // Если размер журнала системных сообщений в байтах превысил размер
   // медленной оперативной памяти RTC, то очищаем его
   if (mlogEnd >= RAM_LOG_LEN) ramLogClear(); 
   // Выводим сообщение после настройки журнала
   LOG_INF("Выполнена настройка журнала, размер %u, начало с %u\n\n",RAM_LOG_LEN,mlogEnd);
   LOG_INF("=============== %s %s ===============", APP_NAME, APP_VER);
   // Предупреждаем о недостаточном питании
   initBrownout();
   LOG_INF("Откомпилировано на arduino-esp32 v%d.%d.%d", ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH);
   ////LOG_INF(" ESP32 Arduino core version: %s", ESP_ARDUINO_VERSION_STR);
   // Переводим контроллер в спящий режим
   // wakeupResetReason();
   //
   // if (alertBuffer == NULL) alertBuffer = (byte*)ps_malloc(MAX_ALERT); 
   //
   // debugMemory("logSetup"); 
}

/*
void formatHex(const char* inData, size_t inLen) {
  // format data as hex bytes for output
  char formatted[(inLen * 3) + 1];
  for (int i=0; i<inLen; i++) sprintf(formatted + (i*3), "%02x ", inData[i]);
  formatted[(inLen * 3)] = 0; // terminator
  LOG_INF("Hex: %s", formatted);
}

const char* espErrMsg(esp_err_t errCode) {
  // convert esp error code to text
  static char errText[100];
  esp_err_to_name_r(errCode, errText, 100);
  return errText;
}

void forceCrash() {
  // force crash for testing purposes
  delay(5000);
#pragma GCC diagnostic ignored "-Wdiv-by-zero"
  printf("%u\n", 1/0);
#pragma GCC diagnostic warning "-Wdiv-by-zero"
}
*/

/****************** base 64 ******************/

/*
#define BASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

const uint8_t* encode64chunk(const uint8_t* inp, int rem) {
  // receive 3 byte input buffer and return 4 byte base64 buffer
  rem = 3 - rem; // last chunk may be less than 3 bytes 
  uint32_t buff = 0; // hold 3 bytes as shifted 24 bits
  static uint8_t b64[4];
  // shift input into buffer
  for (int i = 0; i < 3 - rem; i++) buff |= inp[i] << (8*(2-i)); 
  // shift 6 bit output from buffer and encode
  for (int i = 0; i < 4 - rem; i++) b64[i] = BASE64[buff >> (6*(3-i)) & 0x3F]; 
  // filler for last chunk if less than 3 bytes
  for (int i = 0; i < rem; i++) b64[3-i] = '='; 
  return b64;
}

const char* encode64(const char* inp) {
  // helper to base64 encode strings up to 90 chars long
  static char encoded[121]; // space for 4/3 expansion + terminator
  encoded[0] = 0;
  int len = strlen(inp);
  if (len > 90) {
    LOG_WRN("Input string too long: %u chars", len);
    len = 90;
  }
  for (int i = 0; i < len; i += 3) 
    strncat(encoded, (char*)encode64chunk((uint8_t*)inp + i, min(len - i, 3)), 4);
  return encoded;
}
*/

/************** qualitive core idle time monitoring *************/

/*
// not working properly
#include "esp_freertos_hooks.h"

#define INTERVAL_TIME 100 // reporting interval in ms
#define TICKS_PER_INTERVAL (pdMS_TO_TICKS(INTERVAL_TIME))

static uint32_t idleCalls[portNUM_PROCESSORS] = {0};
static uint32_t idleCnt[portNUM_PROCESSORS];

static bool hookCallback() {
  idleCalls[xPortGetCoreID()]++;
  return true;
}

uint32_t* reportIdle() {
  static uint32_t idlePercent[portNUM_PROCESSORS];
  for (int i = 0; i < portNUM_PROCESSORS; i++)
    idlePercent[i] = (100 * idleCnt[i]) / TICKS_PER_INTERVAL;
  return idlePercent;
}

static void idleMonTask(void* p) {
  while (true) {
    for (int i = 0; i < portNUM_PROCESSORS; i++) {
      idleCnt[i] = idleCalls[i];
      idleCalls[i] = 0;
    }
    vTaskDelay(TICKS_PER_INTERVAL);
  }
  vTaskDelete(NULL);
}

void startIdleMon() {
  // report on each core idle time per interval
  // Core 0: wifi, Core 1: Arduino
  LOG_INF("Start core idle time monitoring @ interval %ums", INTERVAL_TIME);
  for (int i = 0; i < portNUM_PROCESSORS; i++) 
    esp_register_freertos_idle_hook_for_cpu(hookCallback, i);
  xTaskCreatePinnedToCore(idleMonTask, "idlemon", 1024, NULL, IDLEMON_PRI, NULL, 0);
}
*/

/****************** send device to sleep (light or deep) & watchdog ******************/

/*
#include <esp_wifi.h>
#include <driver/gpio.h>

static esp_sleep_wakeup_cause_t printWakeupReason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0 : LOG_INF("Wakeup by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : LOG_INF("Wakeup by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : LOG_INF("Wakeup by internal timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : LOG_INF("Wakeup by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : LOG_INF("Wakeup by ULP program"); break;
    case ESP_SLEEP_WAKEUP_GPIO: LOG_INF("Wakeup by GPIO"); break;    
    case ESP_SLEEP_WAKEUP_UART: LOG_INF("Wakeup by UART"); break; 
    default : LOG_INF("Wakeup by reset"); break;
  }
  return wakeup_reason;
}

RTC_NOINIT_ATTR char brownoutStatus;

static esp_reset_reason_t printResetReason() {
  esp_reset_reason_t bootReason = esp_reset_reason();
  switch (bootReason) {
    case ESP_RST_UNKNOWN: LOG_INF("Reset for unknown reason"); break;
    case ESP_RST_POWERON: {
      LOG_INF("Power on reset");
      brownoutStatus = 0;
      messageLog[0] = 0;
      break;
    }
    case ESP_RST_EXT: LOG_INF("Reset from external pin"); break;
    case ESP_RST_SW: LOG_INF("Software reset via esp_restart"); break;
    case ESP_RST_PANIC: LOG_INF("Software reset due to exception/panic"); break;
    case ESP_RST_INT_WDT: LOG_INF("Reset due to interrupt watchdog"); break;
    case ESP_RST_TASK_WDT: LOG_INF("Reset due to task watchdog"); break;
    case ESP_RST_WDT: LOG_INF("Reset due to other watchdogs"); break;
    case ESP_RST_DEEPSLEEP: LOG_INF("Reset after exiting deep sleep mode"); break;
    case ESP_RST_BROWNOUT: LOG_INF("Software reset due to brownout"); break;
    case ESP_RST_SDIO: LOG_INF("Reset over SDIO"); break;
    default: LOG_WRN("Unhandled reset reason"); break;
  }
  return bootReason;
}

esp_sleep_wakeup_cause_t wakeupResetReason() {
  printResetReason();
  esp_sleep_wakeup_cause_t wakeupReason = printWakeupReason();
  return wakeupReason;
}

void goToSleep(int wakeupPin, bool deepSleep) {
#if !CONFIG_IDF_TARGET_ESP32C3
  // if deep sleep, restarts with reset
  // if light sleep, restarts by continuing this function
  LOG_INF("Going into %s sleep", deepSleep ? "deep" : "light");
  delay(100);
  if (deepSleep) { 
    if (wakeupPin >= 0) {
      // wakeup on pin low
      pinMode(wakeupPin, INPUT_PULLUP);
      esp_sleep_enable_ext0_wakeup((gpio_num_t)wakeupPin, 0);
    }
    esp_deep_sleep_start();
  } else {
    // light sleep
    esp_wifi_stop();
    // wakeup on pin high
    if (wakeupPin >= 0) gpio_wakeup_enable((gpio_num_t)wakeupPin, GPIO_INTR_HIGH_LEVEL); 
    esp_light_sleep_start();
  }
  // light sleep restarts here
  LOG_INF("Light sleep wakeup");
  esp_wifi_start();
#else
  LOG_WRN("This function not compatible with ESP32-C3");
#endif
}


// catch software resets due to brownouts
//https://github.com/espressif/esp-idf/blob/master/components/esp_system/port/brownout.c

#include "esp_private/system_internal.h"
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
#include "esp_private/rtc_ctrl.h"
#include "hal/brownout_ll.h"
#else
#include "driver/rtc_cntl.h" // v2.x
#endif

#include "soc/rtc_periph.h"
#include "hal/brownout_hal.h"

#define BROWNOUT_DET_LVL 7

IRAM_ATTR static void notifyBrownout(void *arg) {
  esp_cpu_stall(!xPortGetCoreID());  // Stop the other core.
  esp_reset_reason_set_hint(ESP_RST_BROWNOUT);
  brownoutStatus = 'B';
  esp_restart_noos();
}
*/

/******************************************************************************
 *                          Отключить предупреждение, выводить только один раз, 
 *                                             чтобы предотвратить перезагрузку
**/
static void initBrownout(void) 
{
   /*
   if (brownoutStatus == 'R') 
      LOG_WRN("Предупреждение об отключении, о котором ранее сообщалось");
   else if (brownoutStatus == 'B') 
   {
      LOG_WRN("Будет произведено отключение из-за недостаточного электроснабжения");
      brownoutStatus = 'R';
   } 
   else 
   {
      brownout_hal_config_t cfg = 
      {
         .threshold = BROWNOUT_DET_LVL,
         .enabled = true,
         .reset_enabled = false,
         .flash_power_down = true,
         .rf_power_down = true,
      };
      brownout_hal_config(&cfg);
      #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
         brownout_ll_intr_clear();
         rtc_isr_register(notifyBrownout, NULL, RTC_CNTL_BROWN_OUT_INT_ENA_M, RTC_INTR_FLAG_IRAM);
         brownout_ll_intr_enable(true);
      #else
         rtc_isr_register(notifyBrownout, NULL, RTC_CNTL_BROWN_OUT_INT_ENA_M);
      #endif
      brownoutStatus = 0; 
   }
   */
}

// ************************************************************** utils.cpp ***


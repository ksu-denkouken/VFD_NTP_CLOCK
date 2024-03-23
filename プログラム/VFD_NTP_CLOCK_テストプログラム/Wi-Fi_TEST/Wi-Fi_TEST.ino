#include <WiFi.h>
const char* ssid = "k23re058";
const char* password = "********";
const char* ntpServer = "ntp.jst.mfeed.ad.jp";
const long gmtOffset_sec = 9 * 3600L;  //JSTとの時差
const int daylightOffset_sec = 0;      //夏時間時差
void printTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {  //時刻取得失敗時
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%H:%M:%S");  //時分秒表記
}
void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println("");
  Serial.println("PROGRAM START.");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi CONNECTED.");
  configTime(gmtOffset_sec, daylightOffset_sec,
             ntpServer);
}
void loop() {
  delay(1000);
  printTime();
}
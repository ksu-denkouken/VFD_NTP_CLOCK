

#include<WiFi.h>

const char *ssid = "ssid";
const char *passworld = "pass";

int _flg = 0;

struct tm timeInfo;
int info1, info2, info3;

//LEDレイアウトを定義
boolean Num_Array[10][7] = {
{1, 1, 1, 1, 1, 1, 0}, //0
{0, 1, 1, 0, 0, 0, 0}, //1
{1, 1, 0, 1, 1, 0, 1}, //2
{1, 1, 1, 1, 0, 0, 1}, //3
{0, 1, 1, 0, 0, 1, 1}, //4
{1, 0, 1, 1, 0, 1, 1}, //5
{1, 0, 1, 1, 1, 1, 1}, //6
{1, 1, 1, 0, 0, 1, 0}, //7
{1, 1, 1, 1, 1, 1, 1}, //8
{1, 1, 1, 1, 0, 1, 1}, //9
};

void NumPrint(int Number) {
digitalWrite(33, -Num_Array[Number][0]);
digitalWrite(32, -Num_Array[Number][1]);
digitalWrite(19, -Num_Array[Number][2]);
digitalWrite(38, -Num_Array[Number][3]);
digitalWrite(4, -Num_Array[Number][4]);
digitalWrite(25, -Num_Array[Number][5]);
digitalWrite(26, -Num_Array[Number][6]);
}

void setup() {
Serial.begin(115200);

WiFi.mode(WIFI_STA);
WiFi.disconnect();

if (WiFi.begin(ssid, passworld) != WL_DISCONNECTED) {
ESP.restart();
}

while (WiFi.status() != WL_CONNECTED) {
delay(1000);
}

Serial.println("Connected to the WiFi network!");

configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp"); //NTPの設定

pinMode(32, OUTPUT);
pinMode(33, OUTPUT);
pinMode(19, OUTPUT);
pinMode(18, OUTPUT);
pinMode(4, OUTPUT);
pinMode(25, OUTPUT);
pinMode(26, OUTPUT);

pinMode(21, OUTPUT);
pinMode(27, OUTPUT);

pinMode(13, OUTPUT); //GND1
pinMode(14, OUTPUT); //GND2
pinMode(16, OUTPUT); //GND3
pinMode(17, OUTPUT); //GND4
pinMode(22, OUTPUT); //GND5
pinMode(23, OUTPUT); //GND6
}

int NumParse(int Number, int s){
if (s == 1){
return Number % 10;
}
else if (s == 2){
return Number / 10;
}

return 0;
}

void loop(){
getLocalTime(&timeInfo);//時刻を取得
info1 = timeInfo.tm_sec;
info2 = timeInfo.tm_min;
info3 = timeInfo.tm_hour;

//flg変数を
if (_flg == 0){
digitalWrite(14, LOW);
digitalWrite(13, HIGH);
digitalWrite(17, HIGH);
digitalWrite(16, HIGH);
digitalWrite(23, HIGH);
digitalWrite(22, HIGH);
_flg = 1;
NumPrint(NumParse(info3, 1)); //1桁目の表示
}
else if (_flg == 1){
digitalWrite(14, HIGH);
digitalWrite(13, LOW);
digitalWrite(17, HIGH);
digitalWrite(16, HIGH);
digitalWrite(23, HIGH);
digitalWrite(22, HIGH);
_flg = 2;
NumPrint(NumParse(info3, 2)); //2桁目の表示
}
else if (_flg == 2){
digitalWrite(14, HIGH);
digitalWrite(13, HIGH);
digitalWrite(17, LOW);
digitalWrite(16, HIGH);
digitalWrite(23, HIGH);
digitalWrite(22, HIGH);
_flg = 3;
NumPrint(NumParse(info2, 1)); //3桁目の表示
}
else if (_flg == 3){
digitalWrite(14, HIGH);
digitalWrite(13, HIGH);
digitalWrite(17, HIGH);
digitalWrite(16, LOW);
digitalWrite(23, HIGH);
digitalWrite(22, HIGH);
_flg = 4;
NumPrint(NumParse(info2, 2)); //4桁目の表示
}
else if (_flg == 4){
digitalWrite(14, HIGH);
digitalWrite(13, HIGH);
digitalWrite(17, HIGH);
digitalWrite(16, HIGH);
digitalWrite(23, LOW);
digitalWrite(22, HIGH);
_flg = 5;
NumPrint(NumParse(info1, 1)); //5桁目の表示
}
else if (_flg == 5){
digitalWrite(14, HIGH);
digitalWrite(13, HIGH);
digitalWrite(17, HIGH);
digitalWrite(16, HIGH);
digitalWrite(23, HIGH);
digitalWrite(22, LOW);
_flg = 0;
NumPrint(NumParse(info1, 2)); //6桁目の表示
}

delay(1);

}

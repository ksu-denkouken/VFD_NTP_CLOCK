/*
 * 2023.10.15
 * 日野君考案のVFD NPT CLOCK　プログラム 
 */

/*
 　 使い方
   1. WiFiのSSIDとPASSWORDをプログラムの下記に設定
   //wifiルーター設定 
    const char* SSID = "JOHOTSUSHIN1";           //使用するWiFiルーターのSSIDを設定、ただし、2.4GHzしか使えない。
    const char* PASSWORD = "johotsushin_5970";   //WiFiルーターのパスワード

   2. USBケーブルを電源と本体に接続
   3. プログラムを本体に書き込む 
   4. 起動すると、VFDに左側から1,2,3,4と数字が表示される 
     この時、WiFiルーターに接続を試みている
     接続ができないときは、ブザー音が鳴る
     WiFiの設定を見直す、もしくは、
     手動で年月日等を設定する画面が表示されるため後のボタンで設定を行う
     ボタンは、T:増、A:減、C:決定
   5. VFDには、年月日、時間、温度と湿度の順番に5秒間隔で表示が繰り返される
 
 */

 
//関数プロトタイプ宣言
//桁毎に数値データを渡してVFDに表示する
void VFD_display(uint8_t *data, uint8_t dot ); 
uint8_t VFD_data[8]={11,11,11,11,11,11,11,11};
//表示機の左側の桁から順に設定、0-9は数字、11はブランク表示
uint8_t VFD_dot = B00000000;
//ドットの設定は
//左側の桁からドットありなら1,無なら0とし
//8桁分を2進数的に表す


//年月日
uint16_t year;
uint8_t month;
uint8_t day;

//タイマー処理
hw_timer_t * timer = NULL; //timer 初期化
volatile uint8_t time_seconds = 0;
volatile uint8_t time_minutes  = 0;
volatile uint8_t time_hours = 0;


//WiFi関係
#include <WiFi.h>

//wifiルーター設定 
const char* SSID = "*************";           //使用するWiFiルーターのSSIDを設定、ただし、2.4GHzしか使えない。
const char* PASSWORD = "*************";   //WiFiルーターのパスワード

//NTPの設定
const char* NTP_SERVER = "ntp.nict.jp";   //ネットワーク・タイム・プロトコルサーバーのURL
const char* TZ = "JST-9";                 //タイムゾーン

//WiFi接続
bool connectWiFi(const char* ssid, const char* password) {
  uint8_t count = 0;
  
  //Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    VFD_data[count] = count;
    VFD_dot = 0x01 << (7-count);
    for(int i=0;i<500;i++){
      VFD_display(VFD_data, VFD_dot);
      delay(1);
    }
    if(count++ == 7) {
      //Serial.println("CONNECTED FAILED");
      return false;
    }
  }
  //Serial.println("\nCONNECTED");
  return true;
}

void disconnectWiFi(void) {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

//NTPサーバーから時刻を取得、設定する
bool setRTC(const char* tz, const char* server) {
  static struct tm timeInfo;

  connectWiFi(SSID, PASSWORD);
  configTzTime(tz, server);
  const bool isSucceeded = getLocalTime(&timeInfo);
  if (isSucceeded) {
//    setRTCDate(timeInfo);
//    setRTCTime(timeInfo);
    time_hours = timeInfo.tm_hour;
    time_minutes = timeInfo.tm_min;
    time_seconds = timeInfo.tm_sec;
    year =  timeInfo.tm_year + 1900;
    month = timeInfo.tm_mon + 1;
    day = timeInfo.tm_mday;
    //Serial.printf("%d/%d/%d  %d:%d:%d\n",year,month,day,time_hours,time_minutes,time_seconds);

  }
  //disconnectWiFi();
  return isSucceeded;
}

//タイマー割込みサービスルーチン
void IRAM_ATTR onTimer() {
  if(time_seconds < 59)time_seconds++;
  else {
    time_seconds = 0;
    if(time_minutes < 59) time_minutes++;
    else {
      time_minutes = 0;
      if(time_hours < 23) time_hours++;
      else {
        time_hours = 0;
      }
    }
  }
  //1日に一回時刻合わせ
  if(time_hours == 0 && time_minutes == 0 && time_seconds == 10 ){
    setRTC(TZ, NTP_SERVER);
  }
}

//温湿度センサー関連
#include <DHT20.h>
DHT20 dht = DHT20();
double tmp = 25.0;
double hum = 60.0;

//スイッチ
#define SW1 14  // SW1を接続するIO番号
#define SW2 12  // SW2を接続するIO番号
#define SW3 13  // SW3を接続するIO番号

//ブザー
#define BUZZER 27   // ブザーを接続するIO番号
int melo = 80;

// 74HC595 シリアルパラレルIC関連
// 74HC595 pin        QA,QB,QC,QD,QE,QF,QG,QH 
// 7segment Mapping   a, b, c, d, e, f, g ,D.P

int tDelay = 0.5;   // 0.5ms遅延設定
#define rclkPin  33     // (33) ST_CP [RCLK] on 74HC595
#define srclkPin  32    // (32)  SH_CP [SRCLK] on 74HC595
#define dsPin  25       // (25) DS [SER] on 74HC595
#define dig1  4         // DIG1を26番ピンに割り当て
#define dig2  16        // DIG2を4番ピンに割り当て
#define dig3  17        // DIG3を16番ピンに割り当て
#define dig4  18        // DIG4を17番ピンに割り当て
#define dig5  19        // DIG5を18番ピンに割り当て
#define dig6  3         // DIG6を19番ピンに割り当て  RxD
#define dig7  1         // DIG7を3番ピンに割り当て   TxD
#define dig8  23        // DIG8を1番ピンに割り当て
#define DP    10        // DP

// seven_ledsをbyte型として定義
// 配列にDisplay表示用のデータ0~9と全消灯を設定
// 1 = LED on, 0 = LED off
byte seven_leds[12] =       { B11101011,  // 0
                              B01100000,  // 1
                              B11000111,  // 2
                              B11100101,  // 3
                              B01101100,  // 4
                              B10101101,  // 5
                              B10101111,  // 6
                              B11100000,  // 7
                              B11101111,  // 8
                              B11101101,  // 9
                              B00010000,  // D.P 
                              B00000000,  // OFF
                             };

//シフトレジスタ部分を関数化
void funcShiftReg(int x)
{
  digitalWrite(rclkPin, LOW);                         //送信中のRCLKをLowにする
  shiftOut(dsPin, srclkPin, LSBFIRST, seven_leds[x]); //シフト演算を使って点灯するLEDを選択
  digitalWrite(rclkPin, HIGH);                        //送信終了後RCLKをHighにする
}


//VFDに数字を表示する関数  
void VFD_display(uint8_t *data, uint8_t dot ){

  //DIG1の表示
  delay(tDelay);
  digitalWrite(dig1, HIGH); 
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, LOW);
  digitalWrite(dig4, LOW);
  digitalWrite(dig5, LOW); 
  digitalWrite(dig6, LOW);
  digitalWrite(dig7, LOW);
  digitalWrite(dig8, LOW);
  //DIG1に表示
  funcShiftReg(data[0]); 
  if(dot & 0x80)funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(11);           //信号初期化

  
  //DIG2の表示
  delay(tDelay);
  digitalWrite(dig1, LOW); 
  digitalWrite(dig2, HIGH);
  digitalWrite(dig3, LOW);
  digitalWrite(dig4, LOW);
  digitalWrite(dig5, LOW); 
  digitalWrite(dig6, LOW);
  digitalWrite(dig7, LOW);
  digitalWrite(dig8, LOW);
  //DIG2に表示
  funcShiftReg(data[1]);
  if(dot & 0x40)funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(11);           //信号初期化


  //DIG3の表示
  delay(tDelay);
  digitalWrite(dig1, LOW); 
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, HIGH);
  digitalWrite(dig4, LOW);
  digitalWrite(dig5, LOW); 
  digitalWrite(dig6, LOW);
  digitalWrite(dig7, LOW);
  digitalWrite(dig8, LOW);
  //DIG3に表示
  funcShiftReg(data[2]); 
  if(dot & 0x20)funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(11);           //信号初期化


  //DIG4の表示
  delay(tDelay);
  digitalWrite(dig1, LOW); 
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, LOW);
  digitalWrite(dig4, HIGH);
  digitalWrite(dig5, LOW); 
  digitalWrite(dig6, LOW);
  digitalWrite(dig7, LOW);
  digitalWrite(dig8, LOW);
  //DIG4に表示
  funcShiftReg(data[3]);
  if(dot & 0x10)funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(11);           //信号初期化

  //DIG5の表示
  delay(tDelay);
  digitalWrite(dig1, LOW); 
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, LOW);
  digitalWrite(dig4, LOW);
  digitalWrite(dig5, HIGH); 
  digitalWrite(dig6, LOW);
  digitalWrite(dig7, LOW);
  digitalWrite(dig8, LOW);
  //DIG5に表示
  funcShiftReg(data[4]);
  if(dot & 0x08)funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(11);           //信号初期化


 //DIG6の表示
  delay(tDelay);
  digitalWrite(dig1, LOW); 
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, LOW);
  digitalWrite(dig4, LOW);
  digitalWrite(dig5, LOW); 
  digitalWrite(dig6, HIGH);
  digitalWrite(dig7, LOW);
  digitalWrite(dig8, LOW);
  //DIG6に表示
  funcShiftReg(data[5]);
  if(dot & 0x04)funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(11);           //信号初期化

  //DIG7の表示
  delay(tDelay);
  digitalWrite(dig1, LOW); 
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, LOW);
  digitalWrite(dig4, LOW);
  digitalWrite(dig5, LOW); 
  digitalWrite(dig6, LOW);
  digitalWrite(dig7, HIGH);
  digitalWrite(dig8, LOW);
  //DIG7に表示
  funcShiftReg(data[6]); 
  if(dot & 0x02)funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(11);           //信号初期化


 //DIG8の表示
  delay(tDelay);
  digitalWrite(dig1, LOW); 
  digitalWrite(dig2, LOW);
  digitalWrite(dig3, LOW);
  digitalWrite(dig4, LOW);
  digitalWrite(dig5, LOW); 
  digitalWrite(dig6, LOW);
  digitalWrite(dig7, LOW);
  digitalWrite(dig8, HIGH);
  //DIG8に表示
  funcShiftReg(data[7]);
  if(dot & 0x01)funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(11);           //信号初期化

}

//VFDに年月日を表示する関数
void date_display(uint16_t year, uint8_t month, uint8_t day ){
    //VFDに表示
    //年の計算
    VFD_data[0] = uint8_t(year / 1000);
    VFD_data[1] = uint8_t((year % 1000) / 100);
    VFD_data[2] = uint8_t((year % 100) / 10);
    VFD_data[3] = uint8_t((year % 10));

    //月の計算
    VFD_data[4] = month / 10;
    VFD_data[5] = month % 10;

    //日の計算
    VFD_data[6] = day / 10;
    VFD_data[7] = day % 10;

    //ドットの位置
    VFD_dot = 0x14;

    //VFD表示
    VFD_display(VFD_data, VFD_dot);

}

//VFDに時間を表示する関数
void time_display(uint8_t hours, uint8_t minutes, uint8_t seconds ){

    //VFDに表示
    //時の計算
    VFD_data[0] = 11; //ブランク表示
    VFD_data[1] = 11; //ブランク表示
    VFD_data[2] = hours / 10;
    VFD_data[3] = hours % 10;

    //分の計算
    VFD_data[4] = minutes / 10;
    VFD_data[5] = minutes % 10;

    //秒の計算
    VFD_data[6] = seconds / 10;
    VFD_data[7] = seconds % 10;

    //ドットの位置
    VFD_dot = 0x14;

    //VFD表示
    VFD_display(VFD_data, VFD_dot);

}

//VFDに温度湿度を表示する関数
void sensor_display(double tmp_d, double hum_d){

  //VFDに表示
  //温度の計算
  VFD_data[0] = uint8_t(tmp_d / 10);
  VFD_data[1] = (uint8_t) tmp_d % 10;
  VFD_data[2] = (uint8_t) tmp_d * 10 % 10;
  VFD_data[3] = 11; //ブランク表示
  VFD_data[4] = 11; //ブランク表示

  //湿度の計算
  VFD_data[5] = uint8_t(hum_d / 10);
  VFD_data[6] = (uint8_t) hum_d % 10;
  VFD_data[7] = (uint8_t) hum_d * 10 % 10;
 
  //ドットの位置
  VFD_dot = 0x42;

  //VFD表示
  VFD_display(VFD_data, VFD_dot);

}

//年月日の設定
void setup_date(void){
  
  year = 2023;
  month = 10;
  day = 1;

  //年の設定
  while(1){  
    if(digitalRead(SW1)==HIGH){
      if(year < 2100) year++;
      delay(500);
      while(1){
        if(digitalRead(SW1)==HIGH){
          if(year + 5 <= 2100){
            year +=5;
          } else {
            year = 2000;
          }
        } else {
          break;
        }
        delay(100);
      }
    }
    if(digitalRead(SW2)==HIGH){
      if(year > 2000) year--;
      delay(500);
      while(1){
        if(digitalRead(SW2)==HIGH){
          if(year - 5 >= 2000){
            year -=5;
          } else {
            year = 2100;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    date_display(year,month,day);

    if(digitalRead(SW3)==HIGH)break;
  }
  tone(BUZZER,2000,80) ;
  delay(500);
  
  //月の設定
  while(1){   
    if(digitalRead(SW1)==HIGH){
      if(month < 12) month++;
      else month = 1;
      delay(500);
      while(1){
        if(digitalRead(SW1)==HIGH){
          if(month + 3 <= 12){
            month +=3;
          } else {
            month = 1;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    if(digitalRead(SW2)==HIGH){
      if(month > 1) month--;
      else month = 12;
      delay(500);
      while(1){
        if(digitalRead(SW2)==HIGH){
          if(month - 3 >= 1){
            month -= 3;
          } else {
            month = 12;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    date_display(year,month,day);

    if(digitalRead(SW3)==HIGH)break;
  }
  tone(BUZZER,2000,80) ;
  delay(500);
  
  //日の設定
  while(1){  
    if(digitalRead(SW1)==HIGH){
      if(day < 31) day++;
      else day = 1;
      delay(500);
      while(1){
        if(digitalRead(SW1)==HIGH){
          if(day + 5 <= 31){
            day += 5;
          } else {
            day = 1;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    if(digitalRead(SW2)==HIGH){
      if(day > 1) day--;
      else day = 31;
      delay(500);
      while(1){
        if(digitalRead(SW2)==HIGH){
          if(day - 5 >= 1){
            day -= 5;
          } else {
            day = 31;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    date_display(year,month,day);
    
    if(digitalRead(SW3)==HIGH)break;
  }
  tone(BUZZER,2000,80) ;
  delay(500);

}

//時間の設定
void setup_time(){
  uint8_t hours = 0;
  uint8_t minutes = 0;
  uint8_t seconds = 0;

  //時の設定
  while(1){  
    if(digitalRead(SW1)==HIGH){
      if(hours < 23) hours++;
      else hours = 0;
      delay(500);
      while(1){
        if(digitalRead(SW1)==HIGH){
          if(hours + 3 <= 23){
            hours +=3;
          } else {
            hours = 0;
          }
        }else {
          break;
        }
        delay(100);

      }
    }
    if(digitalRead(SW2)==HIGH){
      if(hours > 0) hours--;
      else hours = 23;
      delay(500);
      while(1){
        if(digitalRead(SW2)==HIGH){
          if(hours - 3 >= 0){
            hours -= 3;
          } else {
            hours = 23;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    time_display(hours,minutes,seconds);

    if(digitalRead(SW3)==HIGH)break;
  }
  tone(BUZZER,2000,80) ;
  delay(500);
  
  //分の設定
  while(1){   
    if(digitalRead(SW1)==HIGH){
      if(minutes < 59) minutes++;
      else minutes = 0;
      delay(500);
      while(1){
        if(digitalRead(SW1)==HIGH){
          if(minutes + 5 <= 59){
            minutes += 5;
          } else {
            minutes = 0;
          }
        } else {
          break;
        }
        delay(100);
      }
    }
    if(digitalRead(SW2)==HIGH){
      if(minutes > 0) minutes--;
      else minutes = 59;
      delay(500);
      while(1){
        if(digitalRead(SW2)==HIGH){
          if(minutes - 5 >= 0){
            minutes -= 5;
          } else {
            minutes = 59;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    time_display(hours,minutes,seconds);

    if(digitalRead(SW3)==HIGH)break;
  }
  tone(BUZZER,2000,80) ;
  delay(500);
  
  //秒の設定
  while(1){  
    if(digitalRead(SW1)==HIGH){
      if(seconds < 59) seconds++;
      else seconds = 0;
      delay(500);
      while(1){
        if(digitalRead(SW1)==HIGH){
          if(seconds + 5 <= 59){
            seconds += 5;
          } else {
            seconds = 0;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    if(digitalRead(SW2)==HIGH){
      if(seconds > 0) seconds--;
      else seconds = 59;
      delay(500);
      while(1){
        if(digitalRead(SW2)==HIGH){
          if(seconds - 5 >= 0){
            seconds -= 5;
          } else {
            seconds = 59;
          }
        }else {
          break;
        }
        delay(100);
      }
    }
    time_display(hours,minutes,seconds);
    
    if(digitalRead(SW3)==HIGH)break;
  }
  tone(BUZZER,2000,80) ;
  delay(500);

  time_seconds = seconds;
  time_minutes = minutes;
  time_hours = hours;
  
}

//起動後に一度だけ実行される関数
void setup() 
{
  delay(100);
  
  //温湿度センサー
  dht.begin(); //default: 21,22

  //シリアル通信
//  Serial.begin(115200);
//  while (!Serial); //wait for serial connected.
//  delay(1000);

  //タイマー設定
  timer = timerBegin(0, 80, true); //timer=1us
  timerAttachInterrupt(timer, &onTimer, true); //タイマー割込みに使う関数を登録
  timerAlarmWrite(timer, 1000000, true); // タイマー割込み時間の設定us単位 1s
  timerAlarmEnable(timer);
  
  //スイッチ関連の入力処理
  pinMode(SW1, INPUT);    // 入力に指定
  pinMode(SW2, INPUT);    // 入力に指定
  pinMode(SW3, INPUT);    // 入力に指定
  
  //ブザーの出力処理
  pinMode(BUZZER, OUTPUT);  // 出力に指定

  //VFD関連の出力処理
  pinMode(rclkPin, OUTPUT);   //11番ピンをOUTPUTとして定義
  pinMode(dsPin, OUTPUT);     //12番ピンをOUTPUTとして定義
  pinMode(srclkPin, OUTPUT);  //9番ピンをOUTPUTとして定義
  pinMode(dig1, OUTPUT);      //1番ピンをOUTPUTとして定義
  pinMode(dig2, OUTPUT);      //2番ピンをOUTPUTとして定義
  pinMode(dig3, OUTPUT);      //3番ピンをOUTPUTとして定義
  pinMode(dig4, OUTPUT);      //4番ピンをOUTPUTとして定義
  pinMode(dig5, OUTPUT);      //5番ピンをOUTPUTとして定義
  pinMode(dig6, OUTPUT);      //6番ピンをOUTPUTとして定義
  pinMode(dig7, OUTPUT);      //7番ピンをOUTPUTとして定義
  pinMode(dig8, OUTPUT);      //8番ピンをOUTPUTとして定義

  digitalWrite(dig1, LOW);    //1番ピンをHIGH DI1 OFF
  digitalWrite(dig2, LOW);    //2番ピンをHIGH DI2 OFF
  digitalWrite(dig3, LOW);    //3番ピンをHIGH DI3 OFF
  digitalWrite(dig4, LOW);    //4番ピンをHIGH DI4 OFF
  digitalWrite(dig5, LOW);    //5番ピンをHIGH DI5 OFF
  digitalWrite(dig6, LOW);    //6番ピンをHIGH DI6 OFF
  digitalWrite(dig7, LOW);    //7番ピンをHIGH DI7 OFF
  digitalWrite(dig8, LOW);    //8番ピンをHIGH DI8 OFF
  funcShiftReg(11);           //信号初期化

  //時刻設定
  // NTPサーバーから時刻を取得
  //Serial.printf("Wi-Fi router SSID : %s\n",SSID);
  //Serial.printf("NTP Server : %s\n", NTP_SERVER);
  const bool result = setRTC(TZ, NTP_SERVER);
  //Serial.printf("Result : %s\n", result == true ? "Succeeded" : "Failed");

  //NTPサーバーから時刻を取得できない場合は、手動設定
  if(result == false) {
    for(int i=0;i<10;i++){
      tone(BUZZER,2000,80) ;
      delay(80) ;
    }
    setup_date();
    setup_time();
    //Serial.println("NTP error");
  }

  tone(BUZZER,2000,80) ;
  delay(80) ;
  tone(BUZZER,1000,80) ;
  delay(80) ;

}

void loop() 
{
  while(1){
    //日付表示 10秒間
    for(uint16_t i=0; i<10000;i++){
      date_display(year,month,day);
    }

    //時間表示 10秒間
    for(uint16_t i=0; i<10000;i++){
      time_display(time_hours,time_minutes,time_seconds);
    }

    //温度湿度表示 10秒間
    if(millis() - dht.lastRead() >= 1000) {
      if(DHT20_OK == dht.read()){
        tmp = dht.getTemperature();
        hum = dht.getHumidity();

        for(uint16_t i=0; i<10000;i++){
          sensor_display(tmp, hum);
        }
      }
    }
  }
}

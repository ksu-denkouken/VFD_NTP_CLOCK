

// 74HC595 pin        QA,QB,QC,QD,QE,QF,QG,QH 
// 7segment Mapping   a, b, c, d, e, f, g ,D.P

int BUZZER = 27;
int tDelay = 0.5;   // 0.5ms遅延設定
int rclkPin = 33;   // (33) ST_CP [RCLK] on 74HC595
int srclkPin = 32;   // (32)  SH_CP [SRCLK] on 74HC595
int dsPin = 25;     // (25) DS [SER] on 74HC595
int dig1 = 4;       // DIG1を26番ピンに割り当て
int dig2 = 16;       // DIG2を4番ピンに割り当て
int dig3 = 17;       // DIG3を16番ピンに割り当て
int dig4 = 18;       // DIG4を17番ピンに割り当て
int dig5 = 19;       // DIG5を18番ピンに割り当て
int dig6 = 3;       // DIG6を19番ピンに割り当て
int dig7 = 1;       // DIG7を3番ピンに割り当て
int dig8= 23;       // DIG8を1番ピンに割り当て

int a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;
int f = 0;
int g = 0;
int h = 0;
int DP = 10;
int NUL = 11;
int num =0;

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
  
void setup() 
{
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
  funcShiftReg(33);           //信号初期化

  tone(BUZZER,2000,80) ;
  delay(80) ;
  tone(BUZZER,1000,80) ;
  delay(80) ;

}

void loop() 
{
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
  //DIG1にaを表示
  funcShiftReg(a);
  delay(tDelay);
  funcShiftReg(33);

  
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
  //DIG2にbを表示
  funcShiftReg(b);
  delay(tDelay);
  funcShiftReg(33);


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
  //DIG3にcを表示
  funcShiftReg(c);
  delay(tDelay);
  funcShiftReg(33);


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
  //DIG4にdを表示
  funcShiftReg(d);
  funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(33);


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
  //DIG5にeを表示
  funcShiftReg(e);
  delay(tDelay);
  funcShiftReg(33);


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
  //DIG6にfを表示
  funcShiftReg(f);
  funcShiftReg(DP);
  delay(tDelay);
  funcShiftReg(33);


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
  //DIG7にgを表示
  funcShiftReg(g);
  delay(tDelay);
  funcShiftReg(33);


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
  //DIG8にhを表示
  funcShiftReg(h);
  delay(tDelay);
  funcShiftReg(33);

  a = num / 10000000;
  b = (num - (a*10000000)) / 1000000;
  c = (num - (a*10000000) - (b*1000000)) / 100000;
  d = (num - (a*10000000) - (b*1000000) - (c*100000)) / 10000;
  e = (num - (a*10000000) - (b*1000000) - (c*100000) - (d*10000)) / 1000;
  f = (num - (a*10000000) - (b*1000000) - (c*100000) - (d*10000) - (e * 1000))/ 100;
  g = (num - (a*10000000) - (b*1000000) - (c*100000) - (d*10000) - (e*1000) - (f * 100) - (h * 1))/ 10;
  h = (num - (a*10000000) - (b*1000000) - (c*100000) - (d*10000) - (e*1000) - (f * 100) - (g * 10)) ;
 
num=20231015;

}
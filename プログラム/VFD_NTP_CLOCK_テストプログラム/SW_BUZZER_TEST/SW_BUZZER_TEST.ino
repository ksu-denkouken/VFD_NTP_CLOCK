#define SW1 14  // SW1を接続するIO番号
#define SW2 12  // SW2を接続するIO番号
#define SW3 13  // SW3を接続するIO番号
#define BUZZER 27   // ブザーを接続するIO番号
int melo = 80;

// 初期化処理部分
void setup() {
  pinMode(SW1, INPUT);    // 入力に指定
  pinMode(SW2, INPUT);    // 入力に指定
  pinMode(SW3, INPUT);    // 入力に指定
  pinMode(BUZZER, OUTPUT);  // 出力に指定
}

// プログラム本体(無限ループ)
void loop() {
  if (digitalRead(SW1)==HIGH){
    // スイッチが押されていたら発音
    tone(BUZZER,2000,150) ;
    delay(150) ;
    tone(BUZZER,1000,150) ;
    delay(150) ;

  delay(250);

  }
  else{
    // スイッチが押されていないなら消灯
     digitalWrite(BUZZER,LOW);
  }

  if (digitalRead(SW2)==HIGH){
    // スイッチが押されていたら発音
    tone(BUZZER,2000,80) ;
    delay(80) ;
    tone(BUZZER,1000,80) ;
    delay(80) ;

  delay(250);

  }
  else{
    // スイッチが押されていないなら消灯
     digitalWrite(BUZZER,LOW);
  }

 if (digitalRead(SW3)==HIGH){
    // スイッチが押されていたら発音
    tone(BUZZER,2000,40) ;
    delay(40) ;
    tone(BUZZER,1000,40) ;
    delay(40) ;

  delay(250);

  }
  else{
    // スイッチが押されていないなら消灯
     digitalWrite(BUZZER,LOW);
  }

  delay(50);
}
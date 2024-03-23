




// 7 segments
const int SEG_A=23;
const int SEG_B=22;
const int SEG_C=21;
const int SEG_D=19;
const int SEG_E=18;
const int SEG_F=17;
const int SEG_G=16;

// com for 7segments
#define CLOCK GPIO_NUM_4
#define RESET GPIO_NUM_0

hw_timer_t * timer = NULL; //timer 初期化
int msec; //msec カウンター用グローバル変数
int msec_10;  // 10msec カウンター
int inc_keta;
int data = 0;

// DIO initialization.
void dio_init() {

  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);

  pinMode(CLOCK, OUTPUT);
  pinMode(RESET, OUTPUT);
  

  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
}


void seg_cont(int com) {
  switch (com) {
    case 0:
      digitalWrite(RESET, HIGH);
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;
    case 1:
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;
    case 2:
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;
    case 3:
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;
    case 4:
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;
    case 5:
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;
    case 6:
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;  
    case 7:
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;  
    case 8:
      digitalWrite(RESET, LOW);
      digitalWrite(CLOCK, HIGH);
      digitalWrite(CLOCK, LOW);
      break;  
    default:
      digitalWrite(CLOCK, LOW);
      digitalWrite(RESET, HIGH);
  }
}

void disp_9digits(int num, int keta) {
  int k100000000;
  int k10000000;
  int k1000000;
  int k100000;
  int k10000;
  int k1000;
  int k100;
  int k10;
  int k1;
  int dp;

  if (num < 0) {
    num * -1;
    dp = true;
  } else {
    dp = false;
  }
  k100000000 = num / 100000000;
  k10000000 = (num - (k100000000*100000000)) / 10000000;
  k1000000 = (num - (k100000000*100000000) - (k10000000*10000000)) / 1000000;
  k100000 = (num - (k100000000*100000000) - (k10000000*10000000) - (k1000000*1000000)) / 100000;
  k10000 = (num - (k100000000*100000000) - (k10000000*10000000) - (k1000000*1000000) - (k100000*100000)) / 10000;
  k1000 = (num - (k100000000*100000000) - (k10000000*10000000) - (k1000000*1000000) - (k100000*100000) - (k10000 * 10000))/ 1000;
  k100  = (num - (k100000000*100000000) - (k10000000*10000000) - (k1000000*1000000) - (k100000*100000) - (k10000*10000) - (k1000 * 1000) - (k10 * 10))/ 100;
  k10   = (num - (k100000000*100000000) - (k10000000*10000000) - (k1000000*1000000) - (k100000*100000) - (k10000*10000) - (k1000 * 1000) - (k100 * 100)) / 10;
  k1    = num - (k100000000*100000000) - (k10000000*10000000) - (k1000000*1000000) - (k100000*100000) - (k10000*10000) - (k1000 * 1000) - (k100 * 100) - (k10 * 10);

  //  Serial.printf("k1000:%d k100:%d k10:%d k1:%d\n", k1000, k100, k10, k1);
  if (dp == true) digitalWrite(SEG_G, LOW);
  else digitalWrite(SEG_G, HIGH);

  switch (keta) {
    case 0:
    disp_digit(k100000000);
      seg_cont(0);
      break;
    case 1:
    disp_digit(k10000000);
      seg_cont(1);
      break;
    case 2:
    disp_digit(k1000000);
      seg_cont(2);
      break;
    case 3:
      disp_digit(k100000);
      seg_cont(3);
      break;
    case 4:
      disp_digit(k10000);
      seg_cont(4);
      break;
    case 5:
      disp_digit(k1000);
      seg_cont(5);
      break;
    case 6:
      disp_digit(k100);
      seg_cont(6);
      break;
    case 7:
      disp_digit(k10);
      seg_cont(7);
      break;
    case 8:
      disp_digit(k1);
      seg_cont(8);
      break;
  }
  delay(1);
}

void disp_digit(int num) {
  switch (num) {
    case 0:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, LOW);
      break;
    case 1:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      break;
    case 2:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, HIGH);
      break;
    case 3:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, HIGH);
      break;
    case 4:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, HIGH);
      break;
    case 5:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, HIGH);
      break;
    case 6:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, HIGH);
      break;
    case 7:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      break;
    case 8:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, HIGH);
      break;
    case 9:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, HIGH);
      break;
    case 10:  // A
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      break;
    case 11:  //b
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      break;
    case 12: //C
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, HIGH);
      break;
    case 13: //d
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, LOW);
      break;
    case 14:  //E
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      break;
    case 15:  //F
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      break;
    default:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, LOW);
      break;
  }
}

void serial_setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println(""); // to separate line
}

void IRAM_ATTR msecond_10() {
  inc_keta++;
  if (inc_keta > 8) inc_keta = 0;
}


void setup() {

  serial_setup();
  dio_init();

  // msec timer start
  timer = timerBegin(0, 80, true); //timer=1us
  timerAttachInterrupt(timer, &msecond_10, true);
  timerAlarmWrite(timer, 1000, true); // 10ms
  timerAlarmEnable(timer);
}

int pre_inc_keta;

void loop() {

  Serial.printf("%d %d\n", msec_10, inc_keta);
  disp_9digits(data, inc_keta);

  data++;
  if(data > 999999999) data = 0;
}

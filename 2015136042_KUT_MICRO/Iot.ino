#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>//시리얼 통신 라이브러리 호출
#include <LiquidCrystal.h> // LCD 라이브러리
#include <WiFi.h>

#include "IoTMakers.h"
#include "Shield_Wrapper.h"


long time =millis();

int trig=4; //초음파출력 핀번호
int echo=5; //초음파입력 핀번호
int piezo=6; //부저 핀번호
LiquidCrystal lcd(26,28,30,32,34,36); //LCD 핀번호
//LiquidCrystal 라이브러리를 초기화하는 명령어
// lcd(RS,E,D4,D5,D6,D7)
int inputPin=13; //인체 감지센서 핀번호

bool val=true; // 인체감지센서 초기화
int distance = 0; //초음파 거리 초기화
int min_range= 10;//초음파 감지 최소값
int max_range= 20;//초음파 감지 최대값
bool isDetected = false;
bool state=false;
bool bf_isDetected=false;
bool buzzer=false;
bool buzzerToggle = false;
void mycb_numdata_handler(char *tagid, int numval);

/*
Arduino Shield
*/

Shield_Wrapper  g_shield;

#define SDCARD_CS 4
void sdcard_deselect()
{
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); //Deselect the SD card
}
void init_shield()
{
  sdcard_deselect();
  
  const char* WIFI_SSID = "iptime";
 // const char* WIFI_PASS = "rlatkddus";
  g_shield.begin(WIFI_SSID);

  g_shield.print();
}


/*
IoTMakers
*/
IoTMakers g_im;

const char deviceID[]   = "xhdtn8D1477966355349";
const char authnRqtNo[] = "gprr87jbl";
const char extrSysID[]  = "OPEN_TCP_001PTL001_1000002977";

void init_iotmakers()
{
  Client* client = g_shield.getClient();
  if ( client == NULL ) {
    Serial.println(F("No client from shield."));
    while(1);
  }

  g_im.init(deviceID, authnRqtNo, extrSysID, *client);
  
  g_im.set_numdata_handler(mycb_numdata_handler);

  // IoTMakers 서버 연결
  Serial.println(F("connect()..."));
  while ( g_im.connect() < 0 ){
    Serial.println(F("retrying."));
    delay(1000);
  }

  // Auth

  Serial.println(F("auth."));
  while ( g_im.auth_device() < 0 ) {
    Serial.println(F("fail"));
    while(1);
  }

  Serial.print(F("FreeRAM="));Serial.println(g_im.getFreeRAM());
}

int send_sonic();
int send_IR();
int send_isDetected();
void IR_sensor();
void ultraSonic();
void piezo_buzzer(bool buzzer);
void Display_LCD(int n);

void setup() 
{
  Serial.begin(115200);
    while ( !Serial )  {
    ;
  }
pinMode(trig,OUTPUT);
pinMode(echo,INPUT); 
pinMode(inputPin,INPUT);
pinMode(piezo,OUTPUT);

lcd.begin(16,2); //lcd.begin(행,열)
//LCD의 높낮이를 설정하는 명령어
lcd.print("WIFI ..."); //초기 LCD 상태
//lcd에 텍스트를 출력하는 명령어
lcd.setCursor(0,1);//lcd의 현재 커서를 설정하는 명령어
//첫번째 열 두번째 행에 커서를 위치시킴
lcd.print("NOT CONNECTED");

  init_shield();
  
  init_iotmakers();


}

void loop()
{
  static unsigned long tick = millis();

  // 1초 주기로 센서 정보 송신
  if ( ( millis() - tick) > 1000 )
  {
      ultraSonic();//초음파 신호
      IR_sensor();
     Serial.println();
     
    if(state){//경계 상태일 때
    isDetected=((distance >= min_range && distance <= max_range)
    || val==HIGH);
  
Display_LCD(0);//ON 상태의 LCD 로드
 if(isDetected){
    //piezo_buzzer(buzzer);//isDetected 값이 참이면 소음 발생   // 아래에 따로 정리함.
    Display_LCD(2); //침입감지 상태의 LCD 로드
}
 send_isDetected();
 }
    else // 침입이 감지되지 않았을 때
    {
    Display_LCD(1);//OFF 상태의 LCD 로드 
    }
    
    send_IR();
    send_sonic();
   

    tick = millis();
  }

  if(isDetected == true && millis() % 250 == 0) {
    if(buzzerToggle) {
      digitalWrite(piezo,HIGH);
    }
    else {
      digitalWrite(piezo,LOW);
    }
  }
  
  // IoTMakers 서버 수신처리 및 keepalive 송신
  g_im.loop();
  bf_isDetected=isDetected;//이전의 기록값기록
}


int send_IR(){

  char* ir_result="*";
  if(val==true)
  strcpy(ir_result,"O");
  else strcpy(ir_result,"X");

  Serial.print(F("IR : ")); Serial.println(ir_result);
  if ( g_im.send_strdata("ir", ir_result) < 0 ) {
      Serial.println(F("fail"));  
    return -1;
  }
  return 0;   
}
int send_isDetected()
{
if(bf_isDetected!=isDetected){
  char* is_Detected="*";
  if(isDetected==true)
  strcpy(is_Detected,"O");
  else strcpy(is_Detected,"X");

  Serial.print(F("isDetected : ")); Serial.println(is_Detected);
  if ( g_im.send_strdata("isDetected", is_Detected) < 0 ) {
      Serial.println(F("fail"));  
    return -1;
  }
  return 0;   
}
return 0;
}

int send_sonic()
{
  Serial.print(F("sonic : ")); Serial.println(distance);
  if ( g_im.send_numdata("sonic", (double)distance) < 0 ){
      Serial.println(F("fail"));  
    return -1;
  }
  return 0;   
}

void IR_sensor(){
 //인체감지센서
 val=digitalRead(inputPin);//인체감지센서의 데이터를 저장함
  }

void mycb_numdata_handler(char *tagid, double numval)
{
  if ( strcmp(tagid, "state") == 0  )  { 
    Serial.println("my ");
        if(numval==1){
      state=true;
      buzzer=true;
      Serial.println("state : true");
    }
 else {
  state=false;
  buzzer=false;
Serial.println("state : false");
    }
  }
}
void ultraSonic(){
  digitalWrite(trig,HIGH); // 센서에 Trig 신호 입력
  delayMicroseconds(10);//10us 정도 유지
  digitalWrite(trig,LOW); // Trig 신호 off
  
  distance = pulseIn(echo,HIGH)*17/1000;//Echo pin: HIGH->Low 간격 측정 
  //17/1000 은 들어오는 값을 cm형태로 나타내기 위한 값
  //pulseIn(핀,값) 
  delay(100);
}

void piezo_buzzer(bool buzzer){
    if(time + 1000 <millis()){
      digitalWrite(piezo,HIGH);
    delay(250);
    digitalWrite(piezo,LOW);
    delay(250);
    digitalWrite(piezo,HIGH);
    delay(250);
    digitalWrite(piezo,LOW);
    delay(250);
    } 
}
void Display_LCD(int n){
  switch (n) {
    case 0 :  //ON 상태
    lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Boundary state");
        lcd.setCursor(0,1);
        lcd.print(": ON ");
        break;
    case 1 :  //OFF 상태
    lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Boundary state");
        lcd.setCursor(0,1);
        lcd.print(": OFF");
        break;
      case 2 :  //침입감지 상태
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Dangerous!");
        break;
      default:
          break;
  }
}

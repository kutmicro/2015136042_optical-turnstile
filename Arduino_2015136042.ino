#include <SoftwareSerial.h>//시리얼 통신 라이브러리 호출
#include <LiquidCrystal.h> // LCD 라이브러리
#include <SPI.h>
#include <WiFi.h>

int trig=4; //초음파출력 핀번호
int echo=5; //초음파입력 핀번호
int piezo=6; //부저 핀번호
LiquidCrystal lcd(7,8,9,10,11,12); //LCD 핀번호
int inputPin=13; //인체 감지센서 핀번호

int val=0; // 인체감지센서 초기화
int distance = 0; //초음파 거리 초기화
int min_range= 10;//초음파 감지 최소값
int max_range= 20;//초음파 감지 최대값
bool isDetected = false;
bool state=false;

char ssid[] = "iptime";
//char pass[] = "secretPassword"; 



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(trig,OUTPUT);
pinMode(echo,INPUT); 
pinMode(inputPin,INPUT);
pinMode(piezo,OUTPUT);

lcd.begin(16,2); //16*2 LCD
lcd.print("Boundary state"); //초기 LCD 상태
lcd.setCursor(0,1);
lcd.print(": OFF");
}

void loop() {


//ultraSonic();//초음파 신호

 if(state){//경계 상태일 때
  
Display_LCD(0);//ON 상태의 LCD 로드
  
 if(distance >= min_range && distance <= max_range ||IR_sensor())
     isDetected = true; //판별 변수에 true저장
      
    piezo_buzzer();//isDetected 값이 참이면 소음 발생

}
    else // 침입이 감지되지 않았을 때
    {
      isDetected = false; 
   
    Display_LCD(1);//OFF 상태의 LCD 로드 
   
    }
}

void ultraSonic(){
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  distance = pulseIn(echo,HIGH)*17/1000;
  Serial.println(distance);
  delay(100);
}

void piezo_buzzer(){
  if(isDetected){
    Display_LCD(2); //침입감지상태
    digitalWrite(piezo,HIGH);
    delay(300);
    digitalWrite(piezo,LOW);
    delay(300);
  }
}
void Display_LCD(int n){
  switch (n) {
    case 0 :  //ON 상태
        lcd.setCursor(0,0);
        lcd.print("Boundary state");
        lcd.setCursor(0,1);
        lcd.print(": ON ");
        break;
    case 1 :  //OFF 상태
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
bool IR_sensor(){
 //인체감지센서
  val=digitalRead(inputPin);//인체감지센서의 데이터를 저장함
  if(val==HIGH)//인체감지시
   return true;//참 리턴
else
return false;//거짓 리턴
  }


#include <SoftwareSerial.h>//시리얼 통신 라이브러리 호출
int inputPin=13; //인체 감지센서 핀번호
bool val=true; // 인체감지센서 초기화
void IR_sensor();

void setup() {
  Serial.begin(9600);
  pinMode(inputPin,INPUT);
}

void loop() {
  IR_sensor();
  if(val==HIGH){
    Serial.println("HIGH");
  }
  else
  Serial.println("LOW");
  
delay(1000);
}
void IR_sensor(){
 //인체감지센서
 val=digitalRead(inputPin);//인체감지센서의 데이터를 저장함
  }

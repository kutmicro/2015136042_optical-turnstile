#include <SoftwareSerial.h>//시리얼 통신 라이브러리 호출
int piezo=6; //부저 핀번호
bool state=false;
void piezo_buzzer(int x);
void setup() {
  Serial.begin(9600);
  pinMode(piezo,OUTPUT);

}

void loop() {
  int x=Serial.read();
 piezo_buzzer(x);

}
void piezo_buzzer(int x){
  if(x==1){
      digitalWrite(piezo,HIGH);
    delay(250);
    digitalWrite(piezo,LOW);
    delay(250); 
  }
}

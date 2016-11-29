#include <SoftwareSerial.h>//시리얼 통신 라이브러리 호출
int piezo=6; //부저 핀번호

void piezo_buzzer();
void setup() {
  Serial.begin(9600);
  pinMode(piezo,OUTPUT);

}

void loop() {

 piezo_buzzer();

}
void piezo_buzzer(){
      digitalWrite(piezo,HIGH);
    delay(250);
    digitalWrite(piezo,LOW);
    delay(250); 
}

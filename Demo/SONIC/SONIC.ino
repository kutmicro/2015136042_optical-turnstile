#include <SoftwareSerial.h>//시리얼 통신 라이브러리 호출
int trig=4; //초음파출력 핀번호
int echo=5; //초음파입력 핀번호

int distance = 0; //초음파 거리 초기화
int min_range= 10;//초음파 감지 최소값
int max_range= 20;//초음파 감지 최대값
void ultraSonic();
void setup() {
  Serial.begin(9600);
pinMode(trig,OUTPUT);
pinMode(echo,INPUT); 
}

void loop() {
  ultraSonic();

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


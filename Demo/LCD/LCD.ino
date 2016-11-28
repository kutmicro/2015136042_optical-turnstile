#include <SoftwareSerial.h>//시리얼 통신 라이브러리 호출
#include <LiquidCrystal.h> // LCD 라이브러리

LiquidCrystal lcd(26,28,30,32,34,36); //LCD 핀번호
//LiquidCrystal 라이브러리를 초기화하는 명령어
// lcd(RS,E,D4,D5,D6,D7)

void setup() {
  Serial.begin(9600);
lcd.begin(16,2); //lcd.begin(행,열)
//LCD의 높낮이를 설정하는 명령어
lcd.print("Set Up"); //초기 LCD 상태
//lcd에 텍스트를 출력하는 명령어
lcd.setCursor(0,1);//lcd의 현재 커서를 설정하는 명령어
//첫번째 열 두번째 행에 커서를 위치시킴
lcd.print("Hello");
}
void Display_LCD(int n);


void loop() {
 int x=Serial.read();
 Display_LCD(x);

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

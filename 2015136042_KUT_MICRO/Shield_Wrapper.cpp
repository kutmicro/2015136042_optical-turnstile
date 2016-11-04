/*
  Shield_Wrapper.cpp - Library for wrapping Ethernet.
  Created by Kiucheol Shin(kiucheol.shin@kt.com), November 20, 2015.
  Released into the public domain.
*/
/*
 F() 매크로 사용

문자열은 메모리의 골치거리 중 하나입니다.
문자열은 플래시 메모리에 프로그램 이미지에 포함되어 탑재된 뒤, 다시 static 변수로 SRAM을 잡아습니다.
이에 Paul Stoffregen of PJRC and Teensyduino 이 만든 간단하고 훌륭한 솔루션이 F() 매크로입니다.
이 매크로는 문자열이 SRAM에 복사되지 않도록 해주며 아두이노가 실행되면서
문자열을 사용할 때 플래시 메모리에서 직접 읽어 사용하도록 해줍니다.
실제로는 PROGMEM 지시어를 사용해서 선언된 변수를 사용하는 것과 같은 원리입니다.
아래 예제처럼 문자열을 F() 로 감싸서 사용하면됩니다
 *
 */
#include <WiFi.h>
#include <SPI.h>
#include "Shield_Wrapper.h"

WiFiClient __client;

Shield_Wrapper::Shield_Wrapper(){
	_status = WL_IDLE_STATUS;
}



void Shield_Wrapper::begin(const char* ssid)
{
	Serial.println(F("begin WiFi!\n")); 
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("WiFi shield not present"); 
		while(true);
	} 
	Serial.print(F("firmware:")); Serial.println(WiFi.firmwareVersion()); 


	while ( _status != WL_CONNECTED) {
		Serial.print(F("connect to "));Serial.println(ssid);
		// WPA/WAP2 네트워크에 연결
		_status = WiFi.begin((char*)ssid);
		delay(1000);
	}
}

void Shield_Wrapper::disconnect(){
	WiFi.disconnect();
}

void Shield_Wrapper::print(){
	Serial.print(F("IP: "));Serial.println(WiFi.localIP());
	Serial.print(F("GW: "));Serial.println(WiFi.gatewayIP());
}

Client* Shield_Wrapper::getClient(){
	return (Client*)&__client;
}

#include <Arduino.h>
#include <SPI.h>

#include <WiFi.h>

#include "IoTMakers.h"
#include "Shield_Wrapper.h"


/*
Arduino Shield
*/

Shield_Wrapper	g_shield;

#define SDCARD_CS	4
void sdcard_deselect()
{
	pinMode(SDCARD_CS, OUTPUT);
	digitalWrite(SDCARD_CS, HIGH); //Deselect the SD card
}
void init_shield()
{
	sdcard_deselect();
	
	const char* WIFI_SSID = "iptime";
	//const char* WIFI_PASS = "new1234!";
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
	if ( client == NULL )	{
		Serial.println(F("No client from shield."));
		while(1);
	}

	g_im.init(deviceID, authnRqtNo, extrSysID, *client);
	g_im.set_numdata_handler(mycb_numdata_handler);
	g_im.set_strdata_handler(mycb_strdata_handler);
	g_im.set_dataresp_handler(mycb_resp_handler);

	// IoTMakers 서버 연결
	Serial.println(F("connect()..."));
	while ( g_im.connect() < 0 ){
		Serial.println(F("retrying."));
		delay(3000);
	}

	// Auth

	Serial.println(F("auth."));
	while ( g_im.auth_device() < 0 ) {
		Serial.println(F("fail"));
		while(1);
	}

	Serial.print(F("FreeRAM="));Serial.println(g_im.getFreeRAM());
}

#define PIN_LED		9

void setup() 
{
	Serial.begin(115200);
  	while ( !Serial )  {
	  ;
	}

	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, HIGH);

	init_shield();
	
	init_iotmakers();

	digitalWrite(PIN_LED, LOW);

}

void loop()
{
	static unsigned long tick = millis();

	// 3초 주기로 센서 정보 송신
	if ( ( millis() - tick) > 3000 )
	{
		digitalWrite(PIN_LED, HIGH);
		send_temperature();
		send_light();
		digitalWrite(PIN_LED, LOW);

		tick = millis();
 	}
  
	// IoTMakers 서버 수신처리 및 keepalive 송신
	g_im.loop();
}


int send_temperature()
{
	int tmpVal = analogRead(A0);
	float voltage = (tmpVal/1024.0) * 5.0;
	float temperature = (voltage - .5) * 100;

	Serial.print(F("Temperature (c): ")); Serial.println(temperature);
	if ( g_im.send_numdata("temperature", (double)temperature) < 0 ) {
  		Serial.println(F("fail"));  
		return -1;
	}
	return 0;   
}

int send_light()
{
	int tmpVal = analogRead(A1);
	int light = tmpVal/4;

	Serial.print(F("Light : ")); Serial.println(light);
	if ( g_im.send_numdata("light", (double)light) < 0 ){
  		Serial.println(F("fail"));  
		return -1;
	}
	return 0;   
}



void mycb_numdata_handler(char *tagid, double numval)
{
	// !!! USER CODE HERE
	//Serial.print(tagid);Serial.print(F("="));Serial.println(numval);
}
void mycb_strdata_handler(char *tagid, char *strval)
{
	// !!! USER CODE HERE
	//Serial.print(tagid);Serial.print(F("="));Serial.println(strval);
  
	if ( strcmp(tagid, "led")==0 && strcmp(strval, "on")==0 )  	
		digitalWrite(PIN_LED, HIGH);
	else if ( strcmp(tagid, "led")==0 && strcmp(strval, "off")==0 )  	
		digitalWrite(PIN_LED, LOW);
}
void mycb_resp_handler(long long trxid, char *respCode)
{
	if ( strcmp(respCode, "100")==0 )
		Serial.println("resp:OK");
	else
		Serial.println("resp:Not OK");
}

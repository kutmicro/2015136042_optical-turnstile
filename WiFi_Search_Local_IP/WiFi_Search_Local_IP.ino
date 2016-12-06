#include <SPI.h>
#include <WiFi.h>
char ssid[] = "egkim";
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  }
  
void loop() {
  Serial.println("Scanning WIFI");
  listNetworks();
  delay(10000);
}

void listNetworks() {
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen:
  //Serial.print("number of available networks:");
 // Serial.println(numSsid);
int count =1;
char a[] = " ) ";
  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    if(strcmp(WiFi.SSID(thisNet),ssid)==0){
    Serial.print(count);
    Serial.print(a);   
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm ");
    WiFi.begin(WiFi.SSID(thisNet));
    Serial.print(WiFi.localIP());
    Serial.println();
   count++;
   WiFi.disconnect();
    //Serial.print("\tEncryption: ");
   // printEncryptionType(WiFi.encryptionType(thisNet));
    }
  }
  Serial.println();
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
  }
}

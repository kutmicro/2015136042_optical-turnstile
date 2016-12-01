#include <SPI.h>
#include <WiFi.h>
char ssid[] = "KUTAP";
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
    Serial.print(" dBm");
    Serial.println();
   count++;
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
/*   utility/wifi_drv.h 의 경로에 존재함
 *    WL_NETWORKS_LIST_MAXNUM : List에 표기될 수 있는 네트워크의 최대 개수
 *    networkItem : RSSI를 나타낼 WIFI
     
     
     
int32_t WiFiDrv::getRSSINetoworks(uint8_t networkItem)
{
  if (networkItem >= WL_NETWORKS_LIST_MAXNUM)
    return NULL;
  int32_t networkRssi = 0;
  WAIT_FOR_SLAVE_SELECT();//와이파이 쉴드 점등 및 소등
    // Send Command
    SpiDrv::sendCmd(GET_IDX_RSSI_CMD, PARAM_NUMS_1);
//GET_IDX_RSSI_CMD  = 0x32
//PARAM_NUMS_1 은 열거체의 원소
    SpiDrv::sendParam(&networkItem, 1, LAST_PARAM);
    //Wait the reply elaboration
    SpiDrv::waitForSlaveReady();
    // Wait for reply
    uint8_t dataLen = 0;
    SpiDrv::waitResponseCmd(GET_IDX_RSSI_CMD, PARAM_NUMS_1, (uint8_t*)&networkRssi, &dataLen);
    SpiDrv::spiSlaveDeselect();
  return networkRssi;
}
#define WAIT_FOR_SLAVE_SELECT()        
  if (!initialized) {           
    SpiDrv::begin();      
    initialized = true;   
  }                             
  SpiDrv::waitForSlaveReady();  
  SpiDrv::spiSlaveSelect();
static bool initialized = false;
void SpiDrv::begin()
{
    SPI.begin();
    pinMode(SLAVESELECT, OUTPUT);
    pinMode(SLAVEREADY, INPUT);
    pinMode(WIFILED, OUTPUT);
    // digitalWrite(SCK, LOW);
    // digitalWrite(MOSI, LOW);
    digitalWrite(SS, HIGH);
    digitalWrite(SLAVESELECT, HIGH);
    digitalWrite(WIFILED, LOW);
#ifdef _DEBUG_
    INIT_TRIGGER()
#endif
}
*/

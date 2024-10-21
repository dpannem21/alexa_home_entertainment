#include <RCSwitch.h>
#include <ESP8266WiFi.h>
#include <IRremote.h>
#include <fauxmoESP.h>

#define SERIAL_BAUDRATE 9600

#define DEVICE1 "Screen"
#define UP_CODE 13869538
#define DOWN_CODE 13869540
#define DEVICE1_PROTOCOL 1
#define DEVICE1_BIT 24 

RCSwitch rfDebug = RCSwitch();
#define REC_433MHZ_DATA D4

#define REC_38KHZ_DATA D5
IRrecv IR(REC_38KHZ_DATA);

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  
  Serial.println("Enable RF Debugger");
  rfDebug.enableReceive(REC_433MHZ_DATA);

  Serial.println("Enable IR Reciever");
  IR.enableIRIn();

}
void loop() {
  
    if(rfDebug.available()){
      Serial.print("Received ");
      Serial.print( rfDebug.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( rfDebug.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( rfDebug.getReceivedProtocol() );
      delay(1500);
      rfDebug.resetAvailable(); 
    }
    
    
    if(IR.decode()){
      Serial.print("IR Raw Data: ");
      Serial.println(IR.decodedIRData.decodedRawData, HEX);
      Serial.print("IR Protocol: ");
      Serial.println(IR.decodedIRData.protocol);
      delay(1500);
      IR.resume();
    }
}

/*
 * #include <RCSwitch.h>
#include <ESP8266WiFi.h>
#include <IRremote.h>
#include "wifi_cred.h"
#include <fauxmoESP.h>

#define SERIAL_BAUDRATE 9600

#define WIFI_STATUS_LED D7
#define COMMAND_PROCESSING_LED D8

#define DEVICE1 "Screen"
#define UP_CODE 13869538
#define DOWN_CODE 13869540
#define DEVICE1_PROTOCOL 1
#define DEVICE1_BIT 24 

RCSwitch rfControl = RCSwitch();
#define TRA_433MHZ_DATA D2

#define TRA_38KHZ_DATA D6

fauxmoESP fauxmo;
void wifiSetup(){
  WiFi.mode(WIFI_STA); //set WiFi module to STA (Sation) mode 
  Serial.printf("[WiFi] Attempting to connect to %s \n", WIFI_SSID);
  
  // Connect
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Wait till Connected 
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(WIFI_STATUS_LED, HIGH);
    delay(750);
    Serial.print(".");
    digitalWrite(WIFI_STATUS_LED, LOW);
    delay(750);
  }
  Serial.println();
  
  // Connected
  Serial.printf("[WiFi] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  
  pinMode(WIFI_STATUS_LED, OUTPUT);               // Initialize the WiFi Status LED as an output
  digitalWrite(WIFI_STATUS_LED, LOW);
  pinMode(COMMAND_PROCESSING_LED, OUTPUT);        // Initialize the Command Processing Status LED as an output
  digitalWrite(COMMAND_PROCESSING_LED, LOW);
  
  Serial.println("Enable RF Transmitter");
  rfControl.enableTransmit(TRA_433MHZ_DATA);

  Serial.println("Enable IR Transmitter");
  IrSender.begin(TRA_38KHZ_DATA);
  
  wifiSetup();                                   // Initialize the WiFi Connection 
  Serial.println("Setup Alexa Integration");
  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);
  fauxmo.addDevice(DEVICE1);
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
  
  // Callback when a command from Alexa is received. 
  // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
  // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
  // Just remember not to delay too much here, this is a callback, exit as soon as possible.
  // If you have to do something more involved here set a flag and process it in your main loop.
  
  Serial.printf("[Command] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
  
    // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
    // Otherwise comparing the device_name is safer.
    digitalWrite(COMMAND_PROCESSING_LED, HIGH);
    if (strcmp(device_name, DEVICE1)==0 && state) {
      rfControl.send(DOWN_CODE, DEVICE1_BIT);
      IrSender.sendNECRaw(0x6F905583,2);
    } else if (strcmp(device_name, DEVICE1)==0 && !state) {
      rfControl.send(UP_CODE, DEVICE1_BIT);
      IrSender.sendNECRaw(0x6F905583,2);
    }
    digitalWrite(COMMAND_PROCESSING_LED, LOW);
  });}
void loop() {
  if(WiFi.status() == WL_CONNECTED){
    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();
  }
  else{
    wifiSetup(); //attempt to connect to WiFi again 
  }
}
 */

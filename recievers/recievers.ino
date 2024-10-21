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

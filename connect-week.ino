#include "STM32L0.h"
#include "LoRaWAN.h"
#include "LoraMessage.h"

#define SerialDebug true  // set to true to get Serial output for debugging
#define myLed 13  // red led

// Accessible through TTN console where device was created
const char *devEui = ""; 
const char *appEui = "";
const char *appKey = "";

// battery voltage monitor definitions
float VDDA, VBAT, VUSB, Temperature;

void setup(void)  {
  Serial.begin(9600);
  delay(3000);
  Serial.println("SERIAL ENABLED");

  // Configure led
  pinMode(myLed, OUTPUT);

  LoRaWAN.begin(US915);
  LoRaWAN.setADR(false); // Adaptive Data Rating - Defaults to true
  LoRaWAN.setSubBand(2); // Subband will vary based on network provider. TTN uses subband 2

  // Callbacks - See source for all callback options
  LoRaWAN.onJoin(joinCallback);

  // Join and start loop
  Serial.println("JOINING"); 
  LoRaWAN.joinOTAA(appEui, appKey, devEui);
}

void loop(void) {
  if (!LoRaWAN.busy()) {
    if (!LoRaWAN.linkGateways()) {
      LoRaWAN.rejoinOTAA();
    }
  
    if (LoRaWAN.joined()) { 
      Serial.print("TRANSMIT( ");
      Serial.print("TimeOnAir: ");
      Serial.print(LoRaWAN.getTimeOnAir());
      Serial.print(", NextTxTime: ");
      Serial.print(LoRaWAN.getNextTxTime());
      Serial.print(", MaxPayloadSize: ");
      Serial.print(LoRaWAN.getMaxPayloadSize());
      Serial.print(", DR: ");
      Serial.print(LoRaWAN.getDataRate());
      Serial.print(", TxPower: ");
      Serial.print(LoRaWAN.getTxPower(), 1);
      Serial.print("dbm, UpLinkCounter: ");
      Serial.print(LoRaWAN.getUpLinkCounter());
      Serial.print(", DownLinkCounter: ");
      Serial.print(LoRaWAN.getDownLinkCounter());
      Serial.println(" )"); 

      // Read Battery Level and Temperature
      VBAT = STM32L0.getVBAT();
      VDDA = STM32L0.getVDDA();
      VUSB = STM32L0.getVBUS();
      Temperature = STM32L0.getTemperature();

      // Internal STM32L0 functions
      Serial.print("VDDA = "); Serial.println(VDDA, 2); 
      if (VUSB ==  1)  Serial.println("USB Connected!"); 
      Serial.print("STM32L0 MCU Temperature = "); 
      Serial.println(Temperature, 2);
  
      uint16_t voltage = VDDA * 100;

      // Create payload using lora-serialization library
      // https://github.com/thesolarnomad/lora-serialization
      LoraMessage message;
      message
        .addTemperature(Temperature)
        .addUint16(voltage);
      
      bool confirmed = false;
      LoRaWAN.sendPacket(message.getBytes(), message.getLength(), confirmed);

      while (LoRaWAN.busy()) {
        digitalWrite(myLed, HIGH); // turn on led during tx
      }
      
      if (LoRaWAN.confirmed()) {
        Serial.println("ACK");
      } else {
        Serial.println("NACK");
      }

      digitalWrite(myLed, LOW); // turn off led
      STM32L0.stop(10000); // milliseconds
    }
  }
}

void joinCallback(void) {
  if (LoRaWAN.joined()) {
    Serial.println("JOINED");
  } else {
    Serial.println("REJOIN");
    LoRaWAN.rejoinOTAA();
  }
}

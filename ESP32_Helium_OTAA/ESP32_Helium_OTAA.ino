#include "config.h"
#include "Arduino.h"

#define HUMAN_PIN 21
#define CHAINSAW_PIN 13

// Send Data Prototype, convert everything to String
void sendStringData( String myString );

void setup()
{
  if(mcuStarted==0)
  {
    LoRaWAN.displayMcuInit();
  }
  Serial.begin(115200);
  while (!Serial);
  pinMode(HUMAN_PIN,INPUT);
  pinMode(CHAINSAW_PIN,INPUT);
  digitalWrite(HUMAN_PIN, HIGH);
  digitalWrite(CHAINSAW_PIN, HIGH);
  SPI.begin(SCK,MISO,MOSI,SS);
  Mcu.init(SS,RST_LoRa,DIO0,DIO1,license);
  deviceState = DEVICE_STATE_INIT;
}

// The loop function is called in an endless loop
void loop()
{
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
      LoRaWAN.init(loraWanClass,loraWanRegion);
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.displayJoining();
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      LoRaWAN.displaySending();
      String test;
      bool human = digitalRead(HUMAN_PIN);
      bool saw = digitalRead(CHAINSAW_PIN);
      if(saw && human){
        test = "3";
      }
      else if(human){
        test = "1";
      }
      else if(saw){
        test = "2";
      }
      else{
        test = "0";
      }
      Serial.print("Human: ");
      Serial.println(human);
      Serial.print("Saw: ");
      Serial.println(saw);
      sendStringData(test);
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      // Schedule next packet transmission
      txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.displayAck();
      LoRaWAN.sleep(loraWanClass,debugLevel);
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}

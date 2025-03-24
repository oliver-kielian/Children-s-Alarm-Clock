#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>


// Lets define our pins and number of led's for our light matrix
#define LED_PIN 13
#define NUM_LEDS 64


// This is our states for our arduino look at state diagram for more clarification
// TODO: FIX STATES NEEDS MORE CLARIFICATION


enum myState {
  // We are not connected to a central
  // our base 
  // we have our times for the morning light/ night light/ wind down light
  // if time == 7:00 am
  // go to morning light state
  // if time == 7:00 pm 
  // go to wind down light
  // if time == 7:30 pm
  // night light state
  NOTCONNECTED,
  // We are connected to the central
  // if msg == B 
  // currentState == CONNECTED
  CONNECTED,
  // Where should we be if we arent trying to connect but at any moment we want to connnect
  // gather information
  // Depending on what message is sent change parameters for alarms and lights and such
  // if we get dissconnect msg 
  // currentState == NOTCONNECTED
  NIGHTLIGHT,
  MORNINGLIGHT,
  WINDDOWNLIGHT,
  // NIGHT ALARM IS FOR THE WIND DOWN LIGHT
  NIGHTALARM,
  MORNINGALARM
};

enum myState currentState = NOTCONNECTED;

// **** This is beginning bluetooth stuff *****
// set the size of the incoming and outgoing strings. Max 512:
const int characteristicSize = 128;
// create service and characteristics:
// This is our unique services and characteristic
BLEService communicationService("5f7325da-eca4-4d7d-ae15-7bd09b3d24f1");
BLEByteCharacteristic readCharacteristic("034da838-0810-44cb-ad23-8caa8d5ce1fe",
    BLEWrite | BLERead | BLENotify | characteristicSize);

// ***** End of the bluetooth stuff ******

// This is for our light matrix 
Adafruit_NeoPixel matrix(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

BLEDevice central;


void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // put your setup code here, to run once:
  //https://projecthub.arduino.cc/ansh2919/serial-communication-between-python-and-arduino-663756
  matrix.begin();
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("Arduino");
  BLE.setAdvertisedService(communicationService);
  communicationService.addCharacteristic(readCharacteristic);
  BLE.addService(communicationService);
  Serial.println("Bluetooth® device active, waiting for connections...");
  BLE.advertise();
}

void loop() {
  switch (currentState){
    case NOTCONNECTED:

      // wait for a Bluetooth® Low Energy central
      // If the user is connected to the peripheral:
      if (central) {
        Serial.println("Connected to central: ");
        Serial.println(central.address());
        digitalWrite(LED_BUILTIN, HIGH);
        currentState = CONNECTED;
      }
      break;
    case CONNECTED:
      Serial.print("IN CONNECTED STATE ");
      while(central.connected()){
        if (readCharacteristic.written()) {
          String incomingMsg = String(readCharacteristic.value());
          Serial.println(incomingMsg);
          if (incomingMsg == "N") {
            Serial.println("We Have an incomming msg: " + incomingMsg);
          }
          if (incomingMsg == "77") {
            Serial.println("We Have an incomming msg: " + incomingMsg);
          }
          if (incomingMsg == "PM") {
            Serial.println("We Have an incomming msg: " + incomingMsg);
          }
          if (incomingMsg == "AM") {
            Serial.println("We Have an incomming msg: " + incomingMsg);
          }
        }
      }
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Disconnected from central: ");
      Serial.println(central.address());
      currentState = NOTCONNECTED;
      break;
    case NIGHTLIGHT:
      Serial.print("We Are in the NightLigth State");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case MORNINGLIGHT:
      Serial.print("We Are in the MorningLight State");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case NIGHTALARM:
      Serial.print("We Are in the NightAlarm State");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case MORNINGALARM:
      Serial.print("We Are in the MorningAlarm State");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      break;
    default:
      break;
  }

}

// ******* TODO: FIX THIS FUNCTION SO WE CAN USE LATER IN OUR STATES *******
// This is a function we will use later to allow for us to read in the serial
// then make changes to our lights based on what the serial tells us

void updateNightLightsColor(){
  #include <Adafruit_NeoPixel.h>

  #define LED_PIN 6
  #define NUM_PINS 64

  Adafruit_NeoPixel matrix(NUM_PINS, LED_PIN, NEO_GRB + NEO_KHZ800);
    // put your setup code here, to run once:
    //https://projecthub.arduino.cc/ansh2919/serial-communication-between-python-and-arduino-663756
  pinMode(LED_BUILTIN, OUTPUT);
  matrix.begin();

  //https://forum.arduino.cc/t/code-optimising-ws2812-led-matrix-arduino/1068713

  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    int r, g, b;
    String data = Serial.readStringUntil('\n');

    if(sscanf(data.c_str(), "%d,%d,%d", &r, &g, &b) == 3){
      digitalWrite(LED_BUILTIN, HIGH);
      for (int i = 0; i < NUM_PINS; i++) {
        matrix.setPixelColor(i, matrix.Color(r, g, b));
      }
      matrix.show();
    }
  }
}



   //https://forum.arduino.cc/t/code-optimising-ws2812-led-matrix-arduino/1068713

    // put your main code here, to run repeatedly:
    // if(Serial.available() > 0){
    //   int r, g, b;
    //   String data = Serial.readStringUntil('\n');

    //   if(sscanf(data.c_str(), "%d,%d,%d", &r, &g, &b) == 3){
    //     Serial.println("We did get here from Website!!!");
    //     digitalWrite(LED_BUILTIN, HIGH);
    //     for (int i = 0; i < NUM_PINS; i++) {
    //       matrix.setPixelColor(i, matrix.Color(r, g, b));
    //     }
    //     matrix.show();
    //   }
    // }

// ******* TODO: AT LEAST START THESE *******
void updateDayTimeLightsColor(){}

void updateTimerForNightLights(){ }

void updateTimerForDayTimeLights(){ }

void keepTime(){ }


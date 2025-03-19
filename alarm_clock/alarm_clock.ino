#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>


// Lets define our pins and number of led's for our light matrix
#define LED_PIN 13
#define NUM_LEDS 64


// This is our states for our arduino look at state diagram for more clarification
// TODO: FIX STATES NEEDS MORE CLARIFICATION
enum myState {
  NOTCONNECTED,
  CONNECTED,
  NIGHTLIGHT,
  MORNINGLIGHT,
  NIGHTALARM,
  MORNINGALARM
};

enum myState currentState = NOTCONNECTED;

// **** This is beginning bluetooth stuff *****
// set the size of the incoming and outgoing strings. Max 512:
const int characteristicSize = 128;

// create service and characteristics:
// This is our unique services and characteristic
BLEService stringService("c4d6aa1d-e90b-42fa-977e-039e7b401994");
BLEStringCharacteristic txCharacteristic("c4d6aa1d-e90b-42fa-977e-039e7b401994",
    BLEWrite | BLERead | BLENotify, characteristicSize);

// ***** End of the bluetooth stuff ******

// This is for our light matrix 
Adafruit_NeoPixel matrix(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


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
  BLE.setAdvertisedService(stringService);
  BLE.addCharacteristic(txCharacteristic);
  BLE.addService(stringService);
  Serial.println("Bluetooth® device active, waiting for connections...");
  BLE.advertise();

}

void loop() {
  // wait for a Bluetooth® Low Energy central
  BLEDevice central = BLE.central();

  // If the user is connected to the peripheral:
  if (central) {

    Serial.print("Connected to central: ");
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);

    while (central.connected()) {
      if (txCharacteristic.written()) {
        String incomingMsg = txCharacteristic.value();
        if (incomingMsg == "B") {
          Serial.println("Incomming message " + incomingMsg);
        }
        if (incomingMsg == "M") {
          Serial.println("Incomming message Morning Light" + incomingMsg);
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
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());

  }
  switch (currentState){
    case NOTCONNECTED:
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print("Disconnected/Not Conected from central: ");
      Serial.println(central.address());
      break;
    
    case CONNECTED:
      while(central.connected()){
        if (txCharacteristic.written()) {
          String incomingMsg = txCharacteristic.value();
          if (incomingMsg == "N") {
            currentState = NIGHTLIGHT;
          }
          if (incomingMsg == "M") {
            currentState = MORNINGLIGHT;
          }
          if (incomingMsg == "PM") {
            currentState = NIGHTALARM;
          }
          if (incomingMsg == "AM") {
          currentState = MORNINGALARM;
          }
        }
      }
      currentState = NOTCONNECTED;
      break;
    case NIGHTLIGHT:
      Serial.print("We Are in the NightLigth State");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      currentState = CONNECTED;
      break;
    case MORNINGLIGHT:
      Serial.print("We Are in the MorningLight State");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      currentState = CONNECTED;
      break;
    case NIGHTALARM:
      Serial.print("We Are in the NightAlarm State");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      currentState = CONNECTED;
      break;
    case MORNINGALARM:
      Serial.print("We Are in the MorningAlarm State");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(LED_BUILTIN, LOW);
      currentState = CONNECTED;
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


// ******* TODO: AT LEAST START THESE *******
void updateDayTimeLightsColor(){}

void updateTimerForNightLights(){ }

void updateTimerForDayTimeLights(){ }

void keepTime(){ }


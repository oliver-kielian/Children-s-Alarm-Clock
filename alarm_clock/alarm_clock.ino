#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define NUM_PINS 64

 // Bluetooth® Low Energy Battery Service
// BLEService batteryService("180F");

// // Bluetooth® Low Energy Battery Level Characteristic
// BLEUnsignedCharCharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
//     BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes


// set the size of the incoming and outgoing strings. Max 512:
const int characteristicSize = 128;

// create service and characteristics:
// This is our unique services and characteristic
BLEService stringService("c4d6aa1d-e90b-42fa-977e-039e7b401994");
BLEStringCharacteristic txCharacteristic("c4d6aa1d-e90b-42fa-977e-039e7b401994",
    BLEWrite | BLERead | BLENotify, characteristicSize);


Adafruit_NeoPixel matrix(NUM_PINS, LED_PIN, NEO_GRB + NEO_KHZ800);

int oldBatteryLevel = 0;  // last battery level reading from analog input
long previousMillis = 0;  // last time the battery level was checked, in ms

void setup() {
  Serial.begin(115200);    // initialize serial communication
  unsigned long start = millis();


  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  // put your setup code here, to run once:
  //https://projecthub.arduino.cc/ansh2919/serial-communication-between-python-and-arduino-663756
  matrix.begin();
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  /* Set a local name for the Bluetooth® Low Energy device
     This name will appear in advertising packets
     and can be used by remote devices to identify this Bluetooth® Low Energy device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("Arduino");
  // // UUID: Universally Unique Identifier
  // BLE.setAdvertisedService(stringService); // This is the service we are trying to provide so we have no conflicts with existing services
  // BLE.addCharacteristic(txCharacteristic); // for example what we want to read/write about like writing "B" to serial
  // BLE.addService(stringService); // Here we are just adding our service to we are able to use it
  BLE.setAdvertisedService(stringService);
  stringService.addCharacteristic(txCharacteristic);
  BLE.addService(stringService);

  /* Start advertising Bluetooth® Low Energy.  It will start continuously transmitting Bluetooth® Low Energy
     advertising packets and will be visible to remote Bluetooth® Low Energy central devices
     until it receives a new connection */
  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  // wait for a Bluetooth® Low Energy central
  BLEDevice central = BLE.central();

  // Later I assume that this should be a "state" where we are going into this state when we are being connected to 
  // Then if we are not being connnected to that we are still keeping on track with our timers and our lights going off appropriatly at those times
  // For now I think this is good for sprint one and later we can add various states

  // If the user is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    // digitalWrite(LED_BUILTIN, HIGH);

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

    // while the central is connected:
    // Instead of checking for battery level we want to see if we are getting any messages from the website
    // To then later grab that information of the colors to then change but 
    // for now this is a good way to check battery for just a general as well as for later if we decide to go wireless
    // So while we are connected
    while (central.connected()) {
      if (txCharacteristic.written()) {
        String incomingMsg = txCharacteristic.value();
        if (incomingMsg == "B") {
          Serial.println("Received The Letter: " + incomingMsg + " from server");
          digitalWrite(LED_BUILTIN, HIGH);
        }
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());

    // while(central.connect()){
    // ** This Should Be For Both Day And Night Lights **
    // If we read somthing from the serial like color hue -> Red to Orange
    // Make a Change in the matrix colors

    // If We reason somthing from the serial like change Date to -> 8:00 - 9:00
    // Make a change in the timer for when the colors should activate
    // }
    // digitalWrite(LED_BUILTIN, LOW);
    // Serial.print("Disconnected from central: ");
    // Serial.println(central.address());
  }
}

void updateBatteryLevel() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a battery.
  */
  int battery = analogRead(A0);
  int batteryLevel = map(battery, 0, 1023, 0, 100);

  if (batteryLevel != oldBatteryLevel) {      // if the battery level has changed
    Serial.print("Battery Level % is now: "); // print it
    Serial.println(batteryLevel);
    // batteryLevelChar.writeValue(batteryLevel);  // and update the battery level characteristic
    oldBatteryLevel = batteryLevel;           // save the level for next comparison
  }
}

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


void updateDayTimeLightsColor(){

}
// Update the timers from what the website is giving us
void updateTimerForNightLights(){ }

void updateTimerForDayTimeLights(){ }

void keepTime(){ }


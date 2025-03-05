#include <ArduinoBLE.h>

 // Bluetooth® Low Energy Battery Service
BLEService batteryService("180F");

// Bluetooth® Low Energy Battery Level Characteristic
BLEUnsignedCharCharacteristic batteryLevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

int oldBatteryLevel = 0;  // last battery level reading from analog input
long previousMillis = 0;  // last time the battery level was checked, in ms

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

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
  BLE.setLocalName("MyArduino");
  BLE.setAdvertisedService(batteryService); // add the service UUID
  // UUID: Universally Unique Identifier
  batteryService.addCharacteristic(batteryLevelChar); // add the battery level characteristic
  BLE.addService(batteryService); // Add the battery service
  batteryLevelChar.writeValue(oldBatteryLevel); // set initial value for this characteristic

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

  // Later I assume that this should be a state where we are going into this state when we are being connected to 
  // Then if we are not being connnected to that we are still keeping on track with our timers and our lights going off appropriatly at those times
  // For now I think this is good for sprint one and later we can add various states

  // If the user is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // check the battery level every 200ms
    // while the central is connected:
    // Instead of checking for battery level we want to see if we are getting any messages from the website
    // To then later grab that information of the colors to then change but 
    // for now this is a good way to check battery for just a general as well as for later if we decide to go wireless
    while (central.connected()) {
      long currentMillis = millis();
      // if 200ms have passed, check the battery level:
      if (currentMillis - previousMillis >= 200 && (Serial.available() > 0)) {
        char incommingMsg = Serial.read();
        if(incommingMsg == 'H'){
          previousMillis = currentMillis;
          updateBatteryLevel();
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

    // If We reasom somthing from the serial like change Date to -> 8:00 - 9:00
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
    batteryLevelChar.writeValue(batteryLevel);  // and update the battery level characteristic
    oldBatteryLevel = batteryLevel;           // save the level for next comparison
  }
}

// This is a function we will use later to allow for us to read in the serial
// then make changes to our lights based on what the serial tells us
void updateNightLightsColor(){

}

void updateDayTimeLightsColor(){

}

// Update the timers from what the website is giving us
void updateTimerForNightLights(){

}

void updateTimerForDayTimeLights(){

}

void keepTime(){

}


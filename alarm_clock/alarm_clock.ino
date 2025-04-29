#include <Adafruit_NeoPixel.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <RTClib.h>
#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>


// ILI9341 pins
#define LCD_CS   10
#define LCD_RST  9
#define LCD_DC   8

// NeoPixel matrix
#define LED_PIN 6
#define NUM_LEDS 64
uint8_t ledBrightness = 255; // LED matrix brightness 0-255
int morningAlarmTime = 0;
int nightAlarmTime = 0;
int windDownAlarmTime = 0;
char charbufAM[7];
char charbufPM[7];

long int rbg_NightTime;
long int rbg_MorningTime;

DateTime now;
String AM_Time_Hour;
String AM_Time_Minute;
String PM_Time_Hour;
String PM_Time_Minute;
String AM_Color;
String PM_Color;


// LCD screen declaration
Adafruit_ILI9341 LCD = Adafruit_ILI9341(LCD_CS, LCD_DC, LCD_RST);
// RTC module declaration
RTC_DS3231 rtc;
// Light matrix declaratoin
Adafruit_NeoPixel matrix(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// **** This is beginning bluetooth stuff *****
const int characteristicSize = 128;
// create service and characteristics:
// This is our unique services and characteristic
BLEService communicationService("5f7325da-eca4-4d7d-ae15-7bd09b3d24f1");
BLECharacteristic readCharacteristic("034da838-0810-44cb-ad23-8caa8d5ce1fe",
    BLEWrite | BLERead, characteristicSize);

BLEDevice central;
// ***** End of the bluetooth stuff ******


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
};
enum myState currentState = NOTCONNECTED;

//timing stuff
unsigned long previousMillis = 0;
unsigned long nightStart = 0;
unsigned long morningStart = 0;
bool nightLightDone = false;
bool morningLightDone = false;
const long interval = 1000;




void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize LCD
  LCD.begin();
  LCD.setRotation(1);
  LCD.setTextColor(ILI9341_BLACK);
  snorLaxFace();
  matrix.begin();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Can't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set to compile time
  }

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
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    displayTime();
  }
  central = BLE.central();

  switch (currentState){
    case NOTCONNECTED:
      // Serial.println("Not connected");
      // wait for a Bluetooth® Low Energy central
      // If the user is connected to the peripheral:
      if (central) {
        Serial.println("Connected to central: ");
        Serial.println(central.address());
        digitalWrite(LED_BUILTIN, HIGH);
        currentState = CONNECTED;
      }
      if(AM_Time_Hour == String(now.hour()) && AM_Time_Minute == String(now.minute())){
        currentState = MORNINGLIGHT;
      }
      if(PM_Time_Hour == String(now.hour()) && PM_Time_Minute == String(now.minute())){
        Serial.println("Connected to NIGHTLIGHT: ");
        currentState = NIGHTLIGHT;
      }
      break;
    case CONNECTED:
      Serial.println("IN CONNECTED STATE ");
      while(central.connected()){
        if (readCharacteristic.written()) {
          // "https://stackoverflow.com/questions/73695079/noise-in-arduino-ble-data-reading"
          int length = readCharacteristic.valueLength();
          byte value[length + 1];
          readCharacteristic.readValue(value, length);
          value[length] = '\0';
          // Serial.print("Characteristic event, written: ");
          // Serial.println((char *) value);
          if (String((char) value[0]) == "N") {
            Serial.println("We Got A Message Of N Now Set Variables");
            PM_Color = String((char*) value).substring(2);
            Serial.println(PM_Color);
          }
          if (String((char) value[0]) == "M") {
            Serial.println("We Got A Message Of M Now Set Variables");
            AM_Color = String((char*) value).substring(2);
            // Serial.println(AM_Color);
          }
          if (String((char) value[0]) == "P" && String((char) value[1]) == "M") {
            Serial.println("We Got A Message Of PM Now Set Variables");
            PM_Time_Hour = String((char*) value).substring(2,4);
            PM_Time_Minute = String((char*) value).substring(4);
            Serial.println(PM_Time_Hour);
            Serial.println(PM_Time_Minute);
          }
          if (String((char) value[0]) == "A" && String((char) value[1]) == "M") {
            Serial.println("We Got A Message Of AM Now Set Variables");
            AM_Time_Hour = String((char*) value).substring(2,4);
            AM_Time_Minute = String((char*) value).substring(4);
            // Serial.println(AM_Time_Hour);
            // Serial.println(AM_Time_Minute);
          }

        }
      }
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Disconnected from central: ");
      Serial.println(central.address());
      currentState = NOTCONNECTED;
      break;
    case NIGHTLIGHT:{
        if (nightStart == 0) {
          nightStart = millis();
          nightLightDone = false;
          if(ledBrightness == NULL){
            ledBrightness = 255;
          }
          Serial.println(PM_Color);
          PM_Color.toCharArray(charbufPM,7);
          rbg_NightTime = strtoul(charbufPM,0,16);
          // Serial.print("We Are in the NightLigth State");
          byte r =(byte)(rbg_NightTime>>16);
          byte g =(byte)(rbg_NightTime>>8);
          byte b =(byte)(rbg_NightTime);

          Serial.println(PM_Color);
          Serial.println(r);
          Serial.println(g);
          Serial.println(b);
          for (int i = 0; i < NUM_LEDS; i++) {
            matrix.setBrightness(ledBrightness/4);
            matrix.setPixelColor(i, matrix.Color(r, g, b));
          }
          matrix.show();
          Serial.println("Here 1");
          delay(500);
          for (int i = 0; i < NUM_LEDS; i++) {
            matrix.setBrightness(ledBrightness/2);
            matrix.setPixelColor(i, matrix.Color(r, g, b));
          }
          matrix.show();
          Serial.println("Here 2");
          delay(500);
          for (int i = 0; i < NUM_LEDS; i++) {
            matrix.setBrightness(ledBrightness);
            matrix.setPixelColor(i, matrix.Color(r, g, b));
          }
          Serial.println("Here Last");
          matrix.show();
        }
        if(!nightLightDone && millis() - nightStart >= 300000UL){
          nightLightDone = true;
          matrix.setBrightness(ledBrightness / 2);
          matrix.show();
        }
      
        if(AM_Time_Hour == String(now.hour()) && AM_Time_Minute == String(now.minute())){
          currentState = MORNINGLIGHT;
          nightStart = 0;
        }   
      }
      break;
    case MORNINGLIGHT:{
      if (morningStart == 0) {
          morningStart = millis();
          morningLightDone = false;
          if(ledBrightness == NULL){
            ledBrightness = 255;
          }
          AM_Color.toCharArray(charbufAM,7);
          rbg_MorningTime = strtoul(charbufAM,0,16);
          // Serial.print("We Are in the NightLigth State");
          byte r =(byte)(rbg_MorningTime>>16);
          byte g =(byte)(rbg_MorningTime>>8);
          byte b =(byte)(rbg_MorningTime);

          for (int i = 0; i < NUM_LEDS; i++) {
            matrix.setBrightness(ledBrightness);
            matrix.setPixelColor(i, matrix.Color(r, g, b));
          }
          matrix.show();
        }
        if(!morningLightDone && millis() - morningStart >= 300000UL){
          morningLightDone = true;
          currentState = NOTCONNECTED; 
        }
      }
      break;
    default:
      break;
  }
}

void displayTime() {
  now = rtc.now();
  uint16_t CREAM = LCD.color565(255,238,185);
  LCD.setCursor(60, 200);
  LCD.setTextSize(4);
  LCD.setTextColor(ILI9341_BLACK, CREAM);

  if (now.hour() < 10) LCD.print("0");
  LCD.print(now.hour()); LCD.print(":");
  if (now.minute() < 10) LCD.print("0");
  LCD.print(now.minute()); LCD.print(":");
  if (now.second() < 10) LCD.print("0");
  LCD.print(now.second());
}

// ******* TODO: FIX THIS FUNCTION SO WE CAN USE LATER IN OUR STATES *******
// This is a function we will use later to allow for us to read in the serial
// then make changes to our lights based on what the serial tells us
// void updateNightLightsColor(){
//   //https://forum.arduino.cc/t/code-optimising-ws2812-led-matrix-arduino/1068713
//   // put your main code here, to run repeatedly:
//   if(Serial.available() > 0)
//   {
//     int r, g, b;
//     String data = Serial.readStringUntil('\n');

//     if(sscanf(data.c_str(), "%d,%d,%d", &r, &g, &b) == 3){
//       digitalWrite(LED_BUILTIN, HIGH);
//       for (int i = 0; i < NUM_LEDS; i++) {
//         matrix.setPixelColor(i, matrix.Color(r, g, b));
//       }
//       matrix.show();
//     }
//   }
// }


void snorLaxFace(){
  uint16_t BG = LCD.color565( 30, 92, 96 );
  uint16_t CREAM = LCD.color565(255,238,185);
  uint16_t OUTLINE = ILI9341_BLACK; 
  LCD.fillScreen(BG);

  const int cx = 160;
  const int cy = 120;
  const int rectW = 300;
  const int rectH = 200;

  int x0 = cx - rectW / 2;
  int y0 = cy - rectH / 2;

  
  LCD.fillRect(x0, y0, rectW, rectH, CREAM);
  LCD.drawRect(x0, y0, rectW, rectH, OUTLINE);

  const int notchHalf = 55;
  const int notchDepth = 50;

  int notchLeftX = cx - notchHalf;
  int notchRightX = cx + notchHalf;
  int notchBaseY = y0;
  int notchApexX = cx;
  int notchApexY = y0 + notchDepth;

  LCD.fillTriangle(notchLeftX,  notchBaseY,
                  notchRightX, notchBaseY,
                  notchApexX,  notchApexY,
                  BG);

  LCD.drawTriangle(notchLeftX,  notchBaseY,
                  notchRightX, notchBaseY,
                  notchApexX,  notchApexY,
                  BG);
  
  LCD.fillRect(30, 80, 80, 25, OUTLINE);
  LCD.fillRect(190, 80, 80, 25, OUTLINE);
  const int mouthW = 150;
  const int mouthH = 25;
  int x10 = 160 - mouthW / 2;
  int y10 = 120 - mouthH / 2;
  LCD.fillRect(x10, y10 + 45, mouthW, mouthH, OUTLINE);
}

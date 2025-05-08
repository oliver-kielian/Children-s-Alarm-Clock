#include <Adafruit_NeoPixel.h>

#include <Wire.h>
#include <SPI.h>
#include <Arduino_GFX_Library.h>
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
uint8_t ledBrightness = 0; // LED matrix brightness 0-255
int morningAlarmTime = 0;
int nightAlarmTime = 0;
int windDownAlarmTime = 0;
char charbufAM[7];
char charbufPM[7];
char charbufMAN[7];
bool manualLight;
bool manualMode = true;

long int rbg_NightTime;
long int rbg_MorningTime;

DateTime now;
String AM_Time_Hour;
String AM_Time_Minute;
String PM_Time_Hour;
String PM_Time_Minute;
String AM_Color;
String PM_Color;
String MANUAL_Color;


// LCD screen declaration
Arduino_HWSPI bus(LCD_DC, LCD_CS);
Arduino_ILI9341 LCD(&bus, LCD_RST);
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


// This is our states for our arduino
enum myState {
  NOTCONNECTED,
  CONNECTED,
  NIGHTLIGHT,
  MORNINGLIGHT,
  MANUALMODE,
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
  LCD.setTextColor(BLACK);
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
  BLE.poll();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    displayTime();
  }
  central = BLE.central();

  if (central && central.connected() && currentState != CONNECTED) {
    Serial.print("Connected: "); 
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);
    currentState = CONNECTED;
  }

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
      if(manualLight == true){
        currentState = MANUALMODE; 
      }
      break;
    case CONNECTED:
      if(central.connected()){
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
            // Serial.println(PM_Color);
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
            // Serial.println(PM_Time_Hour);
            // Serial.println(PM_Time_Minute);
          }
          if (String((char) value[0]) == "A" && String((char) value[1]) == "M") {
            Serial.println("We Got A Message Of AM Now Set Variables");
            AM_Time_Hour = String((char*) value).substring(2,4);
            AM_Time_Minute = String((char*) value).substring(4);
            // Serial.println(AM_Time_Hour);
            // Serial.println(AM_Time_Minute);
          }
          if(String((char) value[0]) == "L" && String((char) value[1]) == "O"){
            MANUAL_Color = String((char*) value).substring(3);
            Serial.println(MANUAL_Color);
            manualLight = true;
          }
          if(String((char) value[0]) == "L" && String((char) value[1]) == "F"){
            manualLight = false;
          }
          if (String((char) value[0]) == "B") {
            Serial.println("We Got A Message Of B Now Set Variables");
            if(String((char) value[2]) == NULL){
              ledBrightness = String((char*) value).substring(1).toInt();
              Serial.println("TOP");
              Serial.println(ledBrightness);
            }else{
              ledBrightness = String((char*) value).substring(1).toInt();
              Serial.println("BOTTOM");
              Serial.println(ledBrightness);
            }
          }
        }
      }else{
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("Disconnected from central: ");
        Serial.println(central.address());
        BLE.advertise();
        central = BLEDevice();
        currentState = NOTCONNECTED;
      }
      break;
    case NIGHTLIGHT:{
        if (nightStart == 0) {
          nightStart = millis();
          nightLightDone = false;
          if(ledBrightness == 0){
            ledBrightness = 60;
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
          // delay(500);
          for (int i = 0; i < NUM_LEDS; i++) {
            matrix.setBrightness(ledBrightness/2);
            matrix.setPixelColor(i, matrix.Color(r, g, b));
          }
          matrix.show();
          Serial.println("Here 2");
          // delay(500);
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
        if(manualLight == true){
          currentState = MANUALMODE;
        }  
      }
      break;
    case MORNINGLIGHT:{
      if (morningStart == 0) {
          morningStart = millis();
          morningLightDone = false;
          if(ledBrightness == 0){
            ledBrightness = 60;
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
        if(manualLight == true){
          currentState = MANUALMODE;
        }
      }
      break;
    case MANUALMODE:{
        if(ledBrightness == 0){
          ledBrightness = 60;
        }
        if(manualMode){
          manualMode = false;
          MANUAL_Color.toCharArray(charbufMAN,7);
          rbg_MorningTime = strtoul(charbufMAN,0,16);
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
  
        if(manualLight == false){
          currentState = NOTCONNECTED;
          manualMode = true;
          matrix.clear();
          matrix.show();
        }
      } 
      break;
    default:
      break;
  }
}

void displayTime() {
  now = rtc.now();
  LCD.setCursor(80, 200);
  LCD.setTextSize(4);
  LCD.setTextColor(BLACK, WHITE);

  if (now.hour() < 10) LCD.print("0");
  LCD.print(now.hour()); LCD.print(":");
  if (now.minute() < 10) LCD.print("0");
  LCD.print(now.minute()); LCD.print(":");
  if (now.second() < 10) LCD.print("0");
  LCD.print(now.second());
}

void snorLaxFace(){
  LCD.fillScreen(WHITE);
  LCD.fillArc(85, 100, 60, 40, 210, 330, BLACK);
  LCD.fillArc(235, 100, 60, 40, 210, 330, BLACK);

  LCD.drawTriangle(140, 130, 110, 150, 130, 170, BLACK);

  LCD.drawTriangle(195, 130, 205, 170, 225, 150, BLACK);

  LCD.fillArc(170, 100, 80, 60, 30, 150, BLACK);
}



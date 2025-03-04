#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define NUM_PINS 64

Adafruit_NeoPixel matrix(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //https://projecthub.arduino.cc/ansh2919/serial-communication-between-python-and-arduino-663756
  matrix.begin();

  //https://forum.arduino.cc/t/code-optimising-ws2812-led-matrix-arduino/1068713
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    int r, g, b;
    String data = Serial.readStringUntil("\n");

    if(sscanf(data.c_str(), "%d,%d,%d", &r, &g, &b) == 3){
      for (int i = 0; i < NUM_LEDS; i++) {
        matrix.setPixelColor(i, matrix.Color(r, g, b));
        Serial.println("martix %d on", i);
      }
      matrix.show();
    }
  }
}

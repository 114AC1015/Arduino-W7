#include <Arduino.h>

const byte buttonPin = 3;
const byte redPin = 10;
const byte bluePin = 11;

int happiness = 10;
int lastScore = 10;
float displayed = 10;
unsigned long lastPress = 0;
unsigned long changedAt = 0;
unsigned long lastFrame = 0;
bool rawButton = HIGH;
bool stableButton = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  rawButton = stableButton = digitalRead(buttonPin);
  lastPress = changedAt = lastFrame = millis();
  Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();
  bool reading = digitalRead(buttonPin);

  if (reading != rawButton) {
    rawButton = reading;
    changedAt = now;
  }
  if (now - changedAt >= 35 && reading != stableButton) {
    stableButton = reading;
    if (stableButton == LOW) {
      happiness = min(20, happiness + 1);
      lastScore = happiness;
      lastPress = now;
      Serial.println(happiness);
    }
  }

  unsigned long idle = now - lastPress;
  if (idle >= 5000) {
    unsigned long lost = 1 + (idle - 5000) / 1000;
    int newScore = lost >= (unsigned long)lastScore ? 0 : lastScore - (int)lost;
    if (newScore != happiness) {
      happiness = newScore;
      Serial.println(happiness);
    }
  }

  float step = 8.0f * (now - lastFrame) / 1000.0f;
  lastFrame = now;
  if (displayed < happiness) displayed = min((float)happiness, displayed + step);
  else displayed = max((float)happiness, displayed - step);

  byte red = (byte)(255.0f * (1.0f - displayed / 20.0f));
  byte blue = (byte)(255.0f * displayed / 20.0f);
  analogWrite(redPin, 255 - red);
  analogWrite(bluePin, 255 - blue);
}

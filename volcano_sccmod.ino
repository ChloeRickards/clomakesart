#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// DFPlayer pins
static const uint8_t PIN_MP3_TX = 2;  // Connects to module's RX
static const uint8_t PIN_MP3_RX = 3;  // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;

// Button and control pins
const int buttonPin = 4;   // Physical button input
const int busyPin = 5;     // DFPlayer BUSY pin (LOW while playing)
const int wledPin = 6;     // Output to GLEDOPTO IO13

bool isPlaying = false;

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(busyPin, INPUT);
  pinMode(wledPin, OUTPUT);
  digitalWrite(wledPin, HIGH); // idle high, not triggering GLEDOPTO

  if (player.begin(softwareSerial)) {
    Serial.println("DFPlayer OK");
    player.volume(30);
  } else {
    Serial.println("DFPlayer failed!");
  }
}

void loop() {
  // If playing, check if audio has finished
  if (isPlaying && digitalRead(busyPin) == HIGH) {
    isPlaying = false;
    Serial.println("Playback finished, ready for next press");
  }

  // Button is LOW when pressed (INPUT_PULLUP)
  if (digitalRead(buttonPin) == LOW && !isPlaying) {
    isPlaying = true;
    Serial.println("Button pressed, starting sequence");

    // Trigger audio
    player.playFolder(1, 5);

    // Trigger WLED — pull LOW briefly to simulate button press
    digitalWrite(wledPin, LOW);
    delay(200);
    digitalWrite(wledPin, HIGH);
  }
}
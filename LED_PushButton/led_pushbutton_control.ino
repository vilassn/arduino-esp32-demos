// This program is to turn on LED on BOOT button press

#include <Preferences.h>

Preferences preferences;

const int buttonPin = 0;    
const int ledPin = 2;      

bool ledState;         
bool buttonState;             
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() { 
  Serial.begin(115200);

  //Create a namespace called "gpio"
  preferences.begin("gpio", false);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // read the last LED state from flash memory
  ledState = preferences.getBool("state", false); 
  buttonState = ledState;
  Serial.printf("LED state before reset: %d \n", ledState);

  // set the LED to the last stored state
  digitalWrite(ledPin, ledState);
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  lastButtonState = reading;
  if (digitalRead(ledPin)!= ledState) {  
    Serial.println("State changed");

    // change the LED state
    digitalWrite(ledPin, ledState);
    
    // save the LED state in flash memory
    preferences.putBool("state", ledState);
    Serial.printf("State saved: %d \n", ledState);
  }
}
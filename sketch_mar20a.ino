#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

// Declare the pins connected to the buzzer, LED, door sensor, and button
int buzzerPin = 12;
int ledPin = 13;
int sensorPin = 14;
int buttonPin = 2;

RTC_DS1307 rtc;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

boolean isOn = false;         // Track the current state of the circuit
boolean buttonPressed = false; // Track the button press state

void setup() {
  // Initialize the input and output pins
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Start serial communication
  Serial.begin(9600);

  // Initialize the I2C bus, RTC module, and OLED display
  Wire.begin();
  rtc.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Read the value from the door sensor
  int sensorValue = digitalRead(sensorPin);

  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // If the button is pressed and the previous state was not pressed
  if (buttonState == LOW && !buttonPressed) {
    // Toggle the circuit state
    isOn = !isOn;
    buttonPressed = true;
  }

  // Reset the button state if the button is released
  if (buttonState == HIGH) {
    buttonPressed = false;
  }

  // Check the circuit state
  if (isOn) {
    // If the door is open
    if (sensorValue == HIGH) {
      // Activate the buzzer and LED, and print a message to the serial monitor
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(ledPin, HIGH);
      Serial.println("Door is open");

      // Wait for 1 second before turning off the buzzer and LED
      delay(100);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(ledPin, LOW);
      DateTime now = rtc.now();
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.println("OPEN");
      display.setTextSize(2);
      display.print(now.day(), DEC);
      display.print('/');
      display.print(now.month(), DEC);
      display.print('/');
      display.print(now.year(), DEC);
      display.setCursor(0, 30);
      display.setTextSize(2);
      display.print(now.hour(), DEC);
      display.print(':');
      display.print(now.minute(), DEC);
      display.display();
      delay(200);
    } else {
      // If the door is closed
      DateTime now = rtc.now();
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.println("CLOSE");
      display.setTextSize(2);
      display.print(now.day(), DEC);
      display.print('/');
      display.print(now.month(), DEC);
      display.print('/');
      display.print(now.year(), DEC);
      display.setCursor(0, 30);
      display.setTextSize(2);
      display.print(now.hour(), DEC);
      display.print(':');
      display.print(now.minute(), DEC);
      display.display();
      delay(200);
    }
  } else {
    // If the circuit is off
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.println("OFF");
    display.display();
    delay(200);
  }
}

/**
 * IR Remote Shutter
 * 
 * author : @dego_96
 * 
 * Target Board     : Arduino Pro Mini
 * Target Processor : Atmega 168P (5V, 16MHz)
 */
#include <IRremote.h>

#define BAUD_RATE (9600)

#define PIN_IR_RCV (9)
#define PIN_LED    (13)
#define PIN_OUT1   (10)
#define PIN_OUT2   (11)
#define PIN_OUT3   (12)

#define ENABLE_TIME   (100)
#define LED_ON_TIME   (300)

IRrecv irrecv(PIN_IR_RCV);
decode_results results;

int mode;
uint32_t startTime;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_OUT1, OUTPUT);
  pinMode(PIN_OUT2, OUTPUT);
  pinMode(PIN_OUT3, OUTPUT);
  Serial.begin(BAUD_RATE);
  irrecv.enableIRIn();

  mode = 0;
}

void loop() {
  if (irrecv.decode(&results)) {
    uint32_t val = results.value;
    //    Serial.println(val, HEX);
    if (val == 0xFFE01F) {  // MODE 1
      Serial.println("mode 1");
      mode = 1;
      startTime = millis();
    } else if (val == 0xFFA857) { // MODE 2
      Serial.println("mode 2");
      mode = 2;
      startTime = millis();
    } else if (val == 0xFF906F) { // MODE 3
      Serial.println("mode 3");
      mode = 3;
      startTime = millis();
    } else if (val == 0xFFFFFFFF) {
      // nothing to do.
      Serial.println(val, HEX);
    } else {
      mode = 0;
      startTime = 0xFFFFFFFF;
    }
    irrecv.resume();
  }

  pinOut();

  if (mode == 1) {
    if (startTime + (LED_ON_TIME * 1) > millis()) {
      digitalWrite(PIN_LED, HIGH);
    } else {
      digitalWrite(PIN_LED, LOW);
    }
  } else if (mode == 2) {
    if (startTime + (LED_ON_TIME * 1) > millis()) {
      digitalWrite(PIN_LED, HIGH);
    } else if (startTime + (LED_ON_TIME * 2) > millis()) {
      digitalWrite(PIN_LED, LOW);
    } else if (startTime + (LED_ON_TIME * 3) > millis()) {
      digitalWrite(PIN_LED, HIGH);
    } else {
      digitalWrite(PIN_LED, LOW);
    }
  } else if (mode == 3) {
    if (startTime + (LED_ON_TIME * 1) > millis()) {
      digitalWrite(PIN_LED, HIGH);
    } else if (startTime + (LED_ON_TIME * 2) > millis()) {
      digitalWrite(PIN_LED, LOW);
    } else if (startTime + (LED_ON_TIME * 3) > millis()) {
      digitalWrite(PIN_LED, HIGH);
    } else if (startTime + (LED_ON_TIME * 4) > millis()) {
      digitalWrite(PIN_LED, LOW);
    } else if (startTime + (LED_ON_TIME * 5) > millis()) {
      digitalWrite(PIN_LED, HIGH);
    } else {
      digitalWrite(PIN_LED, LOW);
    }
  } else {
    digitalWrite(PIN_LED, LOW);
  }
}

void pinOut() {
  if (startTime + ENABLE_TIME > millis()) {
    switch (mode) {
      case 1:
        digitalWrite(PIN_OUT2, LOW);
        digitalWrite(PIN_OUT3, LOW);
        digitalWrite(PIN_OUT1, HIGH);
        break;
      case 2:
        digitalWrite(PIN_OUT1, LOW);
        digitalWrite(PIN_OUT3, LOW);
        digitalWrite(PIN_OUT2, HIGH);
        break;
      case 3:
        digitalWrite(PIN_OUT1, LOW);
        digitalWrite(PIN_OUT2, LOW);
        digitalWrite(PIN_OUT3, HIGH);
        break;
      default:
        digitalWrite(PIN_OUT1, LOW);
        digitalWrite(PIN_OUT2, LOW);
        digitalWrite(PIN_OUT3, LOW);
        break;
    }
  } else {
    digitalWrite(PIN_OUT1, LOW);
    digitalWrite(PIN_OUT2, LOW);
    digitalWrite(PIN_OUT3, LOW);
  }
}

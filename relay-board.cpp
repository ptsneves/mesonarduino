/*
 * Blink
 * Turns on an LED on for one second, then off for one second, repeatedly.
 *
 * This example code is in the public domain.
 */

#include<string.h>
#include<Arduino.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:

const uint8_t camera_pin = 12;
const uint8_t board_pin = 11;

#define CAMERA_ON "CAMERA:ON"
#define CAMERA_OFF "CAMERA:OFF"
#define BOARD_ON "BOARD:ON"
#define BOARD_OFF "BOARD:OFF"
#define MAX_STRLEN 12
#define SEPARATOR '\r'

enum PinLogicType {
  ACTIVE_LOW,
  ACTIVE_HIGH
};

enum PinValueType {
  OFF = 0,
  ON = 1
};

struct PinData {
  const char* name_on;
  const char* name_off;
  uint8_t pin;
  PinLogicType logic;
  PinValueType value;
};

void changePin(PinData& pin, PinValueType state) {
  const char* name = (state == ON ? pin.name_on : pin.name_off);
  uint8_t digital_write_value = 0;

  pin.value = state;
  if (pin.logic == ACTIVE_LOW) {
    digital_write_value = (state == ON ? LOW : HIGH);
  }
  else {
    digital_write_value = (state == ON ? HIGH : LOW);
  }
  digitalWrite(pin.pin, digital_write_value);

  Serial.print("Success: ");
  Serial.println(name);
}

void printHelp(const PinData pin_list[], const size_t pins_count) {
  Serial.println("Welcome. Commands available are:");
  Serial.print("Separator is: ");
  Serial.println(SEPARATOR);
  for (size_t i = 0; i < pins_count; i++) {
    Serial.println(pin_list[i].name_on);
    Serial.println(pin_list[i].name_off);
  }
  Serial.println("Waiting for input");
}

// the setup routine runs once when you press reset:
void setup() {
}

struct MatchBufferResult {
  PinData * pin;
  PinValueType value;
};

MatchBufferResult matchBufferToPin(PinData pin_list[], const size_t pins_count, const char* buffer ) {
  MatchBufferResult res = {NULL, OFF};
  for (size_t i = 0; i < pins_count; i++) {
    if (!strcmp(buffer, pin_list[i].name_on)) {
      res.pin = &pin_list[i];
      res.value = ON;
    }
    else if (!strcmp(buffer, pin_list[i].name_off)) {
      res.pin = &pin_list[i];
      res.value = OFF;
    }
  }
  return res;
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.begin(57600);
  PinData pins[] = {
    { CAMERA_ON, CAMERA_OFF, camera_pin, ACTIVE_LOW, OFF },
    { BOARD_ON, BOARD_OFF, board_pin, ACTIVE_LOW, OFF }
  };
  const size_t pins_count = sizeof(pins) / sizeof(pins[0]);
  // initialize the digital pin as an output.
  for (size_t i = 0; i < pins_count; i++) {
    pinMode(pins[i].pin, OUTPUT);
    changePin(pins[i], pins[i].value);
  }
  printHelp(pins, pins_count);
  char buffer[MAX_STRLEN];
  uint8_t index = 0;
  while(1) {
    while (Serial.available()) {
      char c = Serial.read();
      Serial.print(c);
      if (index == MAX_STRLEN-1) {
        index = 0;
        Serial.println("No option that long start typing again");
        break;
      }
      else if (c != SEPARATOR) {
        buffer[index % MAX_STRLEN] = c;
        index++;
      }
      else if (c == '\n')
        continue;
      else {
        buffer[index % MAX_STRLEN] = '\0';
        MatchBufferResult match = matchBufferToPin(pins, pins_count, buffer);

        if (match.pin) {
          changePin(*match.pin, match.value);
        }
        else {
          Serial.print(buffer);
          Serial.println(" Not an option");
          printHelp(pins, pins_count);
        }
        index = 0;
        break;
      }
    }
  }
}

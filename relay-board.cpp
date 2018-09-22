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

void printHelp() {
  Serial.println("Welcome. Commands available are:");
  Serial.print("Separator is: ");
  Serial.println(SEPARATOR);
  Serial.println(CAMERA_ON);
  Serial.println(CAMERA_OFF);
  Serial.println(BOARD_ON);
  Serial.println(BOARD_OFF);
  Serial.println("Waiting for input");
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(camera_pin, OUTPUT);
  pinMode(board_pin, OUTPUT);
  digitalWrite(camera_pin, HIGH);
  digitalWrite(board_pin, HIGH);
  Serial.begin(57600);
  printHelp();
}

// the loop routine runs over and over again forever:
void loop() {

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
        if (!strcmp(buffer, CAMERA_ON)) {
          digitalWrite(camera_pin, LOW);
          Serial.println(CAMERA_ON);
        }
        else if (!strcmp(buffer, CAMERA_OFF)) {
          digitalWrite(camera_pin, HIGH);
          Serial.println(CAMERA_OFF);
        }
        else if (!strcmp(buffer, BOARD_ON)) {
          digitalWrite(board_pin, LOW);
          Serial.println(BOARD_ON);
        }
        else if (!strcmp(buffer, BOARD_OFF)) {
          digitalWrite(board_pin, HIGH);
          Serial.println(BOARD_OFF);
        }
        else {
          Serial.print(buffer);
          Serial.println(" Not an option");
          printHelp();
        }
        index = 0;
        break;
      }
    }
  }
}

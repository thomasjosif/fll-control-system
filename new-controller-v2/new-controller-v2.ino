/*
  FLL Controller v2.0 for FLL Tournaments in Canada
  Copyright (C) 2019 Thomas Dick <thomasjosif@outlook.com>
*/
#include <AbsMouse.h>

int STACK_RED = A4;
int STACK_YELLOW = A5;
int STACK_GREEN = A3;

int PILOT_RED = 8;
int PILOT_YELLOW = 7;
int PILOT_GREEN = 6;

int RESET_IND = A0;
int MATCH_START_IND = 9;
int MATCH_PRESTART_IND = A1;

int ESTOP_BUTTON = 1;
int ESTOP_RESET_BUTTON = 0;
int MATCH_START_BUTTON = 3;
int MATCH_PRESTART_BUTTON = 2;

int ESTOP_REF = 5;
int START_REF = A2;

#define MODE_ESTOP_DURING_MATCH -1
#define MODE_ESTOP 0
#define MODE_NOT_READY 1
#define MODE_REF_READY 2
#define MODE_PRESTART 3
#define MODE_MATCH_LIVE 4

#define MATCH_START 1
#define MATCH_STOP 2
#define MATCH_RELOAD 3

int iMode = MODE_ESTOP;
unsigned long currentMillis = 0;
unsigned long lastflash = 0;
unsigned long matchTime = 0;

int ledstate = HIGH;
int ledstate_stack = LOW;


void setup() {
    AbsMouse.init(1535, 863);
  // put your setup code here, to run once:
  pinMode(STACK_RED, OUTPUT);
  pinMode(STACK_YELLOW, OUTPUT);
  pinMode(STACK_GREEN, OUTPUT);

  pinMode(PILOT_RED, OUTPUT);
  pinMode(PILOT_YELLOW, OUTPUT);
  pinMode(PILOT_GREEN, OUTPUT);

  pinMode(RESET_IND, OUTPUT);
  pinMode(MATCH_START_IND, OUTPUT);
  pinMode(MATCH_PRESTART_IND, OUTPUT);

  pinMode(ESTOP_BUTTON, INPUT_PULLUP);
  pinMode(ESTOP_RESET_BUTTON, INPUT_PULLUP);
  pinMode(MATCH_START_BUTTON, INPUT_PULLUP);
  pinMode(MATCH_PRESTART_BUTTON, INPUT_PULLUP);
  pinMode(ESTOP_REF, INPUT_PULLUP);
  pinMode(START_REF, INPUT_PULLUP);
  // LAMP TEST
  digitalWrite(STACK_RED, HIGH);
  digitalWrite(STACK_YELLOW, HIGH);
  digitalWrite(STACK_GREEN, HIGH);
  digitalWrite(PILOT_RED, LOW);
  digitalWrite(PILOT_YELLOW, LOW);
  digitalWrite(PILOT_GREEN, LOW);
  digitalWrite(RESET_IND, LOW);
  digitalWrite(MATCH_START_IND, LOW);
  digitalWrite(MATCH_PRESTART_IND, LOW);
  delay(3000);
  digitalWrite(STACK_RED, LOW);
  digitalWrite(STACK_YELLOW, LOW);
  digitalWrite(STACK_GREEN, LOW);
  digitalWrite(PILOT_RED, HIGH);
  digitalWrite(PILOT_YELLOW, HIGH);
  digitalWrite(PILOT_GREEN, HIGH);
  digitalWrite(RESET_IND, HIGH);
  digitalWrite(MATCH_START_IND, HIGH);
  digitalWrite(MATCH_PRESTART_IND, HIGH);

}

void loop() {
  currentMillis = millis();
  bool ledupdate = false;

 if (currentMillis > lastflash) {
    // save the last time you blinked the LED
    lastflash = currentMillis+500;

    // if the LED is off turn it on and vice-versa:
    if (ledstate == LOW) {
      ledstate = HIGH;
    } else {
      ledstate = LOW;
    }
    if (ledstate_stack == LOW) {
      ledstate_stack = HIGH;
    } else {
      ledstate_stack = LOW;
    }
    ledupdate = true;
  }
/*
#define MODE_ESTOP_RESETTING = -2;
#define MODE_ESTOP_DURING_MATCH = -1;
#define MODE_ESTOP = 0;
#define MODE_NOT_READY = 1;
#define MODE_REF_READY = 2;
#define MODE_PRESTART = 3;
#define MODE_MATCH_LIVE = 4;
 */

switch (iMode) {

  case MODE_ESTOP_DURING_MATCH:
    digitalWrite(RESET_IND, LOW);
    if(ledupdate)
    {
      digitalWrite(PILOT_RED, ledstate);
      digitalWrite(STACK_RED, ledstate_stack);
    }
    if(digitalRead(ESTOP_RESET_BUTTON) == LOW)
    {
      iMode = MODE_NOT_READY;
      digitalWrite(STACK_RED, LOW);
      digitalWrite(PILOT_RED, HIGH);
      digitalWrite(RESET_IND, HIGH);
    }
    break;
  case MODE_ESTOP:
    digitalWrite(STACK_RED, HIGH);
    digitalWrite(PILOT_RED, LOW);
    digitalWrite(RESET_IND, LOW);
    if(digitalRead(ESTOP_RESET_BUTTON) == LOW)
    {
      iMode = MODE_NOT_READY;
      digitalWrite(STACK_RED, LOW);
      digitalWrite(PILOT_RED, HIGH);
      digitalWrite(RESET_IND, HIGH);
    }

    break;    
  case MODE_NOT_READY:
    digitalWrite(STACK_YELLOW, HIGH);
    digitalWrite(PILOT_YELLOW, LOW);
    if(digitalRead(START_REF) == LOW)
    {
      iMode = MODE_REF_READY;
    }
    if(digitalRead(ESTOP_BUTTON) == LOW || digitalRead(ESTOP_REF) == LOW)
    {
      iMode = MODE_ESTOP;
      digitalWrite(STACK_YELLOW, LOW);
      digitalWrite(PILOT_YELLOW, HIGH);
    }    
    break;
  case MODE_REF_READY:
    digitalWrite(MATCH_PRESTART_IND, LOW);
    if(ledupdate)
    {
      digitalWrite(PILOT_YELLOW, ledstate);
      digitalWrite(STACK_YELLOW, ledstate_stack);
    }
    if(digitalRead(MATCH_PRESTART_BUTTON) == LOW)
    {
      iMode = MODE_PRESTART;
      match_control(MATCH_RELOAD);
      digitalWrite(STACK_YELLOW, LOW);
      digitalWrite(PILOT_YELLOW, HIGH);
      digitalWrite(MATCH_PRESTART_IND, HIGH);
      digitalWrite(STACK_GREEN, HIGH);
      digitalWrite(PILOT_GREEN, LOW);
    }
    if(digitalRead(ESTOP_BUTTON) == LOW || digitalRead(ESTOP_REF) == LOW)
    {
      iMode = MODE_ESTOP;
      digitalWrite(STACK_YELLOW, LOW);
      digitalWrite(PILOT_YELLOW, HIGH);
      digitalWrite(MATCH_PRESTART_IND, HIGH);
    }    
    break;
  case MODE_PRESTART:
    digitalWrite(MATCH_START_IND, LOW);
    if(ledupdate)
    {
      digitalWrite(PILOT_GREEN, ledstate);
      digitalWrite(STACK_GREEN, ledstate_stack);
    }
    if(digitalRead(MATCH_START_BUTTON) == LOW)
    {
      matchTime = currentMillis+150000;
      iMode = MODE_MATCH_LIVE;
      match_control(MATCH_START);
      digitalWrite(STACK_GREEN, LOW);
      digitalWrite(PILOT_GREEN, HIGH);
      digitalWrite(MATCH_START_IND, HIGH);
    }
    if(digitalRead(ESTOP_BUTTON) == LOW || digitalRead(ESTOP_REF) == LOW)
    {
      iMode = MODE_ESTOP;
      digitalWrite(STACK_GREEN, LOW);
      digitalWrite(PILOT_GREEN, HIGH);
      digitalWrite(MATCH_START_IND, HIGH);      
    }
    break;
  case MODE_MATCH_LIVE:
    digitalWrite(STACK_GREEN, HIGH);
    digitalWrite(PILOT_GREEN, LOW);

    if(digitalRead(ESTOP_BUTTON) == LOW || digitalRead(ESTOP_REF) == LOW)
    {
      iMode = MODE_ESTOP_DURING_MATCH;
      match_control(MATCH_STOP);
      digitalWrite(STACK_GREEN, LOW);
      digitalWrite(PILOT_GREEN, HIGH);
    }
    if(currentMillis > matchTime)
    {
      iMode = MODE_NOT_READY;
      digitalWrite(STACK_GREEN, LOW);
      digitalWrite(PILOT_GREEN, HIGH);
    }
    break;
  default:
    // statements
    break;
}

}

void match_control(int value) {
    switch(value)
    {
      case MATCH_START:
        AbsMouse.move(484, 752);
        AbsMouse.press(MOUSE_LEFT);
        AbsMouse.release(MOUSE_LEFT);
      break;
      case MATCH_STOP:
        AbsMouse.move(762, 752);
        AbsMouse.press(MOUSE_LEFT);
        AbsMouse.release(MOUSE_LEFT);
      break;
      case MATCH_RELOAD:
        AbsMouse.move(762, 752);
        AbsMouse.press(MOUSE_LEFT);
        AbsMouse.release(MOUSE_LEFT);
      break;
    }
}

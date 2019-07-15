/*
  This is original hacked code written the day before the event when this system was implemented. 
  I highly recommend not using this. 
*/
 
#define STATE_ESTOP 1
#define STATE_NOTREADY 2
#define STATE_WAITING 3
#define STATE_PRESTART 4
#define STATE_MATCH_LIVE 5
 
// constants won't change. Used here to set a pin number :
const int redLED =  10;      // the number of the LED pin
const int yellowLED = 11;
const int greenLED = 12;
 
const int startbutton = 3;
const int stopbutton = 4;
const int resetbutton = 2;
const int ledtestbutton = 6;
const int refstartbutton = 7;
 
// Variables will change :
int waitingSTATE = LOW;        
int estopSTATE = LOW;
int startSTATE = LOW;
 
int state = STATE_NOTREADY;
bool LEDTEST = false;
 
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long startMillis = 0;        // will store last time LED was updated
unsigned long prestartMillis = 0;
 
 
// constants won't change :
const long interval = 500;           // interval at which to blink (milliseconds)
const long intervalmatch = 152000;
const long intervalbutton = 2000;
 
void setup() {
  // set the digital pin as output:
  AbsoluteMouse.begin();
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
 
  pinMode(startbutton, INPUT_PULLUP);
  pinMode(stopbutton, INPUT_PULLUP);
  pinMode(resetbutton, INPUT_PULLUP);
  pinMode(ledtestbutton, INPUT_PULLUP);
  pinMode(refstartbutton, INPUT_PULLUP);
}
 
void loop() {
  unsigned long currentMillis = millis();
 
  if(digitalRead(ledtestbutton) == LOW)
  {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, HIGH);
    LEDTEST = true;
  }
  else if(LEDTEST)
  {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    LEDTEST = false;
  }
  else
  {
    if(digitalRead(resetbutton) == LOW && digitalRead(stopbutton) == HIGH)
    {
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, HIGH);
      digitalWrite(yellowLED, LOW);
      state = STATE_NOTREADY;
    }
    else
    {
      if(digitalRead(stopbutton) == LOW && state != STATE_ESTOP && digitalRead(refstartbutton) != LOW)
      {
        //Mouseclick stop
        // Mouse.move(1185, 851);
         AbsoluteMouse.moveTo(27000, 28000);
         AbsoluteMouse.click();
         state = STATE_ESTOP;
         digitalWrite(redLED, HIGH);
         digitalWrite(yellowLED, LOW);
         digitalWrite(greenLED, LOW);
         estopSTATE = HIGH;
         startMillis = 0;
      }
      if(digitalRead(refstartbutton) == LOW && state == STATE_NOTREADY)
      {
        AbsoluteMouse.moveTo(15000, 28000);
        AbsoluteMouse.click();
        state = STATE_WAITING;
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
        waitingSTATE = HIGH;
      }
      if(digitalRead(startbutton) == HIGH)
      {
        if(state == STATE_WAITING)
        {
          AbsoluteMouse.moveTo(15000, 28000);
          AbsoluteMouse.click();
          state = STATE_PRESTART;
          digitalWrite(redLED, LOW);
          digitalWrite(yellowLED, LOW);
          digitalWrite(greenLED, HIGH);
          startSTATE = HIGH;
          prestartMillis = currentMillis;
        }
        else if(state == STATE_PRESTART && currentMillis - prestartMillis >= intervalbutton)
        {
          //Mouse.move(237, 849);
          AbsoluteMouse.moveTo(5000, 28000);
          AbsoluteMouse.click();
          state = STATE_MATCH_LIVE;
          digitalWrite(redLED, LOW);
          digitalWrite(yellowLED, LOW);
          digitalWrite(greenLED, HIGH);
          startMillis = currentMillis;
        }
      }
 
      if(digitalRead(refstartbutton) == LOW && digitalRead(stopbutton) == LOW)
      {
          AbsoluteMouse.moveTo(15000, 28000);
          AbsoluteMouse.click();
          AbsoluteMouse.moveTo(5000, 28000);
          AbsoluteMouse.click();
          state = STATE_MATCH_LIVE;
          digitalWrite(redLED, LOW);
          digitalWrite(yellowLED, HIGH);
          digitalWrite(greenLED, HIGH);
          startMillis = currentMillis;
      }
 
      if (currentMillis - startMillis >= intervalmatch && state == STATE_MATCH_LIVE)
      {
        state = STATE_NOTREADY;
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
      }
 
     
      if(state == STATE_NOTREADY)
      {
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
      }
     
   
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
 
        if(state == STATE_ESTOP)
        {
          digitalWrite(yellowLED, LOW);
          digitalWrite(greenLED, LOW);
          if(estopSTATE == LOW)
          {
            digitalWrite(redLED, HIGH);
            estopSTATE = HIGH;
          }
          else
          {
            digitalWrite(redLED, LOW);
            estopSTATE = LOW;
          }
        }
 
        if(state == STATE_WAITING)
        {
          digitalWrite(redLED, LOW);
          digitalWrite(greenLED, LOW);
          if(waitingSTATE == LOW)
          {
            digitalWrite(yellowLED, HIGH);
            waitingSTATE = HIGH;
          }
          else
          {
            digitalWrite(yellowLED, LOW);
            waitingSTATE = LOW;
          }
        }
 
 
        if(state == STATE_PRESTART)
        {
          digitalWrite(yellowLED, LOW);
          digitalWrite(redLED, LOW);
          if(startSTATE == LOW)
          {
            digitalWrite(greenLED, HIGH);
            startSTATE = HIGH;
          }
          else
          {
            digitalWrite(greenLED, LOW);
            startSTATE = LOW;
          }
        }
       
      }
    }
  }
}

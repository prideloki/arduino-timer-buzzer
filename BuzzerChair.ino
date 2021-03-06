#include <Timer.h>
//lib https://github.com/aron-bordin/PNG-Arduino-Framework

//inputs outputs
int buzzer = 4;
int switchPin = 9;
int ledPin = 13;
int leftButton = 7;
int rightButton = 5;
int leftLED = 8;
int rightLED = 6;

#define BUZZED 0
#define L1 1
#define R1 2
#define L2 3
#define R2 4
#define STOP_TIMER 5
#define STOP_BUZZED 6

int timerState = LOW;
int readingSwitch;
int previousSwitch = LOW;
int buzzerState = LOW;
int switchState = STOP_BUZZED;
int lBtnReading;
int prevLBtn = LOW;
int rBtnReading;
int prevRBtn = LOW;

long timePressed = 0;
long leftBtnPressed = 0;
long rightBtnPressed = 0;
long debounce = 200;

long startTime;
long totalTime = 0;
int sittingPeriod = 10 * 1000; //5*60*1000;// 5 mins

Timer *timer = new Timer(sittingPeriod);


void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  timer->setOnTimer(&alert);
  startTime = 0;
  switchState = STOP_BUZZED;
}

void loop()
{
  readingSwitch = digitalRead(switchPin);
  lBtnReading = digitalRead(leftButton);
  rBtnReading = digitalRead(rightButton);
  timer->Update();
  if (readingSwitch == HIGH && previousSwitch == LOW && millis() - timePressed > debounce) {
    if (timerState == LOW) { //start timing
      startTime = millis();
      timer->Start();
      timerState = HIGH;
    }
    else { //stop timing
      totalTime = millis() - startTime;
      startTime = 0;
      //send data to temboo
      timer->Stop();
      buzzerState = LOW;
      timerState = LOW;
      digitalWrite(leftLED, LOW);
      digitalWrite(rightLED, LOW);
      switchState = STOP_BUZZED;
    }
    timePressed = millis();
  }

  if (lBtnReading == HIGH && prevLBtn == LOW && millis() - leftBtnPressed > debounce) {
    if (switchState == BUZZED) {
      switchState = L1;
    }
    if (switchState == R1) {
      switchState = L2;
    }
    leftBtnPressed = millis();

  }

  if (rBtnReading == HIGH && prevRBtn == LOW && millis() - rightBtnPressed > debounce) {
    if (switchState == L1) {
      switchState = R1;
    }
    if (switchState == L2) {
      switchState = R2;
    }
    rightBtnPressed = millis();
  }

  switch (switchState) {
    case BUZZED:
      timer->Stop();
      digitalWrite(leftLED, HIGH);
      break;
    case L1:
      digitalWrite(rightLED, HIGH);
      digitalWrite(leftLED, LOW);
      break;
    case R1:
      digitalWrite(leftLED, HIGH);
      digitalWrite(rightLED, LOW);
      break;
    case L2:
      digitalWrite(rightLED, HIGH);
      digitalWrite(leftLED, LOW);
      break;
    case R2:
      digitalWrite(rightLED, LOW);
      switchState = STOP_TIMER;
      break;
    case STOP_TIMER:
      timerState = LOW;
      switchState = STOP_BUZZED;
      break;
    case STOP_BUZZED:
      buzzerState = LOW;
      break;
  }

  digitalWrite(ledPin, timerState); //led indicate whether the timer is running
  digitalWrite(buzzer, buzzerState);
  previousSwitch = readingSwitch;
  prevRBtn = rBtnReading;
  prevLBtn = lBtnReading;
}

void alert() {
  buzzerState = HIGH;
  switchState = BUZZED;
  digitalWrite(buzzer, buzzerState);
}


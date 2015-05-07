#include <Timer.h>
//lib https://github.com/aron-bordin/PNG-Arduino-Framework

//inputs outputs
int buzzer = 6;
int switchPin = 7;   // pushbutton connected to digital pin 7
int ledPin = 8; // LED connected to digital pin 8
int leftButton = 9;
int rightButton = 10;
int leftLED = 11;
int rightLED = 12;

#define BUZZED 0
#define L1 1
#define R1 2
#define L2 3
#define R2 4
#define STOP_BUZZED 5

int timerState = LOW;     // variable to store the read value
int reading;
int previous = LOW;
int buzzerState = LOW;
int switchState = STOP_BUZZED;
int lButton;
int rButton;

long timePressed = 0;//don't change the var name
long debounce = 200;

long startTime;
long totalTime = 0;
int sittingPeriod = 10000;//5*60*1000;// 5 mins

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
}

void loop()
{
  reading = digitalRead(switchPin);
  lButton = digitalRead(leftButton);
  rButton = digitalRead(rightButton);
  timer->Update();
  if(reading == HIGH && previous == LOW && millis()-timePressed > debounce){
    if(timerState == LOW){ //start timing
      startTime = millis();
      timer->Start();
      timerState = HIGH;
    }
    else { //stop timing
      totalTime = millis()-startTime;
      startTime = 0;
      //send data to temboo
      timer->Stop();
      buzzerState = LOW;
      timerState = LOW;
//      digitalWrite(buzzer,buzzerState);
      switchState = STOP_BUZZED;
    }
    timePressed = millis();
  }
  if (buzzerState == HIGH){
    // FSM
    switch (switchState) {
      case BUZZED:
        digitalWrite(leftLED,HIGH);
        if(lButton ==HIGH && millis()-timePressed > debounce){
          digitalWrite(rightLED,HIGH);
          digitalWrite(leftLED,LOW);
          switchState = L1;
        }
        break;
      case L1:
        if(rButton == HIGH && millis()-timePressed > debounce){
          digitalWrite(leftLED,HIGH);
          digitalWrite(rightLED,LOW);
          switchState = R1;
        }
        break;
      case R1:
        if(lButton == HIGH && millis()-timePressed > debounce){
          digitalWrite(rightLED,HIGH);
          digitalWrite(leftLED,LOW);
          switchState = L2;
        }
        break;
      case L2:
        if(rButton == HIGH && millis()-timePressed > debounce){
          digitalWrite(rightLED,LOW);
          switchState = STOP_BUZZED;
        }
        break;
      case STOP_BUZZED:
        buzzerState = LOW;
//        digitalWrite(buzzer,buzzerState);
        break;
        
    }
  }
  digitalWrite(ledPin,timerState); //led indicate whether the timer is running
  digitalWrite(buzzer,buzzerState);
  previous = reading;
}

void alert(){
  buzzerState = HIGH;
  switchState = BUZZED;
  digitalWrite(buzzer,buzzerState);
}


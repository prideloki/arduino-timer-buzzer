#include <Timer.h>
//lib https://github.com/aron-bordin/PNG-Arduino-Framework

int ledPin = 8; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int state = LOW;     // variable to store the read value
int reading;
int previous = LOW;

int sittingPeriod = 10000;//5*60*1000;// 5 mins

long timePressed = 0;//don't change the var name P'Top
long debounce = 200;

long startTime = 0;//when the timer start
long totalTime = 0;

Timer *timer = new Timer(sittingPeriod);


void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output
  pinMode(inPin, INPUT);      // sets the digital pin 7 as input
  timer->setOnTimer(&alert);

}

void loop()
{
  reading = digitalRead(inPin);
  timer->Update();
  if(reading == HIGH && previous == LOW && millis()-timePressed > debounce){
    if(state == LOW){ //start timing
      totalTime = 0;//reset timer
      startTime = millis();
      timer->Start();
      state = HIGH;
    }
    else { //stop timing
      totalTime = millis()-startTime;
      startTime = 0;
      //send data to temboo
      timer->Stop();
      state = LOW;
    }
    timePressed = millis();
  }
  digitalWrite(ledPin,state);
  previous = reading;
}

void alert(){
  //alert via email
  digitalWrite(ledPin,LOW);
  delay(1000);
  digitalWrite(ledPin,HIGH);
  delay(1000);
  digitalWrite(ledPin,LOW);
}


#include <TimerOne.h>

// XXX_T -> traffic light
// XXX_P -> pedestrian light

#define RED_T 4
#define YELLOW 5
#define GREEN_T 6
#define RED_P 7
#define GREEN_P 8

int count = 0;
int pedestrian_flag = 0;
int pedestrian_count = 0;
int toogle = 0;
int night_flag = 0;
unsigned int timer_to_wake = 0;
unsigned int timer_to_sleep = 0;


void ISR_button()
{
  pedestrian_flag = 1;
  pedestrian_count = count;
  digitalWrite(13,HIGH);
}


void ISR_timer()
{
  
  if (analogRead(A0) > 400)
  {
    timer_to_wake = 0;
    timer_to_sleep += 1;
  }
  else
  {
    timer_to_sleep = 0;
    timer_to_wake += 1;
  }

  if(pedestrian_flag == 0 && night_flag == 0)
  {
    switch(count)
    {
      case 25 :
        digitalWrite(RED_T,LOW);
        digitalWrite(GREEN_P,LOW);
        count = -1;
        break;
      case 0 :
        digitalWrite(YELLOW,LOW);
        digitalWrite(RED_T,LOW);
        digitalWrite(GREEN_T,HIGH);
        digitalWrite(RED_P,HIGH);
        break;
      case 10 :
        digitalWrite(GREEN_T,LOW);
        digitalWrite(YELLOW,HIGH);
        break;
      case 15 :
        digitalWrite(YELLOW,LOW);
        digitalWrite(RED_T,HIGH);
        digitalWrite(RED_P,LOW);
        digitalWrite(GREEN_P,HIGH);
        break;
     } 
  }
  else if(pedestrian_flag == 1 && night_flag == 0)
    {
        if(count == pedestrian_count + 5)
        {
          digitalWrite(RED_T,LOW);
          digitalWrite(GREEN_T,LOW);
          digitalWrite(YELLOW,HIGH);
          
        }
        if(count == pedestrian_count + 10)
        {
          digitalWrite(YELLOW,LOW);
          digitalWrite(RED_T,HIGH);
          digitalWrite(GREEN_P,HIGH);
          digitalWrite(RED_P,LOW);
        }
      if(count >= pedestrian_count + 20 && count <= pedestrian_count + 25)
      {
        if(toogle == 0)
        {
          digitalWrite(GREEN_P,LOW);
          toogle = 1;
        }
        else if(toogle == 1)
        {
          digitalWrite(GREEN_P,HIGH);
          toogle = 0;
        }
        if(count == pedestrian_count + 25)
        {
          digitalWrite(GREEN_P,LOW);
          digitalWrite(RED_P,HIGH);
          count = -1;
          pedestrian_count = 0;
          pedestrian_flag = 0;
        }
      }  
    }
    else if(night_flag == 1)
    {
      digitalWrite(RED_T,LOW);
      digitalWrite(RED_P,LOW);
      digitalWrite(GREEN_T,LOW);
      digitalWrite(GREEN_P,LOW);
      if(toogle == 0)
        {
          digitalWrite(YELLOW,LOW);
          toogle = 1;
        }
        else if(toogle == 1)
        {
          digitalWrite(YELLOW,HIGH);
          toogle = 0;
        }
      count = -1;
    }
    count += 1;
  }


void setup() {
  // put your setup code here, to run once:
  pinMode(3, INPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(13,OUTPUT);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(ISR_timer);
  attachInterrupt(digitalPinToInterrupt(3),ISR_button,RISING);
  digitalWrite(13,LOW);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (timer_to_sleep > 5)
  {
    night_flag = 1;
    digitalWrite(13,HIGH);
  }
  else if(timer_to_wake > 5)
  {
    night_flag = 0;
  }

  

}

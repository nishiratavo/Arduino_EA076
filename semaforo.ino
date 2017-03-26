#include <TimerOne.h>

// XXX_T -> traffic light
// XXX_P -> pedestrian light

#define RED_T 4
#define YELLOW 5
#define GREEN_T 6
#define RED_P 7
#define GREEN_P 8
#define TOP 9
#define RIGHT_TOP 10
#define LEFT_TOP 11
#define MIDDLE 12
#define BOTTOM A1
#define RIGHT_BOTTOM A2
#define LEFT_BOTTOM 2
int count = 0;
int display_count = 0;
int pedestrian_flag = 0;
int pedestrian_count = 0;
int toogle = 0;
int night_flag = 0;
unsigned int timer_to_wake = 0;
unsigned int timer_to_sleep = 0;

void timer_display(int number)
{
  switch(number)
  {
    case 0 :
      digitalWrite(TOP,HIGH);
      digitalWrite(RIGHT_TOP,HIGH);
      digitalWrite(LEFT_TOP,HIGH);
      digitalWrite(MIDDLE,LOW);
      digitalWrite(BOTTOM,HIGH);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,HIGH);
      break;
    case 1 :
      digitalWrite(TOP,LOW);
      digitalWrite(RIGHT_TOP,HIGH);
      digitalWrite(LEFT_TOP,LOW);
      digitalWrite(MIDDLE,LOW);
      digitalWrite(BOTTOM,LOW);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,LOW);
      break;
    case 2 :
      digitalWrite(TOP,HIGH);
      digitalWrite(RIGHT_TOP,HIGH);
      digitalWrite(LEFT_TOP,LOW);
      digitalWrite(MIDDLE,HIGH);
      digitalWrite(BOTTOM,HIGH);
      digitalWrite(RIGHT_BOTTOM,LOW);
      digitalWrite(LEFT_BOTTOM,HIGH);
      break;
    case 3 :
      digitalWrite(TOP,HIGH);
      digitalWrite(RIGHT_TOP,HIGH);
      digitalWrite(LEFT_TOP,LOW);
      digitalWrite(MIDDLE,HIGH);
      digitalWrite(BOTTOM,HIGH);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,LOW);
      break;
    case 4 :
      digitalWrite(TOP,LOW);
      digitalWrite(RIGHT_TOP,HIGH);
      digitalWrite(LEFT_TOP,HIGH);
      digitalWrite(MIDDLE,HIGH);
      digitalWrite(BOTTOM,LOW);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,LOW);
      break;
    case 5 :
      digitalWrite(TOP,HIGH);
      digitalWrite(RIGHT_TOP,LOW);
      digitalWrite(LEFT_TOP,HIGH);
      digitalWrite(MIDDLE,HIGH);
      digitalWrite(BOTTOM,HIGH);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,LOW);
      break;
    case 6 :
      digitalWrite(TOP,HIGH);
      digitalWrite(RIGHT_TOP,LOW);
      digitalWrite(LEFT_TOP,HIGH);
      digitalWrite(MIDDLE,HIGH);
      digitalWrite(BOTTOM,HIGH);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,HIGH);
      break;
    case 7 :
      digitalWrite(TOP,HIGH);
      digitalWrite(RIGHT_TOP,HIGH);
      digitalWrite(LEFT_TOP,LOW);
      digitalWrite(MIDDLE,LOW);
      digitalWrite(BOTTOM,LOW);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,LOW);
      break;
    case 8 :
      digitalWrite(TOP,HIGH);
      digitalWrite(RIGHT_TOP,HIGH);
      digitalWrite(LEFT_TOP,HIGH);
      digitalWrite(MIDDLE,HIGH);
      digitalWrite(BOTTOM,HIGH);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,HIGH);
      break;
    case 9 :
      digitalWrite(TOP,HIGH);
      digitalWrite(RIGHT_TOP,HIGH);
      digitalWrite(LEFT_TOP,HIGH);
      digitalWrite(MIDDLE,HIGH);
      digitalWrite(BOTTOM,LOW);
      digitalWrite(RIGHT_BOTTOM,HIGH);
      digitalWrite(LEFT_BOTTOM,LOW);
      break;
  }
}

void traffic_light(int color)
{
  if (color == 4)
  {
    digitalWrite(RED_T,HIGH);
    digitalWrite(YELLOW,LOW);
    digitalWrite(GREEN_T,LOW);
  }
  else if (color == 5)
  {
    digitalWrite(YELLOW,HIGH);
    digitalWrite(RED_T,LOW);
    digitalWrite(GREEN_T,LOW);
  }
  else if (color == 6)
  {
    digitalWrite(YELLOW,LOW);
    digitalWrite(RED_T,LOW);
    digitalWrite(GREEN_T,HIGH);
  }
}

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
        digitalWrite(GREEN_P,LOW);
        count = -1;
        break;
      case 0 :
        traffic_light(GREEN_T);
        digitalWrite(RED_P,HIGH);
        display_count = 9;
        break;
      case 10 :
        traffic_light(YELLOW);
        display_count = 14;
        break;
      case 15 :
        traffic_light(RED_T);
        digitalWrite(RED_P,LOW);
        digitalWrite(GREEN_P,HIGH);
        display_count = 24;
        break;
     } 
     timer_display(display_count - count);
  }
  else if(pedestrian_flag == 1 && night_flag == 0)
    {
        if(count == pedestrian_count + 5)
        {
          digitalWrite(RED_T,LOW);
          digitalWrite(GREEN_T,LOW);
          digitalWrite(YELLOW,HIGH);
          display_count = pedestrian_count + 9;
        }
        if(count == pedestrian_count + 10)
        {
          digitalWrite(YELLOW,LOW);
          digitalWrite(RED_T,HIGH);
          digitalWrite(GREEN_P,HIGH);
          digitalWrite(RED_P,LOW);
          display_count = pedestrian_count + 18;
        }
        if(count >= pedestrian_count + 14 && count <= pedestrian_count + 19)
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
          if(count == pedestrian_count + 19)
          {
            digitalWrite(GREEN_P,LOW);
            digitalWrite(RED_P,HIGH);
            count = -1;
            pedestrian_count = 0;
            pedestrian_flag = 0;
          }
        } 
        if (count >= 0)
        {
          timer_display(display_count - count);
        }

    }
    else if(night_flag == 1)
    {
      pedestrian_flag = 0;
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
      timer_display(0);
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
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(A2,OUTPUT);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(ISR_timer);
  attachInterrupt(digitalPinToInterrupt(3),ISR_button,RISING);
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
    digitalWrite(13,LOW);
  }

  

}

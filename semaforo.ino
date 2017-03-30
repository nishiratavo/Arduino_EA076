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
int count = 0; // count time in seconds
int display_count = 0;
int pedestrian_flag = 0;
int pedestrian_count = 0; // saves count value when there are pedestrians
int toogle = 0;
int night_flag = 0;  
unsigned int timer_to_wake = 0; // counter to  daylight mode
unsigned int timer_to_sleep = 0; // counter to nighttime mode

void timer_display(int number) // decoder for 7 segment display
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

void traffic_light(int color) // turn on/off LEDs like a traffic light
{
  if (color == RED_T)
  {
    digitalWrite(RED_T,HIGH);
    digitalWrite(YELLOW,LOW);
    digitalWrite(GREEN_T,LOW);
  }
  else if (color == YELLOW)
  {
    digitalWrite(YELLOW,HIGH);
    digitalWrite(RED_T,LOW);
    digitalWrite(GREEN_T,LOW);
  }
  else if (color == GREEN_T)
  {
    digitalWrite(YELLOW,LOW);
    digitalWrite(RED_T,LOW);
    digitalWrite(GREEN_T,HIGH);
  }
}

void pedestrian_light(int color) // turn on/off LEDs for pedestrian light
{
  if (color == GREEN_P)
  {
    digitalWrite(RED_P,LOW);
    digitalWrite(GREEN_P,HIGH);
  }
  if (color == RED_P)
  {
    digitalWrite(GREEN_P,LOW);
    digitalWrite(RED_P,HIGH);
  }
}

void toogle_led(int color)
{
  if(toogle == 0)
    {
      digitalWrite(color,LOW);
      toogle = 1;
    }
  else if(toogle == 1)
    {
      digitalWrite(color,HIGH);
      toogle = 0;
    }
}
void ISR_button() // activate flag for pedestrian
{
  pedestrian_flag = 1;
  pedestrian_count = count;
  digitalWrite(13,HIGH);
}


void ISR_timer()
{

  // LDR data for daylight and nighttime modes
  if (analogRead(A0) > 600) 
  {
    timer_to_wake = 0;
    timer_to_sleep += 1;
  }
  else
  {
    timer_to_sleep = 0;
    timer_to_wake += 1;
  }

  // daylight normal mode
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
        pedestrian_light(RED_P);
        display_count = 9;
        break;
      case 10 :
        traffic_light(YELLOW);
        display_count = 14;
        break;
      case 15 :
        traffic_light(RED_T);
        pedestrian_light(GREEN_P);
        display_count = 24;
        break;
     } 
     timer_display(display_count - count);
  }

  // pedestrian mode
  else if(pedestrian_flag == 1 && night_flag == 0) 
    {
        if(count == pedestrian_count + 5)
        {
          traffic_light(YELLOW);
          display_count = pedestrian_count + 9;
        }
        if(count == pedestrian_count + 10)
        {
          traffic_light(RED_T);
          pedestrian_light(GREEN_P);
          display_count = pedestrian_count + 18;
        }
        if(count >= pedestrian_count + 14 && count <= pedestrian_count + 19)
        {
          toogle_led(GREEN_P);
          if(count == pedestrian_count + 19)
          {
            pedestrian_light(RED_P);
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

    // nighttime mode
    else if(night_flag == 1) 
    {
      pedestrian_flag = 0;
      digitalWrite(RED_T,LOW);
      digitalWrite(RED_P,LOW);
      digitalWrite(GREEN_T,LOW);
      digitalWrite(GREEN_P,LOW);
      toogle_led(YELLOW);
      count = -1;
      timer_display(0);
    }
    count += 1;
  }


void setup() {
  pinMode(2,OUTPUT);
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
  pinMode(A2,OUTPUT);
  Timer1.initialize(1000000); // interrupt every 1 second
  Timer1.attachInterrupt(ISR_timer);
  attachInterrupt(digitalPinToInterrupt(3),ISR_button,RISING); // pedestrian interrupt
}

void loop() {
  // conditions for daylight and nighttime mode
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

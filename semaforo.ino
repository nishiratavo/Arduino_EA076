#include <TimerOne.h>

// XXX_T -> traffic light
// XXX_P -> pedestrian light

#define RED_T 4
#define YELLOW 5
#define GREEN_T 6
#define RED_P 7
#define GREEN_P 8

int count = 0;
int button_flag = 0;
int button_count = 0;
int flag = 0;
int toogle = 0;
int light_flag = 0;
int mean_light = 410;
int light_count_night = 0;
int light_count_day = 0;
int flag_save_count_night = 0;
int flag_save_count_day = 0;
int flag_day = 0;


void ISR_button()
{
  button_flag = 1;
  digitalWrite(13,HIGH);
}


void ISR_timer()
{
  if(analogRead(A0) < 400)
  {
    if(flag_save_count_night == 0)
    {
      light_count_night = count;
      flag_save_count_night = 1;
    }
    mean_light = mean_light + analogRead(A0);
    if(count == light_count_night + 5)
    {
      mean_light = mean_light/5;
    }
    
    if(mean_light < 400 && count == light_count_night + 5)
    {
      light_flag = 1;
      flag_day = 1;
      flag_save_count_day = 0;
      count = 0;
    }
    else if(count == light_count_night + 5)
    {
      flag_save_count_night = 0;
      mean_light = 0;
    }
  }

  else if(count == light_count_night + 5)
    {
      flag_save_count_night = 0;
      mean_light = 0;
    }
  
  else if(analogRead(A0) > 400 && flag_day == 1)
  {
    if(flag_save_count_day == 0)
    {
      light_count_day = count;
      flag_save_count_day = 1;
      mean_light = 0;
    }
    mean_light = mean_light + analogRead(A0);
    if(count == light_count_day + 5)
    {
      mean_light = mean_light/5;
    }
    
    if(mean_light > 400 && count == light_count_day + 5)
    {
      light_flag = 0;
      count = 0;
      flag_save_count_night = 0;
      flag_day = 0;
    }
    else if(count == light_count_day + 5)
    {
      flag_save_count_day = 0;
      mean_light = 0;
    }
  }
  else if(count == light_count_day + 5)
    {
      flag_save_count_day = 0;
      mean_light = 0;
    }
  
  if(button_flag == 0 && light_flag == 0)
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
  else if(light_flag == 0 && button_flag == 1)
    {
      if(flag == 0)
      {
        button_count = count;
        flag = 1;
      }
      if(count == button_count + 5)
      {
        digitalWrite(RED_T,LOW);
        digitalWrite(GREEN_T,LOW);
        digitalWrite(YELLOW,HIGH);
      }
      if(count == button_count + 10)
      {
        digitalWrite(YELLOW,LOW);
        digitalWrite(RED_T,HIGH);
        digitalWrite(GREEN_P,HIGH);
        digitalWrite(RED_P,LOW);
      }
      if(count >= button_count + 20 && count <= button_count + 25)
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
        if(count == button_count + 25)
        {
          digitalWrite(GREEN_P,LOW);
          digitalWrite(RED_P,HIGH);
          count = -1;
          button_count = 0;
          button_flag = 0;
          flag = 0;
        }
      }  
    }
    else if(light_flag == 1)
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
  

}

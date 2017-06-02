#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <arduino.h>

#define BUFFER_SIZE 15

int flag = 0; // flag to initiate control algorithm
int vel;
char data[15];
int buffer_position = 0;
int string_complete = 0;
int end_write = 0;
int count = 0;
volatile int count_int = 0;
float vel_measure = 0;
float error = 0;
int pwm = 0;
float a = 0.01;


ISR(TIMER2_OVF_vect) // timer interrupt for sampling time
{
	count++;
	if (count == 3)
	{
		vel_measure = count_int*300;
		count = 0;
		count_int = 0;
		flag = 1; 
	}
}

void ISR_button() // interrupt counts sensor interrupts
{
	//flag = 1;
	count_int++;
}



void buffer_add(char c)// saves UART message in the buffer
{
  if (buffer_position < BUFFER_SIZE)
  {
    data[buffer_position] = c;
    buffer_position += 1;
  }
}

void serialEvent() // receives UART messages and saves it in the buffer
{
  char c;
  while(Serial.available() > 0)
  {
      c = Serial.read();
      if (c == '\n')
      {
        buffer_add('\0');
        end_write = 1;
        buffer_position = 0;
        string_complete = 1;
      }
      else
      {
        buffer_add(c);
      }
      count = 0;
  }
}

void clean_buffer()
{
  for (int i = 0; i < BUFFER_SIZE; ++i)
  {
    data[i] = '\0';
    buffer_position = 0;
    string_complete = 0;
  }
}

void timer2() //timer for data aquisition
{
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
	TCNT2 = 0;
	TIMSK2 |= (1<<TOIE2);
	sei();
}
void setup()
{
	pinMode(3,INPUT); // external interrupt
	pinMode(4,OUTPUT); // IN1 pin in H-bridge for clockwise motion
	pinMode(5,OUTPUT); // PWM, pin ENA in H-bridge
	digitalWrite(2,HIGH);
	digitalWrite(4,HIGH);
	attachInterrupt(digitalPinToInterrupt(3),ISR_button,RISING); 
	Serial.begin(9600);
	timer2();
}


void loop()
{
// -------------Receives velocity from serial-----
	if (string_complete == 1)
	{
		char number[4];
		for (int i = 0; i < 4; ++i)
      {
        if (data[i] == '\0')
        {
          break;
        }

        number[i] = data[i];
        
      }
       vel = atoi(number); //vel
       clean_buffer();
	}
//-------------proportional control algorithm-----------------------
	if (flag == 1)
	{
		Serial.println(vel_measure);
		error = vel_measure - vel;
		pwm = pwm - a*error;
		if (pwm >= 255)
		{
			pwm = 255;
		}
		analogWrite(5, pwm);
		flag = 0;
	}
}

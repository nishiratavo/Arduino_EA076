#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <arduino.h>

#define BUFFER_SIZE 15

int flag = 0;
int vel;
char data[15];
int buffer_position = 0;
int string_complete = 0;
int end_write = 0;
int overflow = 0;
int count_int = 0;
float vel_measure = 0;


ISR(TIMER2_OVF_vect)
{
	overflow++;
}

void ISR_button()
{
	flag = 1;
	TCNT2 = 0;
	count_int++;
	if (overflow*255 + TCNT2 >= 781)
	{
		vel_measure = count_int/0.1;
		overflow = 0;
		count_int = 0;
	}
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

void timer2()
{
	TCCR2 |= (1<<CS22) | (1<<CS21) | (1<<CS20);
	TCNT2 = 0;
	TIMSK |= (1<<TOIE2);
	sei();
}
void setup()
{
	pinMode(2,OUTPUT);// enable A
	pinMode(3,INPUT); // interrupção
	pinMode(4,OUTPUT); // direção
	pinMode(5,OUTPUT); // PWM
	digitalWrite(2,HIGH);
	digitalWrite(4,HIGH);
	attachInterrupt(digitalPinToInterrupt(3),ISR_button,RISING);
	Serial.begin(9600);
	timer2();
}


void loop()
{
	Serial.println(overflow);
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
       vel = atoi(number);
       clean_buffer();
	}

	if (vel != 0)
	{
		//Serial.println(vel);
	}

	analogWrite(5, vel);
	if (flag == 1)
	{
		Serial.println("Interrompeu");
		flag = 0;
	}
}
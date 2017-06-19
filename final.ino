#include <TimerOne.h>


int note5 = 0;
int count5 = 0;
int button5_flag = 0;
int note6 = 0;
int count6 = 0;
int button6_flag = 0;


void ISR_button() // external interrupt
{
	// Press the pushbutton one time to turn note on and another time to turn off

	//----------------------------------------------------
	if ((digitalRead(7) == LOW) && (note6 == 0)) // turn note 333Hz on
	{
		note6 = 1;
	}
	else if ((digitalRead(7) == LOW) && (note6 == 1)) // turn note 333Hz off
	{
		note6 = 0;
	}
	//---------------------------------------------------

	if ((digitalRead(8) == LOW) && (note5 == 0)) // turn note 500Hz on
	{
		note5 = 1;
	}
	else if ((digitalRead(8) == LOW) && (note5 == 1)) // turn note 500Hz off
	
		note5 = 0;
	}



}




void ISR_timer() // interrupt occurs every 100us
{
	if (note6 == 1) // ~= 333 Hz
	{
		// conditional setting the square wave frequency
		if (count6 < 15)
		{
			digitalWrite(6,HIGH);
			count6++;
		}
		else if (count6 >= 15)
		{
			digitalWrite(6,LOW);
			count6++;
			if (count6 == 30)
			{
				count6 = 0;
			}
		}
	}
	else
	{
		digitalWrite(6,LOW);
	}
	//-------------------------------------------

	if (note5 == 1) //  ~= 500 Hz
	{
		// conditional setting the square wave frequency
		if (count5 < 10)
		{
			digitalWrite(5,HIGH);
			count5++;
		}
		else if (count5 >= 10)
		{
			digitalWrite(5,LOW);
			count5++;
			if (count5 == 20)
			{
				count5 = 0;
			}
		}
	}
	else
	{
		digitalWrite(5,LOW);
	}




}

void setup()
{
	pinMode(7,INPUT);
	pinMode(8,INPUT);
	pinMode(6,OUTPUT);
	pinMode(5,OUTPUT);
	pinMode(3,INPUT);
  	Timer1.initialize(100);
  	Timer1.attachInterrupt(ISR_timer);
  	attachInterrupt(digitalPinToInterrupt(3),ISR_button,FALLING); 

}


void loop()
{
	
}







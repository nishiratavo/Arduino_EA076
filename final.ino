#include <TimerOne.h>

int note4 = 0;
int count4 = 0;
int note5 = 0;
int count5 = 0;
int button5_flag = 0;
int note6 = 0;
int count6 = 0;
int button6_flag = 0;


void ISR_button()
{
	if ((digitalRead(0) == LOW) && (button6_flag == 0))
	{
		note6 = 1;
		button6_flag = 1;
		//Serial.println("E");
	}
	else if ((digitalRead(0) == LOW) && (button6_flag == 1))
	{
		note6 = 0;
		button6_flag = 0;
	}

	if ((digitalRead(1) == LOW) && (button5_flag == 0))
	{
		note5 = 1;
		button5_flag = 1;
	}
	else if ((digitalRead(1) == LOW) && (button5_flag == 1))
	{
		note5 = 0;
		button5_flag = 0;
	}


}




void ISR_timer()
{
	if (note6 == 1) // A 440 Hz
	{
		if (count6 < 11)
		{
			digitalWrite(6,HIGH);
			count6++;
		}
		else if (count6 >= 11)
		{
			digitalWrite(6,LOW);
			count6++;
			if (count6 == 22)
			{
				count6 = 0;
			}
		}
	}

	if (note5 == 1) // B ~= 493 Hz
	{
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

	if (note4 == 1) // C ~= 523 Hz
	{
		digitalWrite(4,HIGH);
		count4++;
		if (count4 >= 9)
		{
			digitalWrite(4,LOW);
			count4++;
			if (count4 == 18)
			{
				count4 = 0;
			}
		}
	}




}

void setup()
{
	pinMode(0,INPUT);
	pinMode(1,INPUT);
	//pinMode(7,INPUT);
	pinMode(6,OUTPUT);
	pinMode(5,OUTPUT);
	pinMode(4,OUTPUT);
	pinMode(3,INPUT);
	pinMode(2,OUTPUT);
	//Serial.begin(9600);
  	Timer1.initialize(100);
  	Timer1.attachInterrupt(ISR_timer);
  	attachInterrupt(digitalPinToInterrupt(3),ISR_button,FALLING); 
  	Serial.begin(9600);

}


void loop()
{
	
}







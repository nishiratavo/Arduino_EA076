#include <Wire.h>

#define BUFFER_SIZE 15

char data[BUFFER_SIZE];
int buffer_position = 0;
int end_write = 0;
int measure = 0;
int string_complete = 0;
int memory_position = 0;
int memory = 0;

int get_measure()
{
  return analogRead(A0);
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

int str_cmp(char *str1, char *str2, int size)
{
  int i;
  for (i = 0; i < size; ++i)
  {
    if (str1[i] != str2[i])
    {
      return 0;
    }
    else if (str1[i] == '\0')
    {
      return 1;
    }
  }
  return 1;
}

void buffer_add(char c)
{
  if (buffer_position < BUFFER_SIZE)
  {
    data[buffer_position] = c;
    buffer_position += 1;
  }
}

void serialEvent()
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
        /*for (int i = 0; i < 6; ++i)
        {
          Serial.write(data[i]);
        }*/
        string_complete = 1;
      }
      else
      {
        buffer_add(c);
      }
  }
}

void do_action()  // values from measure and record are different -> see if that is right 
{
  if (string_complete == 1)
  {
    if(str_cmp(data, "PING", 4))
    {
      Serial.println("PONG");
      clean_buffer();
    }

    else if (str_cmp(data, "ID", 2))
    {
      Serial.println("DATALOGGER NAME");
      clean_buffer();
    }

    else if (str_cmp(data, "MEASURE", 7))
    {
      Serial.println(get_measure());
      clean_buffer();
    }

    else if (str_cmp(data, "MEMSTATUS", 9))
    {
      read_memory(0);
      clean_buffer();
    }

    else if (str_cmp(data, "RESET", 5))
    {
      clean_memory();
      clean_buffer();
    }

    else if (str_cmp(data, "RECORD", 6)) // must not show the index value -> fix that 
    {
      save_data(get_measure());
      clean_buffer();
    }

    else if (str_cmp(data, "GET ", 4)) // terminar
    {
      char number[4];
      int j = 0;
      for (int i = 0; i < 4; ++i)
      {
        if (data[i+4] == '\0')
        {
          break;
        }

        number[i] = data[i+4];
        j++;
      }
      int a = atoi(number);
      read_memory(a);
      clean_buffer();
    }
  }

}

void write_memory(int memory_position, int data) // consecutives writes only work with delays -> solve that
{
  Wire.beginTransmission(0x50); // device address
  Wire.write(memory_position); // position
  Wire.write(data); // data
  Wire.endTransmission();
}

void read_memory(int pos) // maybe change read_memory to return int 
{
  Wire.beginTransmission(0x50);
  Wire.write(pos); // position
  Wire.endTransmission();
  Wire.requestFrom(0x50, 1); // device address and quantity of bytes
  if (Wire.available())
  {
    memory = Wire.read(); 
    Serial.println(memory);
  }
}

void clean_memory()
{
  write_memory(0, 0);
}

void save_data(int data)
{
  read_memory(0);  // reads index
  memory_position = memory; // saves index value
  write_memory(0, memory_position+1);  // increment index value
  delay(100);
  write_memory(memory_position+1, data); // saves data 
}


void setup()
{
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  Serial.begin(9600);
  digitalWrite(13, LOW);
  Wire.begin();
  write_memory(0, 0);
  delay(100);
  write_memory(9, 27);

}

void loop()
{
  //serial();
  do_action();
  //write_memory();
  //read_memory();
}


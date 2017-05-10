#include <Wire.h>
#include <TimerOne.h>

#define BUFFER_SIZE 15
#define C1 2
#define C2 3
#define C3 4
#define L1 5
#define L2 6
#define L3 7
#define L4 8
#define HASHTAG 11
#define STAR 10



char data[BUFFER_SIZE];
int buffer_position = 0;
int end_write = 0;
int measure = 0;
int string_complete = 0;
int memory_position = 0;
int memory = 0;
int map_matrix[4][3] = {{1,2,3}, {4,5,6}, {7,8,9}, {STAR,0,HASHTAG}};
int line[4] = {5,6,7,8};
int column[3] = {2,3,4};
int keyboard_value = 0;
int flag_char1 = 0;
int flag_char2 = 0;
int keyboard_message = 0;
int flag_led = 0;
int counter_led = 0;
int flag_measure = 0;
int counter_measure = 0;
int flag_save = 0;

void ISR_timer()
{
// little timer for led activated by the keyboard
  if (flag_led == 1)
  {
    counter_led += 1;
  }
  if (counter_led == 2)
  {
    digitalWrite(9,LOW);
    flag_led = 0;
    counter_led = 0;
  }
//--------------------------
  // flags to activate automatic measurements
  if (flag_measure == 1)
  {
    counter_measure += 1;
  }
  if (counter_measure == 2)
  {
    flag_save = 1;
    //Serial.println("Entrou");
    //keyboard_action(23);
    counter_measure = 0;
  } 
}

uint8_t get_measure() // returns the LDR data
{
  return map(analogRead(A0),0,1023,0,255);
}


//----------UART functions----------------------------------------
void clean_buffer()
{
  for (int i = 0; i < BUFFER_SIZE; ++i)
  {
    data[i] = '\0';
    buffer_position = 0;
    string_complete = 0;
  }
}

int str_cmp(char *str1, char *str2, int size) // compares messages received on UART with the protocol messages
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

void do_action() // interpret UART messages and do the actions for each message
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

    else if (str_cmp(data, "MEASURE", 7)) // prints LDR data
    {
      Serial.println(get_measure());
      clean_buffer();
    }

    else if (str_cmp(data, "MEMSTATUS", 9)) // prints how many blocks of memory are written
    {
      read_memory(0);
      clean_buffer();
    }

    else if (str_cmp(data, "RESET", 5)) // resets memory index
    {
      clean_memory();
      clean_buffer();
    }

    else if (str_cmp(data, "RECORD", 6)) // gets LDR data and saves it in the memory
    {
      save_data(get_measure());
      clean_buffer();
    }

    else if (str_cmp(data, "GET ", 4)) // returns data stored on memory
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
//-------------------------------------------------------------------

//---------------I2C functions-------------------------------------
void write_memory(int memory_position, int data) // i2c write
{
  Wire.beginTransmission(0x50); // device address
  Wire.write(memory_position); // position
  Wire.write(data); // data
  Wire.endTransmission();
}

void read_memory(int pos) // i2c read for UART functions
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

void read_memory_r(int pos) // i2c read for i2c internal functions
{
  Wire.beginTransmission(0x50);
  Wire.write(pos); // position
  Wire.endTransmission();
  Wire.requestFrom(0x50, 1); // device address and quantity of bytes
  if (Wire.available())
  {
    memory = Wire.read(); 
  }
}

void clean_memory() // resets memory index
{
  write_memory(0, 0);
}

void save_data(int data) // save data in the memory
{
  read_memory_r(0);  // reads index
  memory_position = memory; // saves index value
  if (memory_position == 255)
  {
    memory_position = 0;
  }
  write_memory(0, memory_position+1);  // increment index value
  delay(5);
  write_memory(memory_position+1, data); // saves data 
}
//--------------------------------------------------------

//-------------------keyboard functions----------------------
void sweep() // detects which keyboard number was used
{
  for (int i = 0; i < 4; ++i)
  {
    digitalWrite(line[i], LOW);
    for (int j = 0; j < 3; ++j)
    {
      if (digitalRead(column[j]) == LOW)
      {
        //Serial.println(map_matrix[i][j]);
        keyboard_value = map_matrix[i][j];

      }
    }
    digitalWrite(line[i], HIGH);
  }

}

void keyboard_protocol()// interprets the keyboard numbers
{
  if (keyboard_value == HASHTAG)
  {
    flag_char1 = 1;
    keyboard_message = keyboard_value;
  }
  if (flag_char1 == 1 && keyboard_value != HASHTAG)
  {
    flag_char2 = 1;
    keyboard_message += keyboard_value;
    flag_char1 = 0;
  }
  if (flag_char2 == 1 && keyboard_value == STAR)
  {
    keyboard_message += keyboard_value;
    flag_char2 = 0;
    flag_char1 = 0;
    keyboard_action(keyboard_message);
    //Serial.println(keyboard_message);
    keyboard_message = 0;
  }
}

void keyboard_action(int message) // do the actions corresponding to the keyboard messages
{
  switch(message)
  {
    case 22: // *1#
      digitalWrite(9,HIGH);
      flag_led = 1;
      break;

    case 23: // *2#
        save_data(get_measure());
        clean_buffer();
      break;

    case 24: // *3#
      flag_measure = 1;
      break;

    case 25: //*4#
      flag_measure = 0;
        break;
  }
}
//-------------------------------------------------------------






void setup()
{
  pinMode(9,OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(C1, INPUT_PULLUP);
  pinMode(C2, INPUT_PULLUP);
  pinMode(C3, INPUT_PULLUP);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  Serial.begin(9600);
  digitalWrite(13, LOW);
  Wire.begin();
  write_memory(0, 0);
  delay(5);
  write_memory(9, 27);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(ISR_timer);

}

void loop() // for loop design
{
  //serial();
  do_action();
  sweep();
  keyboard_protocol();
  if (flag_save == 1) // saves data when automatic measument mode is activated
      {

        save_data(get_measure());
        clean_buffer();
        flag_save = 0;
      }
}


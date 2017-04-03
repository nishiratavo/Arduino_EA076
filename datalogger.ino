#define BUFFER_SIZE 15

char data[BUFFER_SIZE];
int buffer_position = 0;
int end_write = 0;
int measure = 0;

void clean_buffer()
{
  for (int i = 0; i < BUFFER_SIZE; ++i)
  {
    data[i] = 0;
  }
}

int str_cmp(char *str1, char *str2, int size)
{
  int i;
  for (i = 0; i <= size; ++i)
  {
    if (str1[i] != str2[i])
    {
      return 0;
    }
    if (str1[i] == '\0')
    {
      return 1;
    }
  }
}

int char_to_int(char *number, int size)
{
  int n = 0;
  switch (size) 
  {
      case 5:
        n = number[0];
        break;
      case 6:
        n = number[0]*10 + number[1];
        break;
      case 7:
        n = number[0]*100 + number[1]*10 + number[2];
        break;
      case 8:
        n = number[0]*1000 + number[1]*100 + number[2]*10 + number[3];
        break;
  }
  return n;
}

void buffer_add(char c)
{
  if (buffer_position < BUFFER_SIZE)
  {
    data[buffer_position] = c;
    buffer_position += 1;
  }
}

void uart_receive() // trocar para serialEvent
{
  char c;
  while(Serial.available())
  {
      c = Serial.read();
      if (c == '\n')
      {
        buffer_add('\0');
        end_write = 1;
        buffer_position = 0;
      }
      else
      {
        buffer_add(c);
      }
  }
}

void do_action()
{
  if(str_cmp(data, "PING", 4))
  {
    Serial.println("PONG");
    clean_buffer();
  }

  if (str_cmp(data, "ID", 2))
  {
    Serial.println("DATALOGGER NAME");
    clean_buffer();
  }

  if (str_cmp(data, "MEASURE", 7))
  {
    Serial.println(get_measure());
    clean_buffer();
  }

  if (str_cmp(data, "MEMSTATUS", 7))
  {

    clean_buffer();
  }

  if (str_cmp(data, "RESET", 7))
  {

    clean_buffer();
  }

  if (str_cmp(data, "RECORD", 7))
  {
    get_measure(); // arrumar / usar memória
    clean_buffer();
  }

  if (str_cmp(data, "GET", 3))
  {
    char number[4];
    int j = 0;
    for (int i = 4; i <= 8; ++i)
    {
      if (data[i] == '\0')
      {
        break;
      }
      data[i] = number[j];
      j += 1;
    }
    int a = char_to_int(number, i); // terminar / usar memória
    clean_buffer();
  }

}

int get_measure()
{
  return analogRead(A0);
}



void setup()
{
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  digitalWrite(13, LOW);

}

void loop()
{
  uart_receive();
  do_action();
}


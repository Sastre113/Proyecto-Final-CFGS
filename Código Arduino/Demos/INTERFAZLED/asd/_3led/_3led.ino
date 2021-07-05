#define led_R 12
#define led_A 11
#define led_V 10

bool luzr= false;
bool luzA = false;
bool luzV = false;

int dato = 25;
char b;

void setup()
  {

    Serial.begin(9600);
    Serial1.begin(9600);
    pinMode(led_R, OUTPUT);
    pinMode(led_A, OUTPUT);
    pinMode(led_V, OUTPUT);

  }

  void loop()
  {
    Serial.write("Hola");
    luces();
  }

  void luces()
{
  if (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == '0')
    {
      if (luzr == false)
      {
        digitalWrite(led_R, HIGH);
        luzr = true;
        return;
      }
      if (luzr == true)
      {
        digitalWrite(led_R, LOW);
        luzr = false;
        return;
      }
    }

    if (c == '1')
    {
      if (luzA == false)
      {
        digitalWrite(led_A, HIGH);
        luzA = true;
        return;
      }
      if (luzA == true)
      {
        digitalWrite(led_A, LOW);
        luzA = false;
        return;
      }
    }

      if (c == '2')
      {
        if (luzV == false)
        {
          digitalWrite(led_V, HIGH);
          luzV = true;
          return;
        }
        if (luzV == true)
        {
          digitalWrite(led_V, LOW);
          luzV = false;
          return;
        }

      }
    }

  }

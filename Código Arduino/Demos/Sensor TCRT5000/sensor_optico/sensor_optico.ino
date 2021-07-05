
int sensor=8;
int led=2;
int estado;

void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(sensor,INPUT);
  pinMode(led,OUTPUT);
}
 
// the loop routine runs over and over again forever:
void loop() 
{
  estado=analogRead(A0);
  Serial.println(estado);

  if(estado==HIGH)
  {
    digitalWrite(led,HIGH);
  }
  if(estado==LOW)
  {
    digitalWrite(led,LOW);
  }
  delay(100);
}

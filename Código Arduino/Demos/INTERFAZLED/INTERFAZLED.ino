#define led 13
char x=2;

void setup() 
{
  
  Serial.begin(9600);
  pinMode(led,OUTPUT);
}

void loop() 
{
  if(Serial.available())
  {
    char c=Serial.read();

     if(c=='1')digitalWrite(led,HIGH);
     if(c=='0')digitalWrite(led,LOW);
  }
 Serial.write(3);
}

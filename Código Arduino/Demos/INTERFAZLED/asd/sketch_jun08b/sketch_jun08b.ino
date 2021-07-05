#define led_R 12
#define led_A 11
#define led_V 10

void setup() 
{
  
  Serial.begin(9600);
  pinMode(led_R,OUTPUT);
   pinMode(led_A,OUTPUT);
    pinMode(led_V,OUTPUT);
  
}

void loop() 
{
  if(Serial.available())
  {
    char c=Serial.read();

     if(c=='1')digitalWrite(led_R,HIGH);
     if(c=='0')digitalWrite(led_R,LOW);

     if(c=='3')digitalWrite(led_A,HIGH);
     if(c=='2')digitalWrite(led_A,LOW);

     if(c=='5')digitalWrite(led_V,HIGH);
     if(c=='4')digitalWrite(led_V,LOW);
     Serial.write("1=Recibido el dato");
  }

}

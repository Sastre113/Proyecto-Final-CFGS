#define led_R 12
#define led_A 11
#define led_V 10
#define CANT_LUCES 3
bool rojo=false;
bool amarillo=false;
bool verde=false;

int luces[CANT_LUCES];

void setup() 
{
  int i; 
  Serial.begin(9600);
  pinMode(led_R,OUTPUT);
   pinMode(led_A,OUTPUT);
    pinMode(led_V,OUTPUT);

    for(i=0;i<CANT_LUCES;i++)
    { 
      luces[i]=0;
    }
  
}

void loop() 
{ 
  int i;
  if(Serial.available())
  {
    char c=Serial.read();

     // IMPAR = TRUE, PAR= FALSE
     //ROJO
     if(c=='1')
     {
      rojo=true;
      luces[0]=1;
     }
     if(c=='0')
     {
      rojo=false;
      luces[0]=0;
     }

     //AMARILLO
     if(c=='3')
     {
      amarillo=true;
      luces[1]=1;
     }
     if(c=='2')
     {
      amarillo=false;
      luces[1]=0;
     }

     //VERDE
     if(c=='5')
     {
      verde=true;
      luces[2]=1;
     }
     if(c=='4')
     {
      verde=false;
      luces[2]=0;
     }

   if(c=='a')
     {
       for(i=0;i=CANT_LUCES;i++)
       {
        Serial.write("a=");
        Serial.write(luces[i]);
       }
     }
   
     Serial.write("1=Recibido el dato");
  }

  if(rojo==false)
  {
    digitalWrite(led_R,LOW);
  }
   if(rojo==true)
  {
    digitalWrite(led_R,HIGH);
  }

  if(amarillo==false)
  {
    digitalWrite(led_A,LOW);
  }
   if(amarillo==true)
  {
    digitalWrite(led_A,HIGH);
  }

  if(verde==false)
  {
    digitalWrite(led_V,LOW);
  }
   if(verde==true)
  {
    digitalWrite(led_V,HIGH);
  }
}

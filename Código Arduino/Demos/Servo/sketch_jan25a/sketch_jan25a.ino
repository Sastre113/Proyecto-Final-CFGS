#include <Servo.h>

Servo servo1;

int boton=8;
bool estado=false;

void setup() 
{
  Serial.begin(9600);
  servo1.attach(9); 
  servo1.write(110);
  pinMode(boton,INPUT);
}

void loop() 
{
  //Minimo del servo 1º. Maximo del servo 180º.// 
  //GRADO 1º: BARRERA ABIERTA. //
  //GRADO 110º: BARRERA CERRADA. //

  if(digitalRead(boton)==HIGH)
  {
    estado=false;
    Serial.println("Barrera Cerrada");
    servo1.write(110);
  }
  
  if(digitalRead(boton)==LOW)
  {
     estado=true;
     if(estado==true)
     {
       Serial.println("Barrera Abierta"); 
       servo1.write(1);
       delay(5000);
     } 



      
  }
 
  
  
  
}

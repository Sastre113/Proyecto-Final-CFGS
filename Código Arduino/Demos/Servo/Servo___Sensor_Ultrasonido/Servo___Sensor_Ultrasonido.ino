#include <Servo.h>

Servo servo1;


bool detector=false;
int i;
long tiempo;
long distancia;

void setup() 
{
  Serial.begin(9600);
  servo1.attach(9); 
  servo1.write(110);
  pinMode(25,OUTPUT);
  pinMode(24,INPUT);
}

void loop() 
{
  digitalWrite(25,LOW);
  delayMicroseconds(5);
  digitalWrite(25,HIGH);
  delayMicroseconds(10);
  tiempo=pulseIn(24,HIGH); 
  distancia=int(0.017*tiempo);
  Serial.println("Distancia ");
  Serial.println(distancia);
  Serial.println(" cm");
  delay(2000);
  
  //Minimo del servo 1º. Maximo del servo 180º.// 
  //GRADO 1: BARRERA ABIERTA. //
  //GRADO 110: BARRERA CERRADA. //
  
  if(distancia<=10)
  { 
     servo1.write(1);
  }
  
  if(distancia>10)
  {
    servo1.write(110);
  }
  
}

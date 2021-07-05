int alarma1=44;
int alarma2=45;

unsigned long encender=0;
unsigned long apagar=0;

unsigned long valor_encender=200;
unsigned long valor_apagar=400;
void initempo(unsigned long *crono,unsigned long Time)
{
   *crono=millis()+Time;
}

unsigned long chktempo(unsigned long *crono)
{
  if(*crono>millis())
  {
    return (*crono-millis());
  }
  else
  {
    return 0;
  }
}  
void alarma()
{ 
/***********************************************************************************************/
// - Comprobar si funciona correctamente los 2 zumbadores
/***********************************************************************************************/   
     Serial.println(millis());    
                         
     if((chktempo(&encender))<=0)
     {
       analogWrite(alarma1,20);
      
       initempo(&encender,valor_encender);
     }
          
     if(chktempo(&apagar)<=0)
     {
       analogWrite(alarma1,0);   
       initempo(&apagar,valor_apagar);
     } 
}

void setup() 
{
  Serial.begin(9600);
  pinMode(alarma1,OUTPUT);
  pinMode(alarma2,OUTPUT);

  // LINEAS DE TEST
  analogWrite(alarma1,20);
  delay(50);
  analogWrite(alarma1,0);
  delay(50);
  analogWrite(alarma1,20);
  delay(50);
  analogWrite(alarma1,0);
}

void loop() 
{
  alarma();

}

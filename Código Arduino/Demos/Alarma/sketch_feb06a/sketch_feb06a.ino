void beep(unsigned char pausa)
{
     analogWrite(7, 20);
     delay(pausa);                 // Espera
     analogWrite(7, 0);            // Apaga
     delay(pausa);                 // Espera
}

void setup()
{
   pinMode(7, OUTPUT);
    beep(50);
    beep(50);
    beep(50);
    delay(1000);
}
void loop()
{    
  beep(200);
}

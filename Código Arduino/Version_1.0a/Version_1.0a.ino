//  Version 1.0a //

/**********************************/
/*    Librerias                   */
/**********************************/

#include <DHT.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

/**********************************/
/*    Estructura para las plazas  */
/**********************************/

typedef struct
{
  /*********Datos del programa*******************/

  /*Sensor Ultrasonido*/
  int trigger;
  int echo;
  int IR;

  /*Variable para la funcion temporizador*/
  unsigned long temporizador;

  /*LED de ocupacion
   * Rojo: Ocupado
   * Verde: Libre
  */
 
  int LED_ROJO;
  int LED_VERDE;

  /*Variables para la funcion control_plaza
   * Ocupada --> Indicara si los sensores han marcado una plaza como ocupada o libre.
   * Ciclo --> Variable para controlar cuando empieza la ocupacion de una plaza y cuando termina.
   * Activador --> Sin uso.
  */
  bool ocupada;
  bool ciclo;
  bool activador;

  /* Mediciones realizadas por los sensores de temparatura
   *  Temp = Temperatura 
   *  Hum  = Humedad
  */
  int temp1;
  int temp2;
  int hum1;
  int hum2;
  
  

  /*********Datos para el usuario****************/

  int temp_media;
  int hum_media;

  
  /*********Datos de usuarios****************/

  unsigned long int tiempo_inicio;
  unsigned long int tiempo_final;
  unsigned long int tiempo_ocupacion;
  bool dentro;
  
} datos;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**********************************/
/*    Variables globales          */
/**********************************/
#define CANT_PLAZAS 3
#define DHTTYPE 11
#define RST_PIN  5    //Pin 5 para el reset del RC522
#define SS_PIN  53   //Pin 53 para el SS (SDA) del RC522
#define LED_ACCESO 48
#define LED_NEGACION 49

MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522

 /*********Variables para RFID****************/

/*
 * Crear una estructura para manejar ilimitados usuarios.
 */
byte ActualUID[4]; //Almacenará el código del Tag leído
byte Usuario1[4] = {0x73, 0x20, 0x7B, 0x25} ; //Código Tarjeta Blanca
byte Usuario2[4] = {0xE6, 0x2B, 0xEE, 0x30} ; //Código Llavero Azul
byte Usuario3[4] = {}; // Espacio libre para un usuario mas 

 /*********Variables de la estructura de datos****************/
 
datos plaza[CANT_PLAZAS];
datos usuario[CANT_PLAZAS];
unsigned long tiempo = millis();
unsigned long TIEMPO_SENSOR = 0;

 /*********Variables Zumbadores****************/

int alarma1;
int alarma2;
  
unsigned long encender = 0;
unsigned long apagar = 0;
unsigned long valor_encender = 200;
unsigned long valor_apagar = 400;


 /*********Variables de temporizador LCD****************/
unsigned long lcd0 = 0;
unsigned long lcd1 = 3000;
unsigned long lcd2 = 6000;
unsigned long lcd3 = 9000;

 /*********Pines RELE****************/
int rele1=36; // Iluminacion
int rele2=37; // Ventilacion
int rele3=38; // Luz de emergencias
int rele4=39; // Sin uso

  /*********************************/
  /*    Variables de control       */
  /*     mediante interfaz         */
  /*********************************/
 
bool iluminacion=false;  // Variable de activacion
bool ventilacion=false;  // Variable de activacion
bool alert=false;        // Variable de activacion de alarma por interfaz
bool gate=false;         // Variable de activacon barrera
bool aux=false;          // Variable de activacion luces de emergencia

  /*********************************/
  /*    Variables de control       */
  /*     internas                  */
  /*********************************/
  
bool temp=false;        // Variable que controla la manera que funciona la funcion alarma
bool nuevo_plaza=false; // Variable de activacion. Para repintar LCD cuando se detecten nuevos datos.
bool alarm=false;       // Variable de activacion para los sensores de temperatura

bool acceso=false;      // Variable de acceso positivo
bool no_acceso=false;   // Variable de acceso negativo

unsigned long t_acceso=0;
unsigned long ti_acceso=0;
unsigned long t_noacceso=0;
unsigned long ti_noacceso=0;

bool pause=false;


int c = 0;
DHT dht1(34, DHTTYPE);
DHT dht2(35, DHTTYPE);
LiquidCrystal lcd(42, 43, 44, 45, 46, 47);
Servo SERVO;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**********************************/
/*         Funciones              */
/**********************************/


/*****************Funcion Temporizador*****************/

void initempo(unsigned long *crono, unsigned long Time)
{
  *crono = millis() + Time;
}

unsigned long chktempo(unsigned long *crono)
{
  if (*crono > millis())
  {
    return (*crono - millis());
  }
  else
  {
    return 0;
  }
}

/*****************Funcion Plazas*****************/

void control_plazas(bool &nuevo_plaza)
{
  // La variable i, indica la plaza correspondiente en cada momento.
  int i;
  if (chktempo(&plaza[0].temporizador) <= 0)
  {
    for (i = 0; i < CANT_PLAZAS; i++)
    {
    //  Serial.print("Plaza: ");
    //  Serial.print(i);
    //  Serial.print("\n");
      plaza[i].ocupada = sensor_plaza(i);
      if ((plaza[i].ocupada == false) && (plaza[i].ciclo == false))
      {
        digitalWrite(plaza[i].LED_ROJO, LOW);
        digitalWrite(plaza[i].LED_VERDE, HIGH);
      }
      
      if ((plaza[i].ocupada == true) && (plaza[i].ciclo == false))
      {
        // Se inicia el contador de tiempo para la plaza correspondiente
        plaza[i].tiempo_inicio = millis();
        // Se enciende el LED ROJO para seÃƒÂ±alizar plaza ocupada
        digitalWrite(plaza[i].LED_ROJO, HIGH);
        digitalWrite(plaza[i].LED_VERDE, LOW);
        // Se activa la variable de identificacion para el contador
        plaza[i].ciclo = true;
        nuevo_plaza=true;
      }

      if ((plaza[i].ocupada == true) && (plaza[i].ciclo == true))
      {
        // Se idica el tiempo de ocupacion de la plaza en ms.
        plaza[i].tiempo_final = millis() - plaza[i].tiempo_inicio;
      }

      if ((plaza[i].ocupada == false) && (plaza[i].ciclo == true))
      {
        plaza[i].ciclo = false;
    
        plaza[i].tiempo_final = millis() - plaza[i].tiempo_inicio;
        //segundos

        // Se enciende el LED VERDE para seÃƒÂ±alizar plaza libre
        digitalWrite(plaza[i].LED_ROJO, HIGH);
        digitalWrite(plaza[i].LED_VERDE, LOW);
      }
    }
    initempo(&plaza[0].temporizador, 2000);
  }
}

/*****************Funcion Sensores*****************/

bool sensor_plaza(int i)
{
  float tiempo = 0, distancia = 0, estado = 0;

  /***********************************************************************************************/
  // - Falta definir los parametros de "distancia" y "estado" para la entrega de datos de la funcion
  // mediante "true" o "false".
  /***********************************************************************************************/

  do {
    digitalWrite(plaza[i].trigger, LOW); // Por cuestion de estabilizacion del sensor
    delayMicroseconds(5);
    digitalWrite(plaza[i].trigger, HIGH); // Se realiza un pulso
    delayMicroseconds(10);
    tiempo = pulseIn(plaza[i].echo, HIGH);
    /* FunciÃƒÂ³n para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envÃƒÂ­o
      del pulso ultrasÃƒÂ³nico y cuando el sensor recibe el rebote, es decir: desde que el pin "plaza[i].trigger"
      empieza a recibir el rebote, HIGH, hasta que deja de hacerlo, LOW, la longitud del pulso entrante*/
    distancia = int(0.017 * tiempo);
    /*fÃƒÂ³rmula para calcular la distancia obteniendo un valor entero*/
  } while (distancia <= 0);

  estado = analogRead(plaza[i].IR);


  //////// LINEAS TEST , MONITOR SERIE ////////
/*
  if (distancia != 0)
  {
    Serial.print("tiempo: ");
    Serial.print(tiempo);
    Serial.print("\n");
    Serial.print("cm: ");
    Serial.print(distancia);
    Serial.print("\n");
    Serial.print("IR: ");
    Serial.print(estado);
    Serial.print("\n");
  }
  /*  Aqui finaliza la funcion, devolviendo "true" o "false".
   *   
   *  Para que se cumpla la condicion de "true", los 2 sensores deben estar de acuerdo,segun nuestro criterio
   *  
  */

  if (((distancia < 14)||(distancia > 18)) && (estado < 800))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/*****************Funcion Temperatura*****************/

void control_temperatura()
{
  if (chktempo(&usuario[0].temporizador) <= 0)
  {
    usuario[0].hum1 = dht1.readHumidity();
    usuario[0].temp1 = dht1.readTemperature();

    usuario[0].hum2 = dht2.readHumidity();
    usuario[0].temp2 = dht2.readTemperature();

    usuario[0].temp_media = ((usuario[0].temp1 + usuario[0].temp2) / 2);
    usuario[0].hum_media = ((usuario[0].hum1 + usuario[0].hum2) / 2);

    initempo(&usuario[0].temporizador, 3000);
      //////// LINEAS TEST , MONITOR SERIE ////////
    /*  Serial.print("humedad: ");
      Serial.print(usuario[0].hum_media);
      Serial.print("\n");
      Serial.print("temperatura: ");
      Serial.print(usuario[0].temp_media);
      Serial.print("\n");
    */
  }

  if ((usuario[0].temp_media >= 40)&&(alarm==false))
  {
    //ALARMA DE INCENDIOS Y VENTILACION
    temp=true;
    alarma();
    ventilacion=true;
    control_ventilacion();
  }
  if ((usuario[0].temp_media <= 40)&&(alarm==true))
  {
   temp=false;
   alarma();
   ventilacion=false;
   control_ventilacion();
  }
 temp=false;
}

/*****************Funcion Alarma*****************/

void alarma()
{
  /*
   * Alarma1 , sera el zumbador encargado de simular una alarma
   * Alarma2 , sera el zumbador encargado de simular un pitido de negacion de acceso
   * para el modulo RFID
   */
   //////////    Valores iniciales    ///////////
  ////////// ENCENDER=200 |-| APAGAR=400 /////////
 if((temp==true)||(alert==true))
 {
  if ((chktempo(&encender)) <= 0)
  {
    analogWrite(alarma1, 20);
    //analogWrite(alarma2, 20);
    initempo(&encender, valor_encender);
  }

  if (chktempo(&apagar) <= 0)
  {
    analogWrite(alarma1, 0);
    //analogWrite(alarma2, 0);
    initempo(&apagar, valor_apagar);
  }
 }

 if(alert==false) analogWrite(alarma1, 0);
 
 /*
 Serial.print("\n");
 Serial.print("\n");
 Serial.print(no_acceso);
 Serial.print("\n");
 Serial.print("\n");
 */
}
/*****************Funcion LCD*****************/

void control_lcd()
{
  int i;
  // Informacion sobre plazas //
 if(pause==false)
 {
  if (((chktempo(&lcd1)) <= 0)||(nuevo_plaza==true)) // variable con or para actualizar
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Plazas");
    lcd.setCursor(0, 1);
    for (i = 0; i < CANT_PLAZAS; i++)
    {
      if (plaza[i].ocupada == false)
      {
        lcd.print("P");
        lcd.print(i + 1);
        lcd.print(":L ");
      }

      if (plaza[i].ocupada == true)
      {
        lcd.print("P");
        lcd.print(i + 1);
        lcd.print(":O ");
      }
    }
    initempo(&lcd1, 3000);
    nuevo_plaza==false;
  }

  // Informacion sobre temperatura y humedad //
  if ((chktempo(&lcd2)) <= 0)
  {
    // Datos de temperatura y humedad
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperatura Parking");
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print(usuario[0].temp_media);
    lcd.print("C");
    lcd.print(" Hum:");
    lcd.print(usuario[0].hum_media);
    lcd.print("%");
    initempo(&lcd2, 6000);
  }
 }
}

/*****************Funcion Iluminacion*****************/
void control_iluminacion()
{
  if(iluminacion==false)
  {
   digitalWrite(rele1,HIGH);
  }

  if(iluminacion==true)
  {
   digitalWrite(rele1,LOW);  // Cierra circuito, NORMALMENTE ABIERTO
  }
}


/*****************Funcion Ventilacion*****************/
void control_ventilacion()
{
  if(ventilacion==false)
  {
   digitalWrite(rele2,HIGH);
  }

  if(ventilacion==true)
  {
   digitalWrite(rele2,LOW);  // Cierra circuito, NORMALMENTE ABIERTO
  }
}

/*****************Funcion Luz de Emergencia*****************/
void control_aux()
{
  if(aux==false)
  {
   digitalWrite(rele3,HIGH);
  }

  if(aux==true)
  {
   digitalWrite(rele3,LOW);  // Cierra circuito, NORMALMENTE ABIERTO
  }
}
/*****************Funcion Acceso al parking*****************/

void control_acceso()
{
  bool i=1;
   // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent())
  {
         //Seleccionamos una tarjeta
        if ( mfrc522.PICC_ReadCardSerial())
        {
         // Enviamos serialemente su UID
         // Serial.print(F("Card UID:"));
          for (byte i = 0; i < mfrc522.uid.size; i++) 
          {
           //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
           //Serial.print(mfrc522.uid.uidByte[i], HEX);
           ActualUID[i] = mfrc522.uid.uidByte[i];
          }
      
          //Serial.print("     ");

//      LED_ACCESO 48
//      LED_NEGACION 49
      
          //comparamos los UID para determinar si es uno de nuestros usuarios
          if ((compareArray(ActualUID, Usuario1))||(compareArray(ActualUID, Usuario2)))
          {
           //INTRODUCIR TEMPORIZADOR PARA APAGAR LA LUZ
           //LED DE CONFIRMACION DE ACCESO.
           digitalWrite(LED_ACCESO,HIGH);
           digitalWrite(LED_NEGACION,LOW); 
           acceso=true;
           pause=true;
          
           t_acceso=millis();
           ti_acceso=t_acceso+10000;     
          // Serial.println("Acceso concedido...");
         
            // Usuario entra en el parking //
            if(((compareArray(ActualUID, Usuario1))&&(usuario[0].dentro==false))&&(i==true))
            {
              usuario[0].dentro=true;
              i=0;
            }
            if(((compareArray(ActualUID, Usuario2))&&(usuario[1].dentro==false))&&(i==1))
            {
              usuario[1].dentro=true;
              i=0;
            }
            if(((compareArray(ActualUID, Usuario3))&&(usuario[2].dentro==false))&&(i==1))
            {
              usuario[2].dentro=true;
               i=0;
            }
               // Usuario entra en el parking //
            if(((compareArray(ActualUID, Usuario1))&&(usuario[0].dentro==true))&&(i==1))
            {
              usuario[0].dentro=false;
             i=0;
            }
            if(((compareArray(ActualUID, Usuario2))&&(usuario[1].dentro==true))&&(i==1))
            {
              usuario[1].dentro=false;
              i=0;
            }
            if(((compareArray(ActualUID, Usuario3))&&(usuario[2].dentro==true))&&(i==1))
            {
              usuario[2].dentro=false;
              i=0;
            }    
          }
          else
          {  
           no_acceso=true; 
           t_noacceso=millis();
           ti_noacceso=t_noacceso+2000;
          // Serial.println("Acceso denegado...");
          }
        // Terminamos la lectura de la tarjeta tarjeta  actual

        
        ////////// LINEAS DE TEST //////////
        ////////////////////////////////////
     /*   Serial.println("He llegado aqui, Valor=");
        Serial.println(no_acceso);*/
        ////////////////////////////////////

        mfrc522.PICC_HaltA();
        
    }
  }

}

/*****************Función para comparar dos vectores*****************/

boolean compareArray(byte array1[], byte array2[])
{
  if (array1[0] != array2[0])return (false);
  if (array1[1] != array2[1])return (false);
  if (array1[2] != array2[2])return (false);
  if (array1[3] != array2[3])return (false);
  return (true);
}

/*****************Función para elevar/bajar barrera y control de LED's de acceso*****************/
void control_barrera()
{
   //Minimo del servo 1º. Maximo del servo 180º.// 
  //GRADO 1: BARRERA ABIERTA. //
  //GRADO 110: BARRERA CERRADA. //
  
  if(((acceso==true)&&(compareArray(ActualUID, Usuario1))))
  {
    if(usuario[0].dentro==true)
    {
      SERVO.write(10); 
      gate=true; 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Bienvenido");
      lcd.setCursor(0, 1);
      lcd.print("    Usuario1");
      
    }
    if(usuario[0].dentro==false)
    {
      SERVO.write(10);
      gate=true;  
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Barrera  Abierta");
      lcd.setCursor(0, 1);
      lcd.print(" Adios Usuario1");
     
    }
  }

  if(((acceso==true)&&(compareArray(ActualUID, Usuario2))))
  {
    if(usuario[1].dentro==true)
    {
      SERVO.write(10);
      gate=true;  
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   Bienvenido");
      lcd.setCursor(0, 1);
      lcd.print("    Usuario2");
    }
    if(usuario[1].dentro==false)
    {
      SERVO.write(10);
      gate=true;  
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Barrera  Abierta");
      lcd.setCursor(0, 1);
      lcd.print(" Adios Usuario2");
    }
  }

  
  if((t_acceso>=ti_acceso)&&(acceso==true))
  {
       SERVO.write(100);
       pause=false;
       acceso=false;
       gate=false;
       digitalWrite(LED_ACCESO,LOW);
       digitalWrite(LED_NEGACION,HIGH);
  }
  if(no_acceso==true)
  {
    
    analogWrite(alarma2, 20);
    pause=true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acceso denegado");
  }
  if((t_noacceso>=ti_noacceso)&&(no_acceso==true))
  {
    analogWrite(alarma2,0);
    pause=false;
    no_acceso=false;
  }
}

void control_interfaz()
{
  int x;
  if(Serial.available())
  {
    char c=Serial.read();
    // Iluminación //
    if(c=='0'){iluminacion=false;}
    if(c=='1'){iluminacion=true; }   
    // Ventilación //
    if(c=='2'){ventilacion=false;}
    if(c=='3'){ventilacion=true;}   
    // Alarma //
    if(c=='4'){alert=false;}
    if(c=='5'){ alert=true;}
    // Iluminacion de emergencia //
    if(c=='6'){aux=false;}
    if(c=='7'){aux=true;}

    // Temperatura //
    if(c=='t')
    {
      Serial.write("t=");
      Serial.print(usuario[0].temp_media);
    }
    // Humedad //
    if(c=='h')
    {
      Serial.write("h=");
      Serial.print(usuario[0].hum_media);
    }
    // Barrera //
    if(c=='b')
    {
      Serial.write("b=");
      if(acceso==true)
      {
        Serial.write("Abierta");
        if((gate==true)&&(usuario[0].dentro==true))Serial.write("11");
        if((gate==true)&&(usuario[0].dentro==false))Serial.write("10");

        if((gate==true)&&(usuario[1].dentro==true))Serial.write("21");
        if((gate==true)&&(usuario[1].dentro==false))Serial.write("20");

        if((gate==true)&&(usuario[2].dentro==true))Serial.write("31");
        if((gate==true)&&(usuario[2].dentro==false))Serial.write("30");

      }
      if(acceso==false)
      {
        Serial.write("Cerrada");
      }
    }
    // Usuario //
    if(c=='u')
    {
      Serial.write("u=");
      for(x=0;x<CANT_PLAZAS;x++)
      {
        if(usuario[x].dentro==false)Serial.print("0");
        if(usuario[x].dentro==true)Serial.print("1");
      }
      
    }
    // Plaza //
    if(c=='p')
    {
      Serial.write("p=");
      for(x=0;x<CANT_PLAZAS;x++ )
      {
         if(plaza[x].ocupada==false)Serial.print("0");
         if(plaza[x].ocupada==true)Serial.print("1");
      }
    }
    // Ocupacion Plaza
    if(c=='m')
    {
      
      if(plaza[0].ocupada==true)
      {
        Serial.print("p1=");
        Serial.print((plaza[0].tiempo_final/1000)/60); 
      }
      else
      {
        Serial.print("Li1");
      }  
    }
    if(c=='n')
    {
      if(plaza[1].ocupada==true)
      {
        Serial.print("p2=");
        Serial.print((plaza[1].tiempo_final/1000)/60); 
      }
      else
      {
        Serial.print("Li2");
      }
    }
    if(c=='g')
    {
      if(plaza[2].ocupada==true)
      {
        Serial.print("p3=");
        Serial.print((plaza[2].tiempo_final/1000)/60); 
      }
      else
      {
        Serial.print("Li3");
      }
    }

    // Sincronizacion //
    if(c=='s')
    {
      if(iluminacion==false)Serial.print(0);
      if(iluminacion==true)Serial.print(1);
      if(ventilacion==false)Serial.print(0);
      if(ventilacion==true)Serial.print(1);
      if(alert==false)Serial.print(0);
      if(alert==true)Serial.print(1);
      if(aux==false)Serial.print(0);
      if(aux==true)Serial.print(1);
    }
  }//Serial.available()
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**********************************/
/*         Main                   */
/**********************************/
void setup()
{
  int iniciador;

  Serial.begin(9600);
  Serial.print("Inicializando programa...\n");


  Serial.print("Adjudicando valor a variables\n");

  plaza[0].ocupada = false;
  plaza[0].ciclo = false;
  plaza[0].activador = false;
  plaza[1].ocupada = false;
  plaza[1].ciclo = false;
  plaza[1].activador = false;
  plaza[2].ocupada = false;
  plaza[2].ciclo = false;
  plaza[2].activador = false;

 
  for (iniciador = 0; iniciador < CANT_PLAZAS; iniciador++)
  {
    TIEMPO_SENSOR = TIEMPO_SENSOR + 2000;
    plaza[iniciador].temporizador = TIEMPO_SENSOR;
    TIEMPO_SENSOR = TIEMPO_SENSOR - 1000;
    usuario[iniciador].temporizador = TIEMPO_SENSOR;
    TIEMPO_SENSOR = TIEMPO_SENSOR + 1000;
    usuario[iniciador].dentro=false;
  }

  Serial.print("Adjudicando PIN a las variables de los sensores HC-SR04 \n");
  /*********Pin HC-SR04***************/

  plaza[0].activador = true;
  plaza[0].trigger = 22;
  plaza[0].echo = 23;
  plaza[1].trigger = 24;
  plaza[1].echo = 25;
  plaza[2].trigger = 26;
  plaza[2].echo = 27;


  Serial.print("Adjudicando PIN a las variables de los LED de ocupación \n");
  /*********Pin de LED's**************/

  plaza[0].LED_ROJO = 28;
  plaza[0].LED_VERDE = 29;
  plaza[1].LED_ROJO = 30;
  plaza[1].LED_VERDE = 31;
  plaza[2].LED_ROJO = 32;
  plaza[2].LED_VERDE = 33;

  Serial.print("Adjudicando PIN a las variables de los sensores TCRT5000 \n");
  /*********Pin IR********************/

  plaza[0].IR = A0;
  plaza[1].IR = A1;
  plaza[2].IR = A2;

  Serial.print("Adjudicando PIN a las variables de los zumbadores \n");
  /*********Pin Zumbador**************/

  alarma1 = 12;
  alarma2 = 13;


  Serial.print("Configurando el funcionamiento de los pin's digitales \n");
  /*****************Pines digitales***************/

  /*********************************/
  /*     Sensores ultrasonido      */
  /*              y                */
  /*     LED's de ocupaciÃ³n        */
  /*********************************/

  pinMode(plaza[0].trigger, OUTPUT);
  pinMode(plaza[0].echo, INPUT);
  pinMode(plaza[0].LED_ROJO, OUTPUT);
  pinMode(plaza[0].LED_VERDE, OUTPUT);
  pinMode(plaza[0].LED_ROJO, HIGH);
  pinMode(plaza[0].LED_VERDE, HIGH);
  
  pinMode(plaza[1].trigger, OUTPUT);
  pinMode(plaza[1].echo, INPUT);
  pinMode(plaza[1].LED_ROJO, OUTPUT);
  pinMode(plaza[1].LED_VERDE, OUTPUT);
  pinMode(plaza[1].LED_ROJO, HIGH);
  pinMode(plaza[1].LED_VERDE, HIGH);
  
  pinMode(plaza[2].trigger, OUTPUT);
  pinMode(plaza[2].echo, INPUT);
  pinMode(plaza[2].LED_ROJO, OUTPUT);
  pinMode(plaza[2].LED_VERDE, OUTPUT);
  pinMode(plaza[2].LED_ROJO, HIGH);
  pinMode(plaza[2].LED_VERDE, HIGH);

 
  digitalWrite(plaza[0].LED_ROJO, LOW);
  digitalWrite(plaza[0].LED_VERDE, HIGH);
  digitalWrite(plaza[1].LED_ROJO, LOW);
  digitalWrite(plaza[1].LED_VERDE, HIGH);
  digitalWrite(plaza[2].LED_ROJO, LOW);
  digitalWrite(plaza[2].LED_VERDE, HIGH);
  delay(500);
  digitalWrite(plaza[0].LED_ROJO, HIGH);
  digitalWrite(plaza[0].LED_VERDE, LOW);
  digitalWrite(plaza[1].LED_ROJO, HIGH);
  digitalWrite(plaza[1].LED_VERDE, LOW);
  digitalWrite(plaza[2].LED_ROJO, HIGH);
  digitalWrite(plaza[2].LED_VERDE, LOW);
  delay(500);   
  digitalWrite(plaza[0].LED_ROJO, LOW);
  digitalWrite(plaza[0].LED_VERDE, HIGH);
  digitalWrite(plaza[1].LED_ROJO, LOW);
  digitalWrite(plaza[1].LED_VERDE, HIGH);
  digitalWrite(plaza[2].LED_ROJO, LOW);
  digitalWrite(plaza[2].LED_VERDE, HIGH);
  
  Serial.print("Encendido de Leds\n");
  /*********************************/
  /*  InicializaciÃƒÂ³n y testeo      */
  /*     de los zumbadores         */
  /*********************************/

  pinMode(alarma1, OUTPUT);
  pinMode(alarma2, OUTPUT); 

  ////////// LINEAS DE TEST //////////
  ////////////////////////////////////

  Serial.print("Probando zumbadores...\n");
  analogWrite(alarma1, 20);
  analogWrite(alarma2, 20);
  delay(50);
  analogWrite(alarma1, 0);
  analogWrite(alarma2, 0);
  delay(50);
  analogWrite(alarma1, 20);
  analogWrite(alarma2, 20);
  delay(50);
  analogWrite(alarma1, 0);
  analogWrite(alarma2, 0);

   /*********************************/
  /*               Relé            */
  /*********************************/

  pinMode(rele1,OUTPUT);
  pinMode(rele2,OUTPUT);
  pinMode(rele3,OUTPUT);
  pinMode(rele4,OUTPUT);

  /* Los Reles, 1 y 2, estan configurados como NA:
   *  valor HIGH = CIRCUITO ABIERTO
   *  valor LOW  = CIRCUITO CERRADO
   *  
   * Los Reles, 3 y 4, estan sin usar.
  */
  
  ////////// LINEAS DE TEST //////////
  ////////////////////////////////////
  Serial.print("Probando dispositivos controlados por rele\n");

  digitalWrite(rele1,HIGH);
  digitalWrite(rele2,HIGH);
  digitalWrite(rele3,HIGH);
  digitalWrite(rele4,HIGH);
  delay(500);
  digitalWrite(rele1,LOW);
  digitalWrite(rele2,LOW);
  digitalWrite(rele3,LOW);
  digitalWrite(rele4,LOW); 
  delay(500);
  digitalWrite(rele1,HIGH);
  digitalWrite(rele2,HIGH);
  digitalWrite(rele3,HIGH);
  digitalWrite(rele4,HIGH);
  delay(500);
  digitalWrite(rele1,LOW);
  digitalWrite(rele2,LOW);
  digitalWrite(rele3,LOW);
  digitalWrite(rele4,LOW);
  delay(500);
  digitalWrite(rele1,HIGH);
  digitalWrite(rele2,HIGH);
  digitalWrite(rele3,HIGH);
  digitalWrite(rele4,HIGH); 
  

  ////////////////////////////////////

  Serial.print("Configurando el funcionamiento de los pin's analógicos \n");
  /*****************Pines analogicas*****************/


  /*********************************/
  /*        Sensores IR            */
  /*********************************/

  pinMode(plaza[0].IR, INPUT);
  pinMode(plaza[1].IR, INPUT);
  pinMode(plaza[2].IR, INPUT);


  /*********************************/
  /*  Inicializacion y testeo      */
  /*     de LCD                    */
  /*********************************/

  lcd.begin(16, 2);
  lcd.display();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando LCD");
  lcd.setCursor(0, 1);
  lcd.print("Pantalla de info");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);

   /*********************************/
  /*  Inicializacion y testeo      */
  /*     de RFID                   */
  /*********************************/
  
  Serial.print("Configurando el funcionamiento del modulo RFID \n");
  SPI.begin();        //Iniciamos el Bus SPI
  pinMode(LED_ACCESO,OUTPUT);
  pinMode(LED_NEGACION,OUTPUT);
  digitalWrite(LED_ACCESO,HIGH);
  digitalWrite(LED_NEGACION,HIGH);
  delay(500);
  digitalWrite(LED_ACCESO,LOW);
  digitalWrite(LED_NEGACION,LOW);
  delay(500);
  digitalWrite(LED_ACCESO,HIGH);
  digitalWrite(LED_NEGACION,HIGH);
  delay(500);
  digitalWrite(LED_ACCESO,LOW);
  digitalWrite(LED_NEGACION,HIGH);

  
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  Serial.println("Control de acceso:");

  /*********************************/
  /*  Inicializacion y testeo      */
  /*     de servo                  */
  /*********************************/
  //Minimo del servo 1º. Maximo del servo 180º.// 
  //GRADO 1: BARRERA ABIERTA. //
  //GRADO 110: BARRERA CERRADA. //
  
  SERVO.attach(9);
  SERVO.write(100); 

  acceso=false;
  no_acceso=false;
  
  Serial.println("Programa cargado correctamente.\n");
  Serial.println("Iniciando Loop.\n");
  Serial.flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**********************************/
/*         Loop                   */
/**********************************/

void loop()
{ 
  nuevo_plaza=false; //Variable para actualizar LCD al actualizar datos
  
  control_interfaz();
  control_plazas(nuevo_plaza);
  control_temperatura();
  control_lcd();
  control_iluminacion();
  control_aux();
  alarma();
  control_ventilacion();
  control_acceso(); 
  control_barrera();

  t_acceso=millis();
  t_noacceso=millis();
  Serial.flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////LA ÚLTIMA LÍNEA//////////////////////////////////////////////////

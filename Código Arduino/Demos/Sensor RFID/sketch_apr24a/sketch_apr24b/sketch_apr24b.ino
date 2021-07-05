#include <SPI.h>
#include <MFRC522.h>
MFRC522 rfid(53,5);
void setup()
{
 Serial.begin(9600);
 SPI.begin(); 
 MFRC522.init(); 
 
}
unsigned char het[16]={'H','E','T','P','R','O',' ','R','F','I','D',' ','5','5','2',' '}; //Declaracion de cadena de caracteres
unsigned char *writeData=het; //asigna het a char tipo puntero
unsigned char *str; //char tipo puntero utilizado para la función de lectura de un bloque
 
void loop()
{
 if (rfid.isCard()) {
 rfid.write(8,writeData); //Función escritura (num de bloque, string)
 rfid.read(8,str); //Función lectura (num de bloque, str) guarda lo que está en el bloque a str
 Serial.print(str[0]); //Imprime lo que se guardó en str de la funcion read
 }
 MFRC522.halt();
}

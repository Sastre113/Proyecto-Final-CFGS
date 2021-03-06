//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPort"
#pragma link "CPortCtl"
#pragma resource "*.dfm"
TForm1 *Form1;


bool conexion=false;

bool iluminacion=false;
bool ventilacion=false;
bool alarma=false;
bool emergencia=false;
bool sincro=true;
int ciclo=3;
AnsiString cadena;

bool temp=false;
bool hum=false;
bool usuario1=false;
bool usuario2=false;

/////////////////////////////////////
bool plaza1=false;
bool plaza2=false;
bool plaza3=false;
/*LOS VALORES ESTAN NORMALIZADO PARA "0" Y "1", SIENDO 0=FALSE Y 1=TRUE
EN ARDUINO
*/
//---------------------------------------------------------------------------
void sincronizacion()
{
  Form1->Cursor=crHourGlass;
  ShowMessage("Sincronizando Arduino con Interfaz");
  Form1->ComPort1->WriteStr('s');
  Sleep(1000);
  Form1->ComPort1->ReadStr(cadena,4);


  if(cadena.SubString(1,1)=='0')
  {
    iluminacion=false;
    Form1->Shape1->Brush->Color=clWhite;
    Form1->Shape2->Brush->Color=clRed;
  }
  if(cadena.SubString(1,1)=='1')
  {
    iluminacion=true;
    Form1->Shape1->Brush->Color=clLime;
    Form1->Shape2->Brush->Color=clWhite;
  }
  if(cadena.SubString(2,1)=='0')
  {
    ventilacion=false;
    Form1->Shape3->Brush->Color=clWhite;
    Form1->Shape4->Brush->Color=clRed;
  }
  if(cadena.SubString(2,1)=='1')
  {
    ventilacion=true;
    Form1->Shape3->Brush->Color=clLime;
    Form1->Shape4->Brush->Color=clWhite;
  }
  if(cadena.SubString(3,1)=='0')
  {
    alarma=false;
    Form1->Shape5->Brush->Color=clWhite;
    Form1->Shape6->Brush->Color=clRed;
  }
  if(cadena.SubString(3,1)=='1')
  {
    alarma=true;
    Form1->Shape5->Brush->Color=clLime;
    Form1->Shape6->Brush->Color=clWhite;
  }
  if(cadena.SubString(4,1)=='0')
  {
    emergencia=false;
    Form1->Shape7->Brush->Color=clWhite;
    Form1->Shape8->Brush->Color=clRed;
  }
  if(cadena.SubString(4,1)=='1')
  {
    emergencia=true;
    Form1->Shape7->Brush->Color=clLime;
    Form1->Shape8->Brush->Color=clWhite;
  }

  Form1->ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
  Form1->Memo1->Lines->Add("Bienvenido a la interfaz para gestionar el Parking");
  Form1->Memo1->Lines->Add("Sesion abierta con fecha de "+DateToStr(Date())+" a la "+Time());
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{

   Form1->Memo1->Lines->Add("Sesi?n finalizada "+DateToStr(Date())+" a la "+Time());
   Form1->Memo1->Lines->Add("");
   Form1->Memo1->Lines->SaveToFile("texto.txt");

}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button5Click(TObject *Sender)
{
  if(conexion==false)
  {
 // ComPort1->ShowSetupDialog();
   ComPort1->Open();
   conexion=true;
   Form1->Button5->Caption="Desconectar";
   Form1->Image1->Visible=true;
   Form1->Image2->Visible=false;
   Form1->Memo1->Lines->Add("Conectado al dispositivo Arduino usando el puerto "+ComPort1->Port);
   Form1->Memo1->Lines->Add("Iniciando la funci?n para sincronizar la Interfaz con Arduino");
   Form1->Memo1->Lines->Add("Por favor, tenga paciencia");
   sincronizacion();
    // Plazas
   Form1->Timer1->Enabled=true;
   // Usuarios
   Form1->Timer2->Enabled=true;
   // Barrera
   Form1->Timer4->Enabled=true;
   // Temperatura y humedad
   Form1->Timer6->Enabled=true;
   Form1->Timer7->Enabled=true;
   // Tiempo Plazas
   Form1->Timer8->Enabled=true;
   Form1->Timer9->Enabled=true;
   Form1->Timer10->Enabled=true;
   return;
  }

  if(conexion==true)
  {
   ComPort1->Close();
   conexion=false;
   Form1->Button5->Caption="Conectar";
   Form1->Image1->Visible=false;
   Form1->Image2->Visible=true;
   Form1->Memo1->Lines->Add("Desconectado");
   Form1->Memo1->Lines->Add("");

   // Plazas
   Form1->Timer1->Enabled=false;
   // Usuarios
   Form1->Timer2->Enabled=false;
   // Barrera
   Form1->Timer4->Enabled=false;
   // Temperatura y humedad
   Form1->Timer6->Enabled=false;
   Form1->Timer7->Enabled=false;
   // Tiempo Plazas
   Form1->Timer8->Enabled=false;
   Form1->Timer9->Enabled=false;
   Form1->Timer10->Enabled=false;
   return;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  // ILUMIINACI?N
  // Valores asignados "0" y "1"
  if(iluminacion==false)
  {
   ComPort1->WriteStr('1');
   ComPort1->ClearBuffer(true, true);
   iluminacion=true;
   Form1->Shape1->Brush->Color=clLime;
   Form1->Shape2->Brush->Color=clWhite;
   Form1->Memo1->Lines->Add("// Encendiendo la iluminacion del parking //");
    return;
  }
  if(iluminacion==true)
  {
   ComPort1->WriteStr('0');
   ComPort1->ClearBuffer(true, true);
   Form1->Shape2->Brush->Color=clRed;
   Form1->Shape1->Brush->Color=clWhite;
   Form1->Memo1->Lines->Add("// Apagando la iluminacion del parking //");
   iluminacion=false;
    return;
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
    // VENTILACI?N
  // Valores asignados "2" y "3"
  if(ventilacion==false)
  {
   ComPort1->WriteStr('3');
   ComPort1->ClearBuffer(true, true);
   Form1->Shape3->Brush->Color=clLime;
   Form1->Shape4->Brush->Color=clWhite;
   Form1->Memo1->Lines->Add("// Encendiendo ventilaci?n del parking //");
   ventilacion=true;
   return;
  }
  if(ventilacion==true)
  {
   ComPort1->WriteStr('2');
   ComPort1->ClearBuffer(true, true);
   Form1->Shape4->Brush->Color=clRed;
   Form1->Shape3->Brush->Color=clWhite;
   Form1->Memo1->Lines->Add("// Apagando ventilaci?n del parking //");
    ventilacion=false;
    return;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
       // Alarma
  // Valores asignados "4" y "5"
  if(alarma==false)
  {
   ComPort1->WriteStr('5');
   ComPort1->ClearBuffer(true, true);
   Form1->Shape5->Brush->Color=clLime;
   Form1->Shape6->Brush->Color=clWhite;
   Form1->Memo1->Lines->Add("// Alarma Activada //");
    alarma=true;
    return;
  }
  if(alarma==true)
  {
   ComPort1->WriteStr('4');
   ComPort1->ClearBuffer(true, true);
   Form1->Shape6->Brush->Color=clRed;
   Form1->Shape5->Brush->Color=clWhite;
   Form1->Memo1->Lines->Add("// Alarma Desactivada //");
    alarma=false;
    return;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
       // iluminacion de emergencia
  // Valores asignados "6" y "7"
  if(emergencia==false)
  {
   ComPort1->WriteStr('7');
   ComPort1->ClearBuffer(true, true);
   Form1->Shape7->Brush->Color=clLime;
   Form1->Shape8->Brush->Color=clWhite;
   Form1->Memo1->Lines->Add("// Encendiendo iluminacion de emergencia //");
    emergencia=true;
    return;
  }
  if(emergencia==true)
  {
   ComPort1->WriteStr('6');
   ComPort1->ClearBuffer(true, true);
   Form1->Shape8->Brush->Color=clRed;
   Form1->Shape7->Brush->Color=clWhite;
   Form1->Memo1->Lines->Add("// Apagando iluminacion de emergencia //");
   emergencia=false;
   return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  //Plaza
  ComPort1->WriteStr('p');
  Sleep(100);
  ComPort1->ReadStr(cadena,5);
  Form1->Memo1->Lines->Add("// Actualizando estado de las plazas //");
  if(cadena.SubString(0,1)=='p')
  {
    // Plaza 1 //
    if(cadena.SubString(3,1)=='0')
    {
     Form1->Label27->Color=clLime;
     Form1->Shape9->Brush->Color=clLime;
    }
    if(cadena.SubString(3,1)=='1')
    {
     Form1->Label27->Color=clRed;
     Form1->Shape9->Brush->Color=clRed;
    }
    // Plaza 2 //
    if(cadena.SubString(4,1)=='0')
    {
     Form1->Label28->Color=clLime;
     Form1->Shape10->Brush->Color=clLime;
    }
    if(cadena.SubString(4,1)=='1')
    {
     Form1->Label28->Color=clRed;
     Form1->Shape10->Brush->Color=clRed;
    }
    // Plaza 3 //
    if(cadena.SubString(5,1)=='0')
    {
     Form1->Label29->Color=clLime;
     Form1->Shape11->Brush->Color=clLime;
    }
    if(cadena.SubString(5,1)=='1')
    {
     Form1->Label29->Color=clRed;
     Form1->Shape11->Brush->Color=clRed;
    }

  }
  ComPort1->ClearBuffer(true, true);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
  // Usuarios
  ComPort1->WriteStr('u');
  Sleep(100);
  ComPort1->ReadStr(cadena,4);
  Form1->Memo1->Lines->Add("// Actualizando estado de los usuarios //");
  if(cadena.SubString(0,1)=='u')
  {
     // USUARIO 1
     if(cadena.SubString(3,1)=='1')
     {
       Form1->Label34->Caption="En el parking";
       usuario1=true;
     }
     if(cadena.SubString(3,1)=='0')
     {
       Form1->Label34->Caption="Fuera del parking";
       usuario1=false;
     }
     // USUARIO 2
     if(cadena.SubString(4,1)=='1')
     {
       Form1->Label35->Caption="En el parking";
       usuario2=true;
     }
     if(cadena.SubString(4,1)=='0')
     {
       Form1->Label35->Caption="Fuera del parking";
       usuario2=false;
     }
     // USUARIO 3

     Form1->Label36->Caption="Usuario Libre";
     /*
     if(cadena.SubString(5,1)=='1')
     {
       Form1->Label36->Caption="En el parking";
       usuario2=true;
     }
     if(cadena.SubString(5,1)=='0')
     {
       Form1->Label36->Caption="Fuera del parking";
       usuario2=false;
     }
     */
  }

  ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer4Timer(TObject *Sender)
{
  // Barrera
  ComPort1->WriteStr('b');
  Sleep(100);
  ComPort1->ReadStr(cadena,10);
  Form1->Memo1->Lines->Add("// Actualizando estado de la barrera //");
  if(cadena.SubString(0,1)=='b')
  {
    Form1->Label19->Caption=cadena.SubString(3,7);
    if(cadena.SubString(11,2)=="11")
    {
      Form1->Memo1->Lines->Add("Usuario 1 est? accediendo al parking");
      Form1->Memo1->Lines->Add("ID: 73207B25");
      usuario1=true;
    }
    if(cadena.SubString(11,2)=="10")
    {
      Form1->Memo1->Lines->Add("Usuario 1 est? abandonando el parking");
      Form1->Memo1->Lines->Add("ID: 73207B25");
      usuario1=false;
    }
    if(cadena.SubString(11,2)=="21")
    {
      Form1->Memo1->Lines->Add("Usuario 2 est? accediendo al parking");
      Form1->Memo1->Lines->Add("ID: E62BEE30");
      usuario2=true;
    }
    if(cadena.SubString(11,2)=="20")
    {
      Form1->Memo1->Lines->Add("Usuario 2 est? abandonando el parking");
      Form1->Memo1->Lines->Add("ID: E62BEE30");
      usuario2=false;
    }



  }
  ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Timer6Timer(TObject *Sender)
{
   // Temperatura
   ComPort1->WriteStr('t');
   Sleep(100);
   ComPort1->ReadStr(cadena,4);
   Form1->Memo1->Lines->Add("// Actualizando temperatura del parking //");
   if(cadena.SubString(0,1)=='t')
   {
     Form1->Label16->Caption=cadena.SubString(3,2)+" ?C";
   }
   ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer7Timer(TObject *Sender)
{
    // Humedad
   ComPort1->WriteStr('h');
   Sleep(100);
   ComPort1->ReadStr(cadena,4);
   Form1->Memo1->Lines->Add("// Actualizando humedad del parking //");
   if(cadena.SubString(0,1)=='h')
   {
     Form1->Label17->Caption=cadena.SubString(3,2)+" %";
   }
   ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer8Timer(TObject *Sender)
{
   // Plaza 1
   ComPort1->WriteStr('m');
   Sleep(100);
   ComPort1->ReadStr(cadena,6);
   if(cadena.SubString(0,2)=="p1")
   {
     Form1->Label21->Caption=cadena.SubString(4,4)+" minutos";
   }
   if(cadena.SubString(0,3)=="Li1")
   {
      Form1->Label21->Caption="Libre";
      Form1->Memo1->Lines->Add("1? plaza libre");
   }
   ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer9Timer(TObject *Sender)
{
   // Plaza 2
   ComPort1->WriteStr('n');
   Sleep(100);
   ComPort1->ReadStr(cadena,6);
   if(cadena.SubString(0,2)=="p2")
   {
     Form1->Label22->Caption=cadena.SubString(4,4)+" minutos";
   }
   if(cadena.SubString(0,3)=="Li2")
   {
      Form1->Label22->Caption="Libre";
      Form1->Memo1->Lines->Add("2? plaza libre");
   }
   ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer10Timer(TObject *Sender)
{
    // Plaza 3
   ComPort1->WriteStr('g');
   Sleep(100);
   ComPort1->ReadStr(cadena,6);
   if(cadena.SubString(0,2)=="p3")
   {
     Form1->Label23->Caption=cadena.SubString(4,4)+" minutos";
   }
   if(cadena.SubString(0,3)=="Li3")
   {
      Form1->Label23->Caption="Libre";
      Form1->Memo1->Lines->Add("3? plaza libre");
   }
   ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------





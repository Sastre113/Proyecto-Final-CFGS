//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPort"
#pragma resource "*.dfm"
TForm1 *Form1;

char buffer[1000];
AnsiString leer;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
 ComPort1->ShowSetupDialog();
 ComPort1->Open();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
 ComPort1->WriteStr("1");
 Shape1->Brush->Color=clRed;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
 ComPort1->WriteStr("0");
 Shape1->Brush->Color=clWhite;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label1Click(TObject *Sender)
{
 leer=ComPort1->ReadStr(buffer,3);
 Label1->Caption=leer;

}
//---------------------------------------------------------------------------

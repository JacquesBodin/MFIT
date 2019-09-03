//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WrkDirError_MessageBox.h"
#include "WrkDir_Dialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWDerrorMessage *WDerrorMessage;
//---------------------------------------------------------------------------
__fastcall TWDerrorMessage::TWDerrorMessage(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TWDerrorMessage::Ok_ButtonClick(TObject *Sender)
{
  Hide();
  WrkDir_Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TWDerrorMessage::Cancel_ButtonClick(TObject *Sender)
{
  Hide();
}
//---------------------------------------------------------------------------

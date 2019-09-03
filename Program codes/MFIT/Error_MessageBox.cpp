//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Error_MessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TErrorMessage *ErrorMessage;
//---------------------------------------------------------------------------
__fastcall TErrorMessage::TErrorMessage(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TErrorMessage::showMessage(std::string message)
{
  ErrorMessageLabel->Caption = message.c_str();
  Show();
}
//---------------------------------------------------------------------------
void __fastcall TErrorMessage::OkButtonClick(TObject *Sender)
{
  Hide();
}
//---------------------------------------------------------------------------


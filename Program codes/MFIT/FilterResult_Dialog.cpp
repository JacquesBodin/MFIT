//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FilterResult_Dialog.h"
#include "TCobs_dialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFilterResult_Form *FilterResult_Form;
//---------------------------------------------------------------------------
__fastcall TFilterResult_Form::TFilterResult_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFilterResult_Form::Button1Click(TObject *Sender)
{
  TCobs_Form->ProgressBar->Position = 0;
  TCobs_Form->Caption = "Observations";
  Hide();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PestcheckOutput_MessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPestcheckOutputMessage *PestcheckOutputMessage;
//---------------------------------------------------------------------------
__fastcall TPestcheckOutputMessage::TPestcheckOutputMessage(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

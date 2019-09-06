//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EndInvUserChnls_Dialog.h"
#include "MainForm.h"
#include "MDMi_Dialog.h"
#include "MDMed_Dialog.h"
#include "MDP_SFDM_Dialog.h"
#include "MDP_2RNE_Dialog.h"

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEndInversionUserChnls_Form *EndInversionUserChnls_Form;
//---------------------------------------------------------------------------
__fastcall TEndInversionUserChnls_Form::TEndInversionUserChnls_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEndInversionUserChnls_Form::Cancel_ButtonClick(TObject *Sender)
{
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TEndInversionUserChnls_Form::OK_ButtonClick(TObject *Sender)
{
  Hide();
  string line, paramName;
  double paramValue, scale, offset;
  char idParam;
  int idChnl;
  ifstream optParFile ("MFIT.par");
  if (optParFile)
  {
	if (Main_Form->MDMi_menu->Checked) // ADE_Inst & User Channels
	{
	  getline(optParFile, line);
	  while (getline(optParFile, line))
	  {
		istringstream (line) >> paramName >> paramValue >> scale >> offset;
		if (paramName == "qt") MDMi->QT_Edit->Text = paramValue;
		else
		{
		  idParam = paramName[0];
		  istringstream(paramName.substr(1,paramName.size()-1)) >> idChnl;
		  if (idParam == 'm') MDMi->ChnlParamGrid->Cells[1][idChnl] = paramValue;
		  else if (idParam == 'h') MDMi->ChnlParamGrid->Cells[2][idChnl] = paramValue;
		  else if (idParam == 's') MDMi->ChnlParamGrid->Cells[3][idChnl] = paramValue;
		}
	  }
	  MDMi->Show();
	}
	else if (Main_Form->MDMed_menu->Checked) // ADE NonInst & User Channels
	{
	  getline(optParFile, line);
	  while (getline(optParFile, line))
	  {
		istringstream (line) >> paramName >> paramValue >> scale >> offset;
		if (paramName == "c0") MDMed->c0_Edit->Text = paramValue;
		else
		{
		  idParam = paramName[0];
		  istringstream(paramName.substr(1,paramName.size()-1)) >> idChnl;
		  if (idParam == 'f') MDMed->ChnlParamGrid->Cells[1][idChnl] = paramValue;
		  else if (idParam == 'h') MDMed->ChnlParamGrid->Cells[2][idChnl] = paramValue;
		  else if (idParam == 's') MDMed->ChnlParamGrid->Cells[3][idChnl] = paramValue;
		  else if (idParam == 'g') MDMed->ChnlParamGrid->Cells[4][idChnl] = paramValue;
		}
	  }
	  MDMed->Show();
	}
	else if (Main_Form->MDP_SFDM_menu->Checked) // SFDM & User Channels
	{
	  getline(optParFile, line);
	  while (getline(optParFile, line))
	  {
		istringstream (line) >> paramName >> paramValue >> scale >> offset;
		if (paramName == "qt") MDP_SFDM->QT_Edit->Text = paramValue;
		else
		{
		  idParam = paramName[0];
		  istringstream(paramName.substr(1,paramName.size()-1)) >> idChnl;
		  if (idParam == 'm') MDP_SFDM->ChnlParamGrid->Cells[1][idChnl] = paramValue;
		  else if (idParam == 'b') MDP_SFDM->ChnlParamGrid->Cells[2][idChnl] = paramValue;
		  else if (idParam == 'h') MDP_SFDM->ChnlParamGrid->Cells[3][idChnl] = paramValue;
		  else if (idParam == 's') MDP_SFDM->ChnlParamGrid->Cells[4][idChnl] = paramValue;
		}
	  }
	  MDP_SFDM->Show();
	}
	else if (Main_Form->MDP_2RNE_menu->Checked) // 2RNE & User Channels
	{
	  getline(optParFile, line);
	  while (getline(optParFile, line))
	  {
		istringstream (line) >> paramName >> paramValue >> scale >> offset;
		if (paramName == "qt") MDP_2RNE->QT_Edit->Text = paramValue;
		else
		{
		  idParam = paramName[0];
		  istringstream(paramName.substr(1,paramName.size()-1)) >> idChnl;
		  if (idParam == 'm') MDP_2RNE->ChnlParamGrid->Cells[1][idChnl] = paramValue;
		  else if (idParam == 'x') MDP_2RNE->ChnlParamGrid->Cells[2][idChnl] = paramValue;
		  else if (idParam == 'h') MDP_2RNE->ChnlParamGrid->Cells[3][idChnl] = paramValue;
		  else if (idParam == 's') MDP_2RNE->ChnlParamGrid->Cells[4][idChnl] = paramValue;
		  else if (idParam == 'p') MDP_2RNE->ChnlParamGrid->Cells[5][idChnl] = paramValue;
		  else if (idParam == 'o') MDP_2RNE->ChnlParamGrid->Cells[6][idChnl] = paramValue;
		}
	  }
	  MDP_2RNE->Show();
	}
  }
}
//---------------------------------------------------------------------------


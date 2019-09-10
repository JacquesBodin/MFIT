//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PestGeneralForm.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPestGeneral *PestGeneral;
//---------------------------------------------------------------------------
__fastcall TPestGeneral::TPestGeneral(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool __fastcall TPestGeneral::checkEXTDRVOption() const {return m_EXTDRV_Option;}
int __fastcall TPestGeneral::getNOPTMAX() const {return m_NOPTMAX;}
double __fastcall TPestGeneral::getPHIREDSTP() const {return m_PHIREDSTP;}
double __fastcall TPestGeneral::getRELPARSTP() const {return m_RELPARSTP;}
int __fastcall TPestGeneral::getNPHISTP() const {return m_NPHISTP;}
int __fastcall TPestGeneral::getNRELPAR() const {return m_NRELPAR;}
int __fastcall TPestGeneral::getNPHINORED() const {return m_NPHINORED;}
bool __fastcall TPestGeneral::checkSVDOption() const {return m_SVD_Option;}
bool __fastcall TPestGeneral::checkTikRegOption() const {return m_TikReg_Option;}
int __fastcall TPestGeneral::getTikRegType() const {return m_TikReg_Type;}
double __fastcall TPestGeneral::getPHIMLIM() const {return m_PHIMLIM;}
int __fastcall TPestGeneral::getNrandSampl() const {return m_NrandSampl;}
bool __fastcall TPestGeneral::checkUserApprovalFlag() const {return m_USER_APPROVAL_FLAG;}

void __fastcall TPestGeneral::setEXTDRVOption(bool extDrvOption) {m_EXTDRV_Option = extDrvOption;}
void __fastcall TPestGeneral::setSVDOption(bool svdOption) {m_SVD_Option = svdOption;}
void __fastcall TPestGeneral::setNOPTMAX(int noptmax) {m_NOPTMAX = noptmax;}
void __fastcall TPestGeneral::setPHIREDSTP(double phiredstp) {m_PHIREDSTP = phiredstp;}
void __fastcall TPestGeneral::setRELPARSTP(double relparstp) {m_RELPARSTP = relparstp;}
void __fastcall TPestGeneral::setNPHISTP(int nphistp) {m_NPHISTP = nphistp;}
void __fastcall TPestGeneral::setNRELPAR(int nrelpar) {m_NRELPAR = nrelpar;}
void __fastcall TPestGeneral::setNPHINORED(int nphinored) {m_NPHINORED = nphinored;}
void __fastcall TPestGeneral::setTikRegOption(bool tikRegOption) {m_TikReg_Option = tikRegOption;}
void __fastcall TPestGeneral::setTikRegType(int TikRegType) {m_TikReg_Type = TikRegType;}
void __fastcall TPestGeneral::setPHIMLIM(double phimlim) {m_PHIMLIM = phimlim;}
void __fastcall TPestGeneral::setNrandSampl(int nrandsampl) {m_NrandSampl = nrandsampl;}
void __fastcall TPestGeneral::setUserApprovalFlag(bool userApprovalFlag) {m_USER_APPROVAL_FLAG = userApprovalFlag;}
//---------------------------------------------------------------------------
void __fastcall TPestGeneral::FormCreate(TObject *Sender)
{
  m_USER_APPROVAL_FLAG = false; // set to "true" when the user clicks on the "Ok" button
  m_EXTDRV_Option = true;
  m_SVD_Option = true;
  m_NOPTMAX = 100;
  m_PHIREDSTP = 1.0E-4;
  m_NPHISTP = 5;
  m_NPHINORED = 4;
  m_RELPARSTP = 1.0E-4;
  m_NRELPAR = 5;
  m_TikReg_Option = true;
  m_TikReg_Type = 1;
  m_PHIMLIM = 1.0E-10;
  m_NrandSampl = 500;
}
//---------------------------------------------------------------------------
void __fastcall TPestGeneral::TikReg_CheckBoxClick(TObject *Sender)
{
  if (TikReg_CheckBox->Checked)
  {
	TikRegType_Label->Enabled = true;
	TikRegType_CBox->Enabled = true;
	PHIMLIM_Label->Enabled = true;
	PHIMLIM_Edit->Enabled = true;
  }
  else
  {
	TikRegType_Label->Enabled = false;
	TikRegType_CBox->Enabled = false;
	PHIMLIM_Label->Enabled = false;
	PHIMLIM_Edit->Enabled = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestGeneral::savePestOptions()
{
  m_EXTDRV_Option = EXTDRV_CheckBox->Checked;
  m_SVD_Option = SVD_CheckBox->Checked;
  m_NOPTMAX = NOPTMAX_Edit->Text.ToInt();
  m_PHIREDSTP = PHIREDSTP_Edit->Text.ToDouble();
  m_NPHISTP = NPHISTP_Edit->Text.ToInt();
  m_NPHINORED = NPHINORED_Edit->Text.ToInt();
  m_RELPARSTP = RELPARSTP_Edit->Text.ToDouble();
  m_NRELPAR = NRELPAR_Edit->Text.ToInt();
  m_TikReg_Option = TikReg_CheckBox->Checked;
  if (TikRegType_CBox->ItemIndex == 0) m_TikReg_Type = 1;
  else m_TikReg_Type = 2;
  m_PHIMLIM = PHIMLIM_Edit->Text.ToDouble();
  m_NrandSampl = nRandSampl_Edit->Text.ToInt();
}
//---------------------------------------------------------------------------
void __fastcall TPestGeneral::OK_ButtonClick(TObject *Sender)
{
  savePestOptions();
  Hide();
  m_USER_APPROVAL_FLAG = true;
  Main_Form->notifyChanges();
}
//---------------------------------------------------------------------------
void __fastcall TPestGeneral::BackToSavedPestOptions()
{
  EXTDRV_CheckBox->Checked = m_EXTDRV_Option;
  SVD_CheckBox->Checked = m_SVD_Option;
  NOPTMAX_Edit->Text = m_NOPTMAX;
  PHIREDSTP_Edit->Text = m_PHIREDSTP;
  NPHISTP_Edit->Text = m_NPHISTP;
  NPHINORED_Edit->Text = m_NPHINORED;
  RELPARSTP_Edit->Text = m_RELPARSTP;
  NRELPAR_Edit->Text = m_NRELPAR;
  TikReg_CheckBox->Checked = m_TikReg_Option;
  if (TikReg_CheckBox->Checked)
  {
	TikRegType_Label->Enabled = true;
	TikRegType_CBox->Enabled = true;
	PHIMLIM_Label->Enabled = true;
	PHIMLIM_Edit->Enabled = true;
  }
  else
  {
	TikRegType_Label->Enabled = false;
	TikRegType_CBox->Enabled = false;
	PHIMLIM_Label->Enabled = false;
	PHIMLIM_Edit->Enabled = false;
  }
  if (m_TikReg_Type == 2) TikRegType_CBox->ItemIndex = 1;
  else TikRegType_CBox->ItemIndex = 0;
  PHIMLIM_Edit->Text = m_PHIMLIM;
}
//---------------------------------------------------------------------------
void __fastcall TPestGeneral::Cancel_ButtonClick(TObject *Sender)
{
  BackToSavedPestOptions();
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TPestGeneral::FormClose(TObject *Sender, TCloseAction &Action)
{
  BackToSavedPestOptions();
}
//---------------------------------------------------------------------------
void __fastcall TPestGeneral::runPest()
{
  String cmdLine = "Pest MFIT";
  STARTUPINFO info = {sizeof(info)};
  PROCESS_INFORMATION processInfo;
  if (CreateProcess(NULL, cmdLine.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &info, &processInfo))
  {
	while(TRUE)
	{
	  DWORD nStatus = MsgWaitForMultipleObjects(1, &processInfo.hProcess, FALSE, INFINITE, QS_ALLINPUT);
	  if ( nStatus == WAIT_OBJECT_0 ) break;
	  MSG msg;
	  while( PeekMessage(&msg,NULL,0,0,PM_REMOVE) ) {DispatchMessage(&msg);}
	}  // launched process has exited
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
  }
}
//---------------------------------------------------------------------------





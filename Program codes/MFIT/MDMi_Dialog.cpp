//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MDMi_Dialog.h"
#include "MDMed_Dialog.h"
#include "MDP_SFDM_Dialog.h"
#include "MDP_2RNE_Dialog.h"
#include "MainForm.h"
#include "Error_MessageBox.h"
#include "TCobs_dialog.h"

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MultiFlow_Generic_Dialog"
#pragma resource "*.dfm"
TMDMi *MDMi;
//---------------------------------------------------------------------------
__fastcall TMDMi::TMDMi(TComponent* Owner)
	: TMultiFlow_Generic(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::resetForm()
{
  TsimMin_Edit->Text = 0;
  TsimMax_Edit->Text = 0;
  NoTimeSteps_Edit->Text = 500;
  n_Edit->Text = 1;
  QT_Edit->Text = 0;
  m_TsimMin = -999;
  m_TsimMax = -999;
  m_NoTimeSteps = -999;
  m_QT = -999;
  m_ModelParams.clear();
  m_AutoChnlSols.clear();
  for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
  ChnlParamGrid->RowCount = 2;
  ChnlParamGrid->Cells[0][0] = "Channel No.";
  ChnlParamGrid->Cells[0][1] = "1";
  ChnlParamGrid->Cells[1][0] = "Mass";
  ChnlParamGrid->Cells[2][0] = "T0";
  ChnlParamGrid->Cells[3][0] = "Pe";
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::FormCreate(TObject *Sender)
{
  resetForm();
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::setQT(double qt)
{
  if (qt >= 0)
  {
	m_QT = qt;
	QT_Edit->Text = FloatToStrF(qt, ffGeneral, 8, 0);
  }
  else ErrorMessage->showMessage("Error: invalid QT value");
}
//---------------------------------------------------------------------------
double __fastcall TMDMi::getQT() const
{
  if (m_QT < 0) ErrorMessage->showMessage("Error: QT value not defined");
  return m_QT;
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::initChnlsParamGrid()
{
  if (Main_Form->MDMed_menu->Checked) // importing previously validated MDMed parameters into the MDMi parameter grid
  {
	TsimMin_Edit->Text = MDMed->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDMed->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDMed->NoTimeSteps_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
	n_Edit->Text = MDMed->n_Edit->Text;
	ChnlParamGrid->RowCount = MDMed->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
	  ChnlParamGrid->Cells[2][row] = MDMed->ChnlParamGrid->Cells[2][row]; // h
	  ChnlParamGrid->Cells[3][row] = MDMed->ChnlParamGrid->Cells[3][row]; // s
	}
  }
  else if (Main_Form->MDP_SFDM_menu->Checked) // importing previously validated MDP_SFDM parameters into the MDMi parameter grid
  {
	TsimMin_Edit->Text = MDP_SFDM->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDP_SFDM->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDP_SFDM->NoTimeSteps_Edit->Text;
	QT_Edit->Text = MDP_SFDM->QT_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
	n_Edit->Text = MDP_SFDM->n_Edit->Text;
	ChnlParamGrid->RowCount = MDP_SFDM->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
	  ChnlParamGrid->Cells[1][row] = MDP_SFDM->ChnlParamGrid->Cells[1][row]; // m
	  ChnlParamGrid->Cells[2][row] = MDP_SFDM->ChnlParamGrid->Cells[3][row]; // h
	  ChnlParamGrid->Cells[3][row] = MDP_SFDM->ChnlParamGrid->Cells[4][row]; // s
	}
  }
  else if (Main_Form->MDP_2RNE_menu->Checked) // importing previously validated MDP_2RNE parameters into the MDMi parameter grid
  {
	TsimMin_Edit->Text = MDP_2RNE->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDP_2RNE->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDP_2RNE->NoTimeSteps_Edit->Text;
	QT_Edit->Text = MDP_2RNE->QT_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
	n_Edit->Text = MDP_2RNE->n_Edit->Text;
	ChnlParamGrid->RowCount = MDP_2RNE->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
	  ChnlParamGrid->Cells[1][row] = MDP_2RNE->ChnlParamGrid->Cells[1][row]; // m
	  ChnlParamGrid->Cells[2][row] = MDP_2RNE->ChnlParamGrid->Cells[3][row]; // h
	  ChnlParamGrid->Cells[3][row] = MDP_2RNE->ChnlParamGrid->Cells[4][row]; // s
	}
  }
  else // MultiFlow model not yet defined: checking (only) the times Tmin and Tmax from the user observation data (if available)
  {
	checkTsimMinMax();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::Import_ButtonClick(TObject *Sender)
{
  if (OpenDialog1->Execute())
  {
	ifstream importFile(OpenDialog1->FileName.c_str());
	if (importFile)
	{
	  double tMin, tMax, qt, m, h, s;
	  int noTimeSteps, n;
	  importFile >> tMin >> tMax >> noTimeSteps >> qt >> n;
	  if (tMin >= 0) TsimMin_Edit->Text = tMin;
	  else ErrorMessage->showMessage("Error: invalid tMin value");
	  if (tMax > tMin) TsimMax_Edit->Text = tMax;
	  else ErrorMessage->showMessage("Error: invalid tMax value");
	  if (noTimeSteps > 0) NoTimeSteps_Edit->Text = noTimeSteps;
	  else ErrorMessage->showMessage("Error: invalid noTimeSteps value");
	  if (qt > 0) QT_Edit->Text = FloatToStrF(qt, ffGeneral, 8, 0);
	  else ErrorMessage->showMessage("Error: invalid QT value");
	  if (n > 0)
	  {
		for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
		n_Edit->Text = n;
		ChnlParamGrid->RowCount = 1 + n;
		for (int row = 1; row < ChnlParamGrid->RowCount; row++)
		{
		  ChnlParamGrid->Cells[0][row] = row;
		  importFile >> m >> h >> s;
		  if (m >= 0) ChnlParamGrid->Cells[1][row] = FloatToStrF(m, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid m value");
		  if (h > 0) ChnlParamGrid->Cells[2][row] = FloatToStrF(h, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid T0 value");
		  if (s >= 0) ChnlParamGrid->Cells[3][row] = FloatToStrF(s, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid Pe value");
		}
	  }
	  else ErrorMessage->showMessage("Error: invalid number of channels");
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::Ok_ButtonClick(TObject *Sender)
{
  bool errorM(false);
  bool errorH(false);
  bool errorS(false);
  for (int row = 1; row < ChnlParamGrid->RowCount; row++)
  {
	double m(ChnlParamGrid->Cells[1][row].ToDouble());
	double h(ChnlParamGrid->Cells[2][row].ToDouble());
	double s(ChnlParamGrid->Cells[3][row].ToDouble());
	if (m<0) errorM = true;
	if (h<0) errorH = true;
	if (s<0) errorS = true;
  }
  if (errorM) ErrorMessage->showMessage("Error: invalid m value");
  else if (errorH) ErrorMessage->showMessage("Error: invalid T0 value");
  else if (errorS) ErrorMessage->showMessage("Error: invalid Pe value");
  else if (TsimMin_Edit->Text.ToDouble() < 0) ErrorMessage->showMessage("Error: invalid Tmin value");
  else if (TsimMax_Edit->Text.ToDouble() <= TsimMin_Edit->Text.ToDouble()) ErrorMessage->showMessage("Error: invalid Tmax value");
  else if (NoTimeSteps_Edit->Text.ToInt() <= 0) ErrorMessage->showMessage("Error: invalid noTimeSteps value");
  else if (QT_Edit->Text.ToDouble() < 0) ErrorMessage->showMessage("Error: invalid QT value");
  else if (ChnlParamGrid->RowCount-1 <= 0) ErrorMessage->showMessage("Error: invalid number of channels");
  else
  {
	setTsimMin(TsimMin_Edit->Text.ToDouble());
	setTsimMax(TsimMax_Edit->Text.ToDouble());
	setNoTimeSteps(NoTimeSteps_Edit->Text.ToInt());
	setQT(QT_Edit->Text.ToDouble());
	m_ModelParams.clear();
	vector<double> vectMHS (3,0); // a single channel (m, h, s, initialized as [0,0,0])
	ofstream modelInputFile("Input.txt", ios::out | ios::trunc);
	modelInputFile << getTsimMin() << endl; // min simulation time
	modelInputFile << getTsimMax() << endl; // max simulation time
	modelInputFile << getNoTimeSteps() << endl; // number of time steps
	modelInputFile << getQT() << endl; // total flowrate
	modelInputFile << ChnlParamGrid->RowCount-1 << endl; // number of channels
	modelInputFile << endl;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  double m(ChnlParamGrid->Cells[1][row].ToDouble());
	  vectMHS[0] = m;
	  modelInputFile << m << endl;
	  double h(ChnlParamGrid->Cells[2][row].ToDouble());
	  vectMHS[1] = h;
	  modelInputFile << h << endl;
	  double s(ChnlParamGrid->Cells[3][row].ToDouble());
	  vectMHS[2] = s;
	  modelInputFile << s;
	  addChnl(vectMHS);
	  if (row < ChnlParamGrid->RowCount-1) modelInputFile << endl << endl;
	}
	Main_Form->MDMi_menu->Checked = true;
	Main_Form->MDMed_menu->Checked = false;
	Main_Form->MDP_SFDM_menu->Checked = false;
    Main_Form->MDP_2RNE_menu->Checked = false;
	Main_Form->BTCcalcMenu->Enabled = true;
	Hide();
    Main_Form->notifyChanges();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::BackToSavedOptions()
{
  if (m_ModelParams.size()>0)
  {
	TsimMin_Edit->Text = getTsimMin();
	TsimMax_Edit->Text = getTsimMax();
	NoTimeSteps_Edit->Text = getNoTimeSteps();
	QT_Edit->Text = getQT();
	for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
	n_Edit->Text = getN();
	ChnlParamGrid->RowCount = 1 + getN();
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row;
	  ChnlParamGrid->Cells[1][row] = getChnlParam(row, 1);
	  ChnlParamGrid->Cells[2][row] = getChnlParam(row, 2);
	  ChnlParamGrid->Cells[3][row] = getChnlParam(row, 3);
	}
  }
  else // m_ModelParams is empty
  {
	resetForm();
	checkTsimMinMax();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::Cancel_ButtonClick(TObject *Sender)
{
  BackToSavedOptions();
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::FormClose(TObject *Sender, TCloseAction &Action)
{
  BackToSavedOptions();
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::initAutoChnlSols(int nMax)
{
  m_AutoChnlSols.clear();
  for (int i = 0; i < nMax; i++) // for each solution (set of channels)
  {
	vector<double> solParams; // storage of optimized channel parameters [qt, m1, h1, s1, m2, h2, s2, ...]
	solParams.push_back(-999); // qt
	for (int j = 0; j < i+1; j++) // for each channel of the solution
	{
	  solParams.push_back(-999); //m
	  solParams.push_back(-999); //h
	  solParams.push_back(-999); //s
	}
	m_AutoChnlSols.push_back(solParams);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::setAutoChnlSol(int n)
{ // set the n-channel solution into m_AutoChnlSols: used during the optimization process
  ifstream optParFile ("MFIT.sol");
  string line, paramName;
  double paramValue, scale, offset;
  char idParam;
  int idChnl;
  if (optParFile)
  {
	// 1. Optimized parameters
	getline(optParFile, line);
	while (getline(optParFile, line))
	{
	  istringstream (line) >> paramName >> paramValue >> scale >> offset;
	  if (paramName == "qt") m_AutoChnlSols[n-1][0] = paramValue;
	  else
	  {
		idParam = paramName[0];
		istringstream(paramName.substr(1,paramName.size()-1)) >> idChnl;
		if (idParam == 'm') m_AutoChnlSols[n-1][1+(idChnl-1)*3] = paramValue;
		else if (idParam == 'h') m_AutoChnlSols[n-1][2+(idChnl-1)*3] = paramValue;
		else if (idParam == 's') m_AutoChnlSols[n-1][3+(idChnl-1)*3] = paramValue;
	  }
	}
	// 2. Fixed (non-optimized) parameters of the solution are set from the first channel model parameters
	if (m_AutoChnlSols[n-1][0] == -999) m_AutoChnlSols[n-1][0] = getQT(); //qt
	for (idChnl = 1; idChnl <= n ; idChnl++)
	{
	  if (m_AutoChnlSols[n-1][1+(idChnl-1)*3] == -999) // m
	  {
		m_AutoChnlSols[n-1][1+(idChnl-1)*3] = getChnlParam(1,1)/double(n); // special case: redistributed
	  }
	  if (m_AutoChnlSols[n-1][2+(idChnl-1)*3] == -999) // h
	  {
		m_AutoChnlSols[n-1][2+(idChnl-1)*3] = getChnlParam(1,2);
	  }
	  if (m_AutoChnlSols[n-1][3+(idChnl-1)*3] == -999) // s
	  {
		m_AutoChnlSols[n-1][3+(idChnl-1)*3] = getChnlParam(1,3);
	  }
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::setAutoChnlSolParam(int n, int idParam, int idChnl, double paramValue)
{ // set the specified parameter of the n-channel solution into m_AutoChnlSols: used when opening a MFIT file
  if (idParam == 0) m_AutoChnlSols[n-1][0] = paramValue; //qt
  else if (idParam == 1) m_AutoChnlSols[n-1][1+(idChnl-1)*3] = paramValue; //m
  else if (idParam == 2) m_AutoChnlSols[n-1][2+(idChnl-1)*3] = paramValue; //h
  else if (idParam == 3) m_AutoChnlSols[n-1][3+(idChnl-1)*3] = paramValue; //s
}
//---------------------------------------------------------------------------
vector<double> __fastcall TMDMi::getAutoChnlSol(int n) const // extract the n-channel solution parameters
{
  return m_AutoChnlSols[n-1];
}
//---------------------------------------------------------------------------
void __fastcall TMDMi::setParamGridFromSol(int n)
{
  vector<double> solParams = getAutoChnlSol(n);
  ChnlParamGrid->RowCount = 1 + n;
  QT_Edit->Text = solParams[0];
  for (unsigned int i = 1; i < solParams.size(); i+=3)
  {
	int idChnl = 1 + (i-1)/3;
	ChnlParamGrid->Cells[1][idChnl] = FloatToStrF(solParams[1+(idChnl-1)*3], ffGeneral, 8, 0); // m
	ChnlParamGrid->Cells[2][idChnl] = FloatToStrF(solParams[2+(idChnl-1)*3], ffGeneral, 8, 0); // h
	ChnlParamGrid->Cells[3][idChnl] = FloatToStrF(solParams[3+(idChnl-1)*3], ffGeneral, 8, 0); // s
  }
  for (int row = 2; row < ChnlParamGrid->RowCount; row++) ChnlParamGrid->Cells[0][row] = row;
  n_Edit->Text = ChnlParamGrid->RowCount-1;
}
//---------------------------------------------------------------------------

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
TMDMed *MDMed;
//---------------------------------------------------------------------------
__fastcall TMDMed::TMDMed(TComponent* Owner)
	: TMultiFlow_Generic(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::resetForm()
{
  TsimMin_Edit->Text = 0;
  TsimMax_Edit->Text = 0;
  NoTimeSteps_Edit->Text = 500;
  n_Edit->Text = 1;
  c0_Edit->Text = 0;
  m_TsimMin = -999;
  m_TsimMax = -999;
  m_NoTimeSteps = -999;
  m_c0 = -999;
  m_ModelParams.clear();
  m_AutoChnlSols.clear();
  for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
  ChnlParamGrid->RowCount = 2;
  ChnlParamGrid->Cells[0][0] = "Channel No.";
  ChnlParamGrid->Cells[0][1] = "1";
  ChnlParamGrid->Cells[1][0] = "Flowrate contrib. ratio";
  ChnlParamGrid->Cells[2][0] = "T0";
  ChnlParamGrid->Cells[3][0] = "Pe";
  ChnlParamGrid->Cells[4][0] = "Gamma Coeff.";
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::FormCreate(TObject *Sender)
{
  resetForm();
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::setC0(double c0)
{
  if (c0 >= 0)
  {
	m_c0 = c0;
	c0_Edit->Text = FloatToStrF(c0, ffGeneral, 8, 0);
  }
  else ErrorMessage->showMessage("Error: invalid c0 value");
}
//---------------------------------------------------------------------------
double __fastcall TMDMed::getC0() const
{
  if (m_c0 < 0) ErrorMessage->showMessage("Error: c0 value not defined");
  return m_c0;
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::initChnlsParamGrid()
{
  if (Main_Form->ADEiMenu->Checked) // importing previously validated MDMi parameters into the MDMed parameter grid
  {
	TsimMin_Edit->Text = MDMi->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDMi->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDMi->NoTimeSteps_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++)
	{
	  ChnlParamGrid->Rows[i]->Clear();
	}
	n_Edit->Text = MDMi->n_Edit->Text;
	ChnlParamGrid->RowCount = MDMi->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
	  ChnlParamGrid->Cells[2][row] = MDMi->ChnlParamGrid->Cells[2][row]; // h
	  ChnlParamGrid->Cells[3][row] = MDMi->ChnlParamGrid->Cells[3][row]; // s
	}
  }
  else if (Main_Form->SFDMmenu->Checked)
  {
	TsimMin_Edit->Text = MDP_SFDM->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDP_SFDM->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDP_SFDM->NoTimeSteps_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++)
	{
	  ChnlParamGrid->Rows[i]->Clear();
	}
	n_Edit->Text = MDP_SFDM->n_Edit->Text;
	ChnlParamGrid->RowCount = MDP_SFDM->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
	  ChnlParamGrid->Cells[2][row] = MDP_SFDM->ChnlParamGrid->Cells[3][row]; // h
	  ChnlParamGrid->Cells[3][row] = MDP_SFDM->ChnlParamGrid->Cells[4][row]; // s
	}
  }
  else if (Main_Form->TwoRNEmenu->Checked)
  {
	TsimMin_Edit->Text = MDP_2RNE->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDP_2RNE->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDP_2RNE->NoTimeSteps_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++)
	{
	  ChnlParamGrid->Rows[i]->Clear();
	}
	n_Edit->Text = MDP_2RNE->n_Edit->Text;
	ChnlParamGrid->RowCount = MDP_2RNE->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
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
void __fastcall TMDMed::Import_ButtonClick(TObject *Sender)
{
  if (OpenDialog1->Execute())
  {
	ifstream importFile(OpenDialog1->FileName.c_str());
	if (importFile)
	{
	  double tMin, tMax, c0, f, h, s, g;
	  int noTimeSteps, n;
	  importFile >> tMin >> tMax >> noTimeSteps >> c0 >> n;
	  if (tMin >= 0) TsimMin_Edit->Text = tMin;
	  else ErrorMessage->showMessage("Error: invalid tMin value");
	  if (tMax > tMin) TsimMax_Edit->Text = tMax;
	  else ErrorMessage->showMessage("Error: invalid tMax value");
	  if (noTimeSteps > 0) NoTimeSteps_Edit->Text = noTimeSteps;
	  else ErrorMessage->showMessage("Error: invalid noTimeSteps value");
	  if (c0 >= 0) c0_Edit->Text = FloatToStrF(c0, ffGeneral, 8, 0);
	  else ErrorMessage->showMessage("Error: invalid c0 value");
	  if (n > 0)
	  {
		for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
		n_Edit->Text = n;
		ChnlParamGrid->RowCount = 1 + n;
		for (int row = 1; row < ChnlParamGrid->RowCount; row++)
		{
		  ChnlParamGrid->Cells[0][row] = row;
		  importFile >> f >> h >> s >> g;
		  if ((f >= 0) && (f <=1)) ChnlParamGrid->Cells[1][row] = FloatToStrF(f, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid f value");
		  if (h > 0) ChnlParamGrid->Cells[2][row] = FloatToStrF(h, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid T0 value");
		  if (s >= 0) ChnlParamGrid->Cells[3][row] = FloatToStrF(s, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid Pe value");
		  if ((g >= 0) && (g <= 1)) ChnlParamGrid->Cells[4][row] = FloatToStrF(g, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid g value");
		}
	  }
	  else ErrorMessage->showMessage("Error: invalid number of channels");
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::Ok_ButtonClick(TObject *Sender)
{
  bool errorF(false);
  bool errorH(false);
  bool errorS(false);
  bool errorG(false);
  for (int row = 1; row < ChnlParamGrid->RowCount; row++)
  {
	double f(ChnlParamGrid->Cells[1][row].ToDouble());
	double h(ChnlParamGrid->Cells[2][row].ToDouble());
	double s(ChnlParamGrid->Cells[3][row].ToDouble());
	double g(ChnlParamGrid->Cells[4][row].ToDouble());
	if ( (f<0) || (f>1) ) errorF = true;
	if (h<0) errorH = true;
	if (s<0) errorS = true;
	if ( (g<0) || (g>1) ) errorG = true;
  }
  if (errorF) ErrorMessage->showMessage("Error: invalid f value");
  else if (errorH) ErrorMessage->showMessage("Error: invalid T0 value");
  else if (errorS) ErrorMessage->showMessage("Error: invalid Pe value");
  else if (errorG) ErrorMessage->showMessage("Error: invalid Gamma value");
  else if (TsimMin_Edit->Text.ToDouble() < 0) ErrorMessage->showMessage("Error: invalid Tmin value");
  else if (TsimMax_Edit->Text.ToDouble() <= TsimMin_Edit->Text.ToDouble()) ErrorMessage->showMessage("Error: invalid Tmax value");
  else if (NoTimeSteps_Edit->Text.ToInt() <= 0) ErrorMessage->showMessage("Error: invalid noTimeSteps value");
  else if (c0_Edit->Text.ToDouble() < 0) ErrorMessage->showMessage("Error: invalid c0 value");
  else if (ChnlParamGrid->RowCount-1 <= 0) ErrorMessage->showMessage("Error: invalid number of channels");
  else
  {
	setTsimMin(TsimMin_Edit->Text.ToDouble());
	setTsimMax(TsimMax_Edit->Text.ToDouble());
	setNoTimeSteps(NoTimeSteps_Edit->Text.ToInt());
	setC0(c0_Edit->Text.ToDouble());
	m_ModelParams.clear();
	vector<double> vectFHSG (4,0); // a single channel (f, h, s, g, initialized as [0,0,0,0])
	ofstream modelInputFile("Input.txt", ios::out | ios::trunc);
	modelInputFile << getTsimMin() << endl; // min simulation time
	modelInputFile << getTsimMax() << endl; // max simulation time
	modelInputFile << getNoTimeSteps() << endl; // number of time steps
	modelInputFile << getC0() << endl; // source concentration
	modelInputFile << ChnlParamGrid->RowCount-1 << endl; // number of channels
	modelInputFile << endl;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  double f(ChnlParamGrid->Cells[1][row].ToDouble());
	  vectFHSG[0] = f;
	  modelInputFile << f << endl;
	  double h(ChnlParamGrid->Cells[2][row].ToDouble());
	  vectFHSG[1] = h;
	  modelInputFile << h << endl;
	  double s(ChnlParamGrid->Cells[3][row].ToDouble());
	  vectFHSG[2] = s;
	  modelInputFile << s << endl;
	  double g(ChnlParamGrid->Cells[4][row].ToDouble());
	  vectFHSG[3] = g;
	  modelInputFile << g;
	  addChnl(vectFHSG);
	  if (row < ChnlParamGrid->RowCount-1) modelInputFile << endl << endl;
	}
	Main_Form->ADEiMenu->Checked = false;
	Main_Form->ADEniMenu->Checked = true;
	Main_Form->SFDMmenu->Checked = false;
	Main_Form->TwoRNEmenu->Checked = false;
	Main_Form->BTCcalcMenu->Enabled = true;
	Hide();
	Main_Form->notifyChanges();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::BackToSavedOptions()
{
  if (m_ModelParams.size()>0)
  {
	TsimMin_Edit->Text = getTsimMin();
	TsimMax_Edit->Text = getTsimMax();
	NoTimeSteps_Edit->Text = getNoTimeSteps();
	c0_Edit->Text = getC0();
	for (int i = 1; i < ChnlParamGrid->RowCount; i++) ChnlParamGrid->Rows[i]->Clear();
	n_Edit->Text = getN();
	ChnlParamGrid->RowCount = 1 + getN();
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row;
	  ChnlParamGrid->Cells[1][row] = getChnlParam(row, 1);
	  ChnlParamGrid->Cells[2][row] = getChnlParam(row, 2);
	  ChnlParamGrid->Cells[3][row] = getChnlParam(row, 3);
	  ChnlParamGrid->Cells[4][row] = getChnlParam(row, 4);
	}
  }
  else // m_ModelParams is empty
  {
	resetForm();
	checkTsimMinMax();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::Cancel_ButtonClick(TObject *Sender)
{
  BackToSavedOptions();
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::FormClose(TObject *Sender, TCloseAction &Action)
{
  BackToSavedOptions();
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::initAutoChnlSols(int nMax)
{
  m_AutoChnlSols.clear();
  for (int i = 0; i < nMax; i++) // for each solution (set of channels)
  {
	vector<double> solParams; // storage of optimized channel parameters [c0, f1, h1, s1, g1, f2, h2, s2, g2, ...]
	solParams.push_back(-999); // c0
	for (int j = 0; j < i+1; j++) // for each channel of the solution
	{
	  solParams.push_back(-999); //f
	  solParams.push_back(-999); //h
	  solParams.push_back(-999); //s
	  solParams.push_back(-999); //g
	}
	m_AutoChnlSols.push_back(solParams);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::setAutoChnlSol(int n)
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
	  if (paramName == "c0") m_AutoChnlSols[n-1][0] = paramValue;
	  else
	  {
		idParam = paramName[0];
		istringstream(paramName.substr(1,paramName.size()-1)) >> idChnl;
		if (idParam == 'f') m_AutoChnlSols[n-1][1+(idChnl-1)*4] = paramValue;
		else if (idParam == 'h') m_AutoChnlSols[n-1][2+(idChnl-1)*4] = paramValue;
		else if (idParam == 's') m_AutoChnlSols[n-1][3+(idChnl-1)*4] = paramValue;
		else if (idParam == 'g') m_AutoChnlSols[n-1][4+(idChnl-1)*4] = paramValue;
	  }
	}
	// 2. Fixed (non-optimized) parameters of the solution are set from the first channel model parameters
	if (m_AutoChnlSols[n-1][0] == -999) m_AutoChnlSols[n-1][0] = getC0(); //C0
	for (idChnl = 1; idChnl <= n ; idChnl++)
	{
	  if (m_AutoChnlSols[n-1][1+(idChnl-1)*4] == -999) // f
	  {
		m_AutoChnlSols[n-1][1+(idChnl-1)*4] = double(1)/double(n); // special case: redistributed
	  }
	  if (m_AutoChnlSols[n-1][2+(idChnl-1)*4] == -999) // h
	  {
		m_AutoChnlSols[n-1][2+(idChnl-1)*4] = getChnlParam(1,2);
	  }
	  if (m_AutoChnlSols[n-1][3+(idChnl-1)*4] == -999) // s
	  {
		m_AutoChnlSols[n-1][3+(idChnl-1)*4] = getChnlParam(1,3);
	  }
	  if (m_AutoChnlSols[n-1][4+(idChnl-1)*4] == -999) // g
	  {
		m_AutoChnlSols[n-1][4+(idChnl-1)*4] = getChnlParam(1,4);
	  }
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::setAutoChnlSolParam(int n, int idParam, int idChnl, double paramValue)
{ // set the specified parameter of the n-channel solution into m_AutoChnlSols: used when opening a MFIT file
  if (idParam == 0) m_AutoChnlSols[n-1][0] = paramValue; //C0
  else if (idParam == 1) m_AutoChnlSols[n-1][1+(idChnl-1)*4] = paramValue; //f
  else if (idParam == 2) m_AutoChnlSols[n-1][2+(idChnl-1)*4] = paramValue; //h
  else if (idParam == 3) m_AutoChnlSols[n-1][3+(idChnl-1)*4] = paramValue; //s
  else if (idParam == 4) m_AutoChnlSols[n-1][4+(idChnl-1)*4] = paramValue; //g
}
//---------------------------------------------------------------------------
vector<double> __fastcall TMDMed::getAutoChnlSol(int n) const // extract the n-channel solution parameters
{
  return m_AutoChnlSols[n-1];
}
//---------------------------------------------------------------------------
void __fastcall TMDMed::setParamGridFromSol(int n)
{
  vector<double> solParams = getAutoChnlSol(n);
  ChnlParamGrid->RowCount = 1 + n;
  c0_Edit->Text = solParams[0];
  for (unsigned int i = 1; i < solParams.size(); i+=4)
  {
	int idChnl = 1 + (i-1)/4;
	ChnlParamGrid->Cells[1][idChnl] = FloatToStrF(solParams[1+(idChnl-1)*4], ffGeneral, 8, 0); //f
	ChnlParamGrid->Cells[2][idChnl] = FloatToStrF(solParams[2+(idChnl-1)*4], ffGeneral, 8, 0); //h
	ChnlParamGrid->Cells[3][idChnl] = FloatToStrF(solParams[3+(idChnl-1)*4], ffGeneral, 8, 0); //s
	ChnlParamGrid->Cells[4][idChnl] = FloatToStrF(solParams[4+(idChnl-1)*4], ffGeneral, 8, 0); //g
  }
  for (int row = 2; row < ChnlParamGrid->RowCount; row++) ChnlParamGrid->Cells[0][row] = row;
  n_Edit->Text = ChnlParamGrid->RowCount-1;
}
//---------------------------------------------------------------------------


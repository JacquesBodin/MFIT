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
TMDP_2RNE *MDP_2RNE;
//---------------------------------------------------------------------------
__fastcall TMDP_2RNE::TMDP_2RNE(TComponent* Owner)
	: TMultiFlow_Generic(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::resetForm()
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
  ChnlParamGrid->Cells[2][0] = "Channel Length";
  ChnlParamGrid->Cells[3][0] = "T0";
  ChnlParamGrid->Cells[4][0] = "Pe";
  ChnlParamGrid->Cells[5][0] = "Psi Coeff.";
  ChnlParamGrid->Cells[6][0] = "Omega Coeff.";
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::FormCreate(TObject *Sender)
{
  resetForm();
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::setQT(double qt)
{
  if (qt >= 0)
  {
	m_QT = qt;
	QT_Edit->Text = FloatToStrF(qt, ffGeneral, 8, 0);
  }
  else ErrorMessage->showMessage("Error: invalid QT value");
}
//---------------------------------------------------------------------------
double __fastcall TMDP_2RNE::getQT() const
{
  if (m_QT < 0) ErrorMessage->showMessage("Error: QT value not defined");
  return m_QT;
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::initChnlsParamGrid()
{
  if (Main_Form->ADEiMenu->Checked) // importing previously validated MDMi parameters into the MDP_2RNE parameter grid
  {
	TsimMin_Edit->Text = MDMi->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDMi->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDMi->NoTimeSteps_Edit->Text;
	QT_Edit->Text = MDMi->QT_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++)
	{
	  ChnlParamGrid->Rows[i]->Clear();
	}
	n_Edit->Text = MDMi->n_Edit->Text;
	ChnlParamGrid->RowCount = MDMi->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
	  ChnlParamGrid->Cells[1][row] = MDMi->ChnlParamGrid->Cells[1][row]; // m
	  ChnlParamGrid->Cells[3][row] = MDMi->ChnlParamGrid->Cells[2][row]; // h
	  ChnlParamGrid->Cells[4][row] = MDMi->ChnlParamGrid->Cells[3][row]; // s
	}
  }
  else if (Main_Form->ADEniMenu->Checked)
  {
	TsimMin_Edit->Text = MDMed->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDMed->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDMed->NoTimeSteps_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++)
	{
	  ChnlParamGrid->Rows[i]->Clear();
	}
	n_Edit->Text = MDMed->n_Edit->Text;
	ChnlParamGrid->RowCount = MDMed->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
	  ChnlParamGrid->Cells[3][row] = MDMed->ChnlParamGrid->Cells[2][row]; // h
	  ChnlParamGrid->Cells[4][row] = MDMed->ChnlParamGrid->Cells[3][row]; // s
	}
  }
  else if (Main_Form->SFDMmenu->Checked)
  {
	TsimMin_Edit->Text = MDP_SFDM->TsimMin_Edit->Text;
	TsimMax_Edit->Text = MDP_SFDM->TsimMax_Edit->Text;
	NoTimeSteps_Edit->Text = MDP_SFDM->NoTimeSteps_Edit->Text;
	QT_Edit->Text = MDP_SFDM->QT_Edit->Text;
	for (int i = 1; i < ChnlParamGrid->RowCount; i++)
	{
	  ChnlParamGrid->Rows[i]->Clear();
	}
	n_Edit->Text = MDP_SFDM->n_Edit->Text;
	ChnlParamGrid->RowCount = MDP_SFDM->ChnlParamGrid->RowCount;
	for (int row = 1; row < ChnlParamGrid->RowCount; row++)
	{
	  ChnlParamGrid->Cells[0][row] = row; // Channel No.
	  ChnlParamGrid->Cells[1][row] = MDP_SFDM->ChnlParamGrid->Cells[1][row]; // m
	  ChnlParamGrid->Cells[3][row] = MDP_SFDM->ChnlParamGrid->Cells[3][row]; // h
	  ChnlParamGrid->Cells[4][row] = MDP_SFDM->ChnlParamGrid->Cells[4][row]; // s
	}
  }
  else // MultiFlow model not yet defined: checking (only) the times Tmin and Tmax from the user observation data (if available)
  {
	checkTsimMinMax();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::Import_ButtonClick(TObject *Sender)
{
  if (OpenDialog1->Execute())
  {
	ifstream importFile(OpenDialog1->FileName.c_str());
	if (importFile)
	{
	  double tMin, tMax, qt, m, x, h, s, p, o;
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
		  importFile >> m >> x >> h >> s >> p >> o;
		  if (m >= 0) ChnlParamGrid->Cells[1][row] = FloatToStrF(m, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid m value");
		  if (x > 0) ChnlParamGrid->Cells[2][row] = FloatToStrF(x, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid x value");
		  if (h > 0) ChnlParamGrid->Cells[3][row] = FloatToStrF(h, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid T0 value");
		  if (s >= 0) ChnlParamGrid->Cells[4][row] = FloatToStrF(s, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid Pe value");
		  if ((p > 0) && (p < 1)) ChnlParamGrid->Cells[5][row] = FloatToStrF(p, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid p value");
		  if (o >= 0) ChnlParamGrid->Cells[6][row] = FloatToStrF(o, ffGeneral, 8, 0);
		  else ErrorMessage->showMessage("Error: invalid o value");
		}
	  }
	  else ErrorMessage->showMessage("Error: invalid number of channels");
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::Ok_ButtonClick(TObject *Sender)
{
  bool errorM(false);
  bool errorX(false);
  bool errorH(false);
  bool errorS(false);
  bool errorP(false);
  bool errorO(false);
  for (int row = 1; row < ChnlParamGrid->RowCount; row++)
  {
	double m(ChnlParamGrid->Cells[1][row].ToDouble());
	double x(ChnlParamGrid->Cells[2][row].ToDouble());
	double h(ChnlParamGrid->Cells[3][row].ToDouble());
	double s(ChnlParamGrid->Cells[4][row].ToDouble());
	double p(ChnlParamGrid->Cells[5][row].ToDouble());
	double o(ChnlParamGrid->Cells[6][row].ToDouble());
	if (m<0) errorM = true;
	if (x<=0) errorX = true;
	if (h<0) errorH = true;
	if (s<0) errorS = true;
	if ((p<=0) || (p>=1))  errorP = true;
	if (o<0) errorO = true;
  }
  if (errorM) ErrorMessage->showMessage("Error: invalid m value");
  else if (errorX) ErrorMessage->showMessage("Error: invalid channel length value");
  else if (errorH) ErrorMessage->showMessage("Error: invalid T0 value");
  else if (errorS) ErrorMessage->showMessage("Error: invalid Pe value");
  else if (errorP) ErrorMessage->showMessage("Error: invalid Psi value");
  else if (errorO) ErrorMessage->showMessage("Error: invalid Omega value");
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
	vector<double> vectMXHSPO (6,0); // a single channel (m, x, h, s, p, o initialized as [0,0,0,0,0,0])
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
	  vectMXHSPO[0] = m;
	  modelInputFile << m << endl;
	  double x(ChnlParamGrid->Cells[2][row].ToDouble());
	  vectMXHSPO[1] = x;
	  modelInputFile << x << endl;
	  double h(ChnlParamGrid->Cells[3][row].ToDouble());
	  vectMXHSPO[2] = h;
	  modelInputFile << h << endl;
	  double s(ChnlParamGrid->Cells[4][row].ToDouble());
	  vectMXHSPO[3] = s;
	  modelInputFile << s << endl;
	  double p(ChnlParamGrid->Cells[5][row].ToDouble());
	  vectMXHSPO[4] = p;
	  modelInputFile << p << endl;
	  double o(ChnlParamGrid->Cells[6][row].ToDouble());
	  vectMXHSPO[5] = o;
	  modelInputFile << o;
	  addChnl(vectMXHSPO);
	  if (row < ChnlParamGrid->RowCount-1) modelInputFile << endl << endl;
	}
	Main_Form->ADEiMenu->Checked = false;
	Main_Form->ADEniMenu->Checked = false;
	Main_Form->SFDMmenu->Checked = false;
	Main_Form->TwoRNEmenu->Checked = true;
	Main_Form->BTCcalcMenu->Enabled = true;
	Hide();
    Main_Form->notifyChanges();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::BackToSavedOptions()
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
	  ChnlParamGrid->Cells[4][row] = getChnlParam(row, 4);
	  ChnlParamGrid->Cells[5][row] = getChnlParam(row, 5);
	  ChnlParamGrid->Cells[6][row] = getChnlParam(row, 6);
	}
  }
  else // m_ModelParams is empty
  {
	resetForm();
	checkTsimMinMax();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::Cancel_ButtonClick(TObject *Sender)
{
  BackToSavedOptions();
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::FormClose(TObject *Sender, TCloseAction &Action)
{
  BackToSavedOptions();
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::initAutoChnlSols(int nMax)
{
  m_AutoChnlSols.clear();
  for (int i = 0; i < nMax; i++) // for each solution (set of channels)
  {
	vector<double> solParams; // storage of optimized channel parameters [qt, m1, x1, h1, s1, p1, o1, m2, x2, ...]
	solParams.push_back(-999); // qt
	for (int j = 0; j < i+1; j++) // for each channel of the solution
	{
	  solParams.push_back(-999); //m
	  solParams.push_back(-999); //x
	  solParams.push_back(-999); //h
	  solParams.push_back(-999); //s
	  solParams.push_back(-999); //p
	  solParams.push_back(-999); //o
	}
	m_AutoChnlSols.push_back(solParams);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::setAutoChnlSol(int n)
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
		if (idParam == 'm') m_AutoChnlSols[n-1][1+(idChnl-1)*6] = paramValue;
		else if (idParam == 'x') m_AutoChnlSols[n-1][2+(idChnl-1)*6] = paramValue;
		else if (idParam == 'h') m_AutoChnlSols[n-1][3+(idChnl-1)*6] = paramValue;
		else if (idParam == 's') m_AutoChnlSols[n-1][4+(idChnl-1)*6] = paramValue;
		else if (idParam == 'p') m_AutoChnlSols[n-1][5+(idChnl-1)*6] = paramValue;
		else if (idParam == 'o') m_AutoChnlSols[n-1][6+(idChnl-1)*6] = paramValue;
	  }
	}
	// 2. Fixed (non-optimized) parameters of the solution are set from the first channel model parameters
	if (m_AutoChnlSols[n-1][0] == -999) m_AutoChnlSols[n-1][0] = getQT(); //qt
	for (idChnl = 1; idChnl <= n ; idChnl++)
	{
	  if (m_AutoChnlSols[n-1][1+(idChnl-1)*6] == -999) // m
	  {
		m_AutoChnlSols[n-1][1+(idChnl-1)*6] = getChnlParam(1,1)/double(n); // special case: redistributed
	  }
	  if (m_AutoChnlSols[n-1][2+(idChnl-1)*6] == -999) // x
	  {
		m_AutoChnlSols[n-1][2+(idChnl-1)*6] = getChnlParam(1,2);
	  }
	  if (m_AutoChnlSols[n-1][3+(idChnl-1)*6] == -999) // h
	  {
		m_AutoChnlSols[n-1][3+(idChnl-1)*6] = getChnlParam(1,3);
	  }
	  if (m_AutoChnlSols[n-1][4+(idChnl-1)*6] == -999) // s
	  {
		m_AutoChnlSols[n-1][4+(idChnl-1)*6] = getChnlParam(1,4);
	  }
	  if (m_AutoChnlSols[n-1][5+(idChnl-1)*6] == -999) // p
	  {
		m_AutoChnlSols[n-1][5+(idChnl-1)*6] = getChnlParam(1,5);
	  }
	  if (m_AutoChnlSols[n-1][6+(idChnl-1)*6] == -999) // o
	  {
		m_AutoChnlSols[n-1][6+(idChnl-1)*6] = getChnlParam(1,6);
	  }
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::setAutoChnlSolParam(int n, int idParam, int idChnl, double paramValue)
{ // set the specified parameter of the n-channel solution into m_AutoChnlSols: used when opening a MFIT file
  if (idParam == 0) m_AutoChnlSols[n-1][0] = paramValue; //qt
  else if (idParam == 1) m_AutoChnlSols[n-1][1+(idChnl-1)*6] = paramValue; //m
  else if (idParam == 2) m_AutoChnlSols[n-1][2+(idChnl-1)*6] = paramValue; //x
  else if (idParam == 3) m_AutoChnlSols[n-1][3+(idChnl-1)*6] = paramValue; //h
  else if (idParam == 4) m_AutoChnlSols[n-1][4+(idChnl-1)*6] = paramValue; //s
  else if (idParam == 5) m_AutoChnlSols[n-1][5+(idChnl-1)*6] = paramValue; //p
  else if (idParam == 6) m_AutoChnlSols[n-1][6+(idChnl-1)*6] = paramValue; //o
}
//---------------------------------------------------------------------------
vector<double> __fastcall TMDP_2RNE::getAutoChnlSol(int n) const // extract the n-channel solution parameters
{
  return m_AutoChnlSols[n-1];
}
//---------------------------------------------------------------------------
void __fastcall TMDP_2RNE::setParamGridFromSol(int n)
{
  vector<double> solParams = getAutoChnlSol(n);
  ChnlParamGrid->RowCount = 1 + n;
  QT_Edit->Text = solParams[0];
  for (unsigned int i = 1; i < solParams.size(); i+=6)
  {
	int idChnl = 1 + (i-1)/6;
	ChnlParamGrid->Cells[1][idChnl] = FloatToStrF(solParams[1+(idChnl-1)*6], ffGeneral, 8, 0); //m
	ChnlParamGrid->Cells[2][idChnl] = FloatToStrF(solParams[2+(idChnl-1)*6], ffGeneral, 8, 0); //x
	ChnlParamGrid->Cells[3][idChnl] = FloatToStrF(solParams[3+(idChnl-1)*6], ffGeneral, 8, 0); //h
	ChnlParamGrid->Cells[4][idChnl] = FloatToStrF(solParams[4+(idChnl-1)*6], ffGeneral, 8, 0); //s
	ChnlParamGrid->Cells[5][idChnl] = FloatToStrF(solParams[5+(idChnl-1)*6], ffGeneral, 8, 0); //p
	ChnlParamGrid->Cells[6][idChnl] = FloatToStrF(solParams[6+(idChnl-1)*6], ffGeneral, 8, 0); //o
  }
  for (int row = 2; row < ChnlParamGrid->RowCount; row++) ChnlParamGrid->Cells[0][row] = row;
  n_Edit->Text = ChnlParamGrid->RowCount-1;
}
//---------------------------------------------------------------------------

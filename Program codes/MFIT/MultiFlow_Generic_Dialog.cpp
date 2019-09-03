//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <fstream>
#include "MultiFlow_Generic_Dialog.h"
#include "TCobs_dialog.h"
#include "Error_MessageBox.h"

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMultiFlow_Generic *MultiFlow_Generic;
//---------------------------------------------------------------------------
__fastcall TMultiFlow_Generic::TMultiFlow_Generic(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMultiFlow_Generic::setTsimMin(double tSimMin)
{
  if (tSimMin >= 0)
  {
	m_TsimMin = tSimMin;
	TsimMin_Edit->Text = tSimMin;
  }
  else ErrorMessage->showMessage("Error: invalid tSimMin value");
}
//---------------------------------------------------------------------------
double __fastcall TMultiFlow_Generic::getTsimMin() const
{
  if (m_TsimMin < 0) ErrorMessage->showMessage("Error: m_TsimMin value not defined");
  return m_TsimMin;
}
//---------------------------------------------------------------------------
void __fastcall TMultiFlow_Generic::setTsimMax(double tSimMax)
{
  if (tSimMax >= m_TsimMin)
  {
	m_TsimMax = tSimMax;
	TsimMax_Edit->Text = tSimMax;
  }
  else ErrorMessage->showMessage("Error: invalid tSimMax value");
}
//---------------------------------------------------------------------------
double __fastcall TMultiFlow_Generic::getTsimMax() const
{
  if (m_TsimMax <= m_TsimMin) ErrorMessage->showMessage("Error: m_TsimMax value not defined");
  return m_TsimMax;
}
//---------------------------------------------------------------------------
void __fastcall TMultiFlow_Generic::setNoTimeSteps(int noTimeSteps)
{
  if (noTimeSteps >= 1)
  {
	m_NoTimeSteps = noTimeSteps;
	NoTimeSteps_Edit->Text = noTimeSteps;
  }
  else ErrorMessage->showMessage("Error: invalid noTimeSteps value");
}
//---------------------------------------------------------------------------
int __fastcall TMultiFlow_Generic::getNoTimeSteps() const
{
  if (m_NoTimeSteps < 1) ErrorMessage->showMessage("Error: m_NoTimeSteps value not defined");
  return m_NoTimeSteps;
}
//---------------------------------------------------------------------------
int __fastcall TMultiFlow_Generic::getN() const
{
  if (m_ModelParams.size() == 0) ErrorMessage->showMessage("Error: number of channels = 0");
  return m_ModelParams.size();
}
//---------------------------------------------------------------------------
int __fastcall TMultiFlow_Generic::getNoParamPerChnl() const
{
  if (m_ModelParams.size() == 0) ErrorMessage->showMessage("Error: NoParamPerChnl=0");
  return m_ModelParams[0].size();
}
//---------------------------------------------------------------------------
double __fastcall TMultiFlow_Generic::getChnlParam(int idChnl, int idParam) const
{ // Extracts the parameters (m, h, s, ...) of the specified channel.
  // The shared parameters (qt, c0) are considered apart.
  // Example: for the ADEi model, m <> idParam=1, h(T0) <> idParam=2, etc.
  if ((idChnl <= getN()) && (idParam <= getNoParamPerChnl()))
  {
	return m_ModelParams[idChnl-1][idParam-1];
  }
  else
  {
	ErrorMessage->showMessage("Error: unvalid idChnl and/or idParam value");
	return -1;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMultiFlow_Generic::newPestInstructionFile()
{
  int noTimeSteps(getNoTimeSteps()); // number of simulation time steps
  if (noTimeSteps > 0)
  {
	ofstream insFile("MFIT.ins", ios::out | ios::trunc);
	insFile << "pif ~" << endl << "~CONCENTRATION VALUE~" << endl;
	for (int i=1; i<=noTimeSteps ; i++)
	{
	  insFile << "l1 ~;~ !o" << i << "!" << endl;
	}
  }
  else ErrorMessage->showMessage("Error: unvalid noTimeSteps value");
}
//---------------------------------------------------------------------------
void __fastcall TMultiFlow_Generic::addChnl(vector<double> aChnlParams)
{
  m_ModelParams.push_back(aChnlParams);
}
//---------------------------------------------------------------------------
void __fastcall TMultiFlow_Generic::checkTsimMinMax()
{
  // If observation data are available and if the minimum and maximum
  // simulation time values have not yet been defined, then the minimum
  // and maximum time values of the observations are proposed to the user
  if ((TCobs_Form->getTobsMin() != -999) && (m_ModelParams.size()==0))
  {
	TsimMin_Edit->Text = ceil(TCobs_Form->getTobsMin());
	TsimMin_Edit->Text = TsimMin_Edit->Text + ".0";
	TsimMax_Edit->Text = floor(TCobs_Form->getTobsMax());
	TsimMax_Edit->Text = TsimMax_Edit->Text + ".0";
  }
}
//---------------------------------------------------------------------------
void __fastcall TMultiFlow_Generic::Update_ButtonClick(TObject *Sender)
{
  int n = n_Edit->Text.ToInt();
  ChnlParamGrid->RowCount = 1 + n;
  for (int i=1; i<=n; i++) ChnlParamGrid->Cells[0][i]=i;
}
//---------------------------------------------------------------------------







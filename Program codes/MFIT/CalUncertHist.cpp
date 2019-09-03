//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CalUncertHist.h"
#include "Error_MessageBox.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPostCalHist_Form *PostCalHist_Form;
//---------------------------------------------------------------------------
__fastcall TPostCalHist_Form::TPostCalHist_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPostCalHist_Form::clearVectRecalSets()
{
  m_vectRecalSets.clear();
}
//---------------------------------------------------------------------------
void __fastcall TPostCalHist_Form::addRecalSet(std::vector<double> vRecal)
{
  m_vectRecalSets.push_back(vRecal);
}
//---------------------------------------------------------------------------
int __fastcall TPostCalHist_Form::getNoRecalSets() const
{
  return m_vectRecalSets.size();
}
//---------------------------------------------------------------------------
double __fastcall TPostCalHist_Form::getRecalParamValue(unsigned int idRecalSet, unsigned int idParam)
{
  if ((idRecalSet < m_vectRecalSets.size())&&(idParam<m_vectRecalSets[0].size()))
  {
	return m_vectRecalSets[idRecalSet][idParam];
  }
  else if (idRecalSet >= m_vectRecalSets.size())
  {
	ErrorMessage->showMessage("Error: unvalid idRecalSet value");
	return -999;
  }
  else
  {
	ErrorMessage->showMessage("Error: unvalid idParam value");
	return -999;
  }
}
//---------------------------------------------------------------------------
double __fastcall TPostCalHist_Form::getParMin(unsigned int idParam)
{
  if (idParam < m_vectRecalSets[0].size())
  {
	double min = m_vectRecalSets[0][idParam];
	for (unsigned int i = 0; i < m_vectRecalSets.size(); i++)
	{
	  if (m_vectRecalSets[i][idParam] < min) min = m_vectRecalSets[i][idParam];
	}
	return min;
  }
  else
  {
	ErrorMessage->showMessage("Error: unvalid idParam value");
	return -999;
  }
}
//---------------------------------------------------------------------------
double __fastcall TPostCalHist_Form::getParMax(unsigned int idParam)
{
  if (idParam < m_vectRecalSets[0].size())
  {
	double max = m_vectRecalSets[0][idParam];
	for (unsigned int i = 0; i < m_vectRecalSets.size(); i++)
	{
	  if (m_vectRecalSets[i][idParam] > max) max = m_vectRecalSets[i][idParam];
	}
	return max;
  }
  else
  {
	ErrorMessage->showMessage("Error: unvalid idParam value");
	return -999;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPostCalHist_Form::Cancel_ButtonClick(TObject *Sender)
{
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TPostCalHist_Form::Param_CBoxChange(TObject *Sender)
{
	int noRecalSets = getNoRecalSets();
	int idParam = Param_CBox->ItemIndex;
	double minParVal = 0.99*getParMin(idParam);
	double maxParVal = 1.01*getParMax(idParam);
	int nBins(2.0*pow(noRecalSets,1.0/3.0)); // Rice rule
	double binWidth = (maxParVal-minParVal)/double(nBins);
	Hist_Chart->Series[0]->Clear();
	for (int i = 0; i < nBins; i++)
	{
	  double binMin = minParVal + i*binWidth;
	  double binMax = binMin + binWidth;
	  double binCenter = 0.5*(binMin + binMax);
	  int binFreq(0);
	  for (int j = 0; j < noRecalSets; j++)
	  {
		double parVal = getRecalParamValue(j, idParam);
		if ((parVal>=binMin)&&(parVal<binMax)) binFreq++;
	  }
	  Hist_Chart->Series[0]->AddXY(binCenter,binFreq);
	}
	Caption = "Post calibration analysis - " + Param_CBox->Text;
}
//---------------------------------------------------------------------------

void __fastcall TPostCalHist_Form::Export_ButtonClick(TObject *Sender)
{
  if (SaveDialog1->Execute())
  {
	ofstream recalParamFile(SaveDialog1->FileName.c_str(), ios::out | ios::trunc);
	if (recalParamFile)
	{
	  int noRecalSets = getNoRecalSets();
	  int idParam = Param_CBox->ItemIndex;
	  for (int i = 0; i < noRecalSets; i++)
	  {
		double parVal = getRecalParamValue(i, idParam);
		recalParamFile << parVal << endl;
	  }
	}
  }
}
//---------------------------------------------------------------------------


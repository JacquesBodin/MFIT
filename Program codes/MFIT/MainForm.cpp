//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "WrkDir_Dialog.h"
#include "TCobs_dialog.h"
#include "FormatPlot_Dialog.h"
#include "MDMi_Dialog.h"
#include "MDMed_Dialog.h"
#include "MDP_SFDM_Dialog.h"
#include "MDP_2RNE_Dialog.h"
#include "Error_MessageBox.h"
#include "PestGeneralForm.h"
#include "PestMDMiUserChnls_Dialog.h"
#include "PestMDMiAutoChnls_Dialog.h"
#include "PestMDMedUserChnls_Dialog.h"
#include "PestMDMedAutoChnls_Dialog.h"
#include "PestSFDMuserChnls_Dialog.h"
#include "PestSFDMautoChnls_Dialog.h"
#include "Pest2RNEuserChnls_Dialog.h"
#include "Pest2RNEautoChnls_Dialog.h"
#include "PestcheckOutput_MessageBox.h"
#include "EndInvUserChnls_Dialog.h"
#include "CalUncertHist.h"
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  // *** temp
#include <shellapi.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip> // std::setprecision
#include "spline.h"
using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMain_Form *Main_Form;
//---------------------------------------------------------------------------
__fastcall TMain_Form::TMain_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::ObservationsMenuClick(TObject *Sender)
{
  TCobs_Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::WrkDirMenuClick(TObject *Sender)
{
  WrkDir_Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::ADEiParamMenuClick(TObject *Sender)
{
  if (!ADEiMenu->Checked)
  {
	PhiN_Chart->Series[0]->Clear();
	PhiN_Chart->Series[1]->Clear();
	PhiN_SplitViewPannel->Close();
	PestParamMenu->Enabled = false;
	PestUserChnls->Checked = false;
	PestAutoChnls->Checked = false;
	PestADEiUserChnls->resetPestParamGrid();
	PestADEiAutoChnls->resetPestParamGrid();
	PestADEiAutoChnls->clearPestParams();
	PestCreateDatasetsMenu->Enabled = false;
	RunPestChekMenu->Enabled = false;
	RunPestMenu->Enabled = false;
	CalUncertMenu->Enabled = false;
  }
  MDMi->initChnlsParamGrid();
  MDMi->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::ADEniParamMenuClick(TObject *Sender)
{
  if (!ADEniMenu->Checked)
  {
	PhiN_Chart->Series[0]->Clear();
	PhiN_Chart->Series[1]->Clear();
	PhiN_SplitViewPannel->Close();
	PestParamMenu->Enabled = false;
	PestUserChnls->Checked = false;
	PestAutoChnls->Checked = false;
	PestADEniUserChnls->resetPestParamGrid();
	PestADEniAutoChnls->resetPestParamGrid();
	PestADEniAutoChnls->clearPestParams();
	PestCreateDatasetsMenu->Enabled = false;
	RunPestChekMenu->Enabled = false;
	RunPestMenu->Enabled = false;
	CalUncertMenu->Enabled = false;
  }
  MDMed->initChnlsParamGrid();
  MDMed->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::SFDMparamMenuClick(TObject *Sender)
{
  if (!SFDMmenu->Checked)
  {
	PhiN_Chart->Series[0]->Clear();
	PhiN_Chart->Series[1]->Clear();
	PhiN_SplitViewPannel->Close();
	PestParamMenu->Enabled = false;
	PestUserChnls->Checked = false;
	PestAutoChnls->Checked = false;
	PestSFDMuserChnls->resetPestParamGrid();
	PestSFDMautoChnls->resetPestParamGrid();
	PestSFDMautoChnls->clearPestParams();
	PestCreateDatasetsMenu->Enabled = false;
	RunPestChekMenu->Enabled = false;
	RunPestMenu->Enabled = false;
	CalUncertMenu->Enabled = false;
  }
  MDP_SFDM->initChnlsParamGrid();
  MDP_SFDM->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::TwoRNEparamMenuClick(TObject *Sender)
{
  if (!TwoRNEmenu->Checked)
  {
	PhiN_Chart->Series[0]->Clear();
	PhiN_Chart->Series[1]->Clear();
	PhiN_SplitViewPannel->Close();
	PestParamMenu->Enabled = false;
	PestUserChnls->Checked = false;
	PestAutoChnls->Checked = false;
	Pest2RNEuserChnls->resetPestParamGrid();
	Pest2RNEautoChnls->resetPestParamGrid();
	Pest2RNEautoChnls->clearPestParams();
	PestCreateDatasetsMenu->Enabled = false;
	RunPestChekMenu->Enabled = false;
	RunPestMenu->Enabled = false;
	CalUncertMenu->Enabled = false;
  }
  MDP_2RNE->initChnlsParamGrid();
  MDP_2RNE->Show();
}
//---------------------------------------------------------------------------
bool __fastcall TMain_Form::process_Model_Output()
{
  bool success(true);
  m_modelBTC.clear();
  ifstream simOutputFile("Output.txt", ios::in); // Open the file "Output.txt" for reading. This file contains the time-concentration values of the simulated BTC
  if (simOutputFile) // if opening sucess
  {
	string firstLine, line;
	char semicolon;
	vector<double> vect_TC (2,0); // time, concentration, initialized as [0,0]
	getline(simOutputFile, firstLine);
	while (simOutputFile >> vect_TC[0])
	{
	  simOutputFile >> semicolon >> vect_TC[1];
	  m_modelBTC.push_back(vect_TC);
	}
  }
  else success = false;
  return success;
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::plot_modelBTC(vector< vector<double> > vect_BTC)
{
  TC_Chart->Series[1]->Clear();
  for (unsigned int i = 0; i < vect_BTC.size(); i++)
  {
	TC_Chart->Series[1]->AddXY(vect_BTC[i][0], vect_BTC[i][1]);
  }
}
//---------------------------------------------------------------------------
double __fastcall TMain_Form::phiCalc()
{ // computes the PHI value (sum of squared weighted residuals) reached by PEST
  vector<double> vectUserObsT, vectUserObsC, vectUserObsW;
  for (int i = 0; i < TCobs_Form->getNoUserObs(); i++)
  {
	vectUserObsT.push_back(TCobs_Form->getTobsValue(i));
	vectUserObsC.push_back(TCobs_Form->getCobsValue(i));
	vectUserObsW.push_back(TCobs_Form->getWobsValue(i));
  }
  tk::spline s1, s2; // see http://kluge.in-chemnitz.de/opensource/spline/
  s1.set_points(vectUserObsT,vectUserObsC);
  s2.set_points(vectUserObsT,vectUserObsW);
  double phiResult(0);
  for (unsigned int i = 0; i < m_modelBTC.size(); i++)
  {
	double tSim = m_modelBTC[i][0]; // time value from the model output
	double cSim = m_modelBTC[i][1]; // concentration value from the model output
	double sUserObsC_tSim = s1(tSim); // corresponding concentration value, interpolated from the Observation_Dialog StringGrig
	if (sUserObsC_tSim < 0) sUserObsC_tSim = 0;
	double sUserObsW_tSim = s2(tSim); // corresponding weight value, interpolated from the Observation_Dialog StringGrig
	if (sUserObsW_tSim < 0) sUserObsW_tSim = 0;
	phiResult = phiResult + pow((sUserObsC_tSim-cSim)*sUserObsW_tSim,2);
  }
  return phiResult;
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::init_AutoChnlSolBTCs(int nMax)
{
  m_AutoChnlSolBTCs.clear();
  vector< vector<double> > vect_BTC_init (m_modelBTC); // initialization using the vector m_modelBTC, which has a suited size
  for (int n = 1; n <= nMax; n++) m_AutoChnlSolBTCs.push_back(vect_BTC_init);
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::set_AutoChnlSolBTC(int n, vector< vector<double> > vect_BTC)
{
  m_AutoChnlSolBTCs[n-1] = vect_BTC;
}
//---------------------------------------------------------------------------
vector< vector<double> > __fastcall TMain_Form::get_AutoChnlSolBTC(int n) const
{
  return m_AutoChnlSolBTCs[n-1];
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::BTCcalcMenuClick(TObject *Sender)
{ if (ADEiMenu->Checked) system("MDMi");
  else if (ADEniMenu->Checked)	system("MDMed");
  else if (SFDMmenu->Checked) system("MDP_SFDM");
  else if (TwoRNEmenu->Checked) system("MDP_2RNE");
  if (process_Model_Output())
  {
	plot_modelBTC(m_modelBTC);
	TC_Chart->Visible = true;
	PestParamMenu->Enabled = true;
	notifyChanges();
  }
  else
  {
	ErrorMessage->showMessage("Error: cannot open the file Output.txt");
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::TC_ChartClickAxis(TCustomChart *Sender, TChartAxis *Axis,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
  FormatPlot_Form->checkChartAxisMinMax();
  FormatPlot_Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::PestOptionsMenuClick(TObject *Sender)
{
  if (!PestGeneral->Visible)
  {
	PestGeneral->savePestOptions();
	PestGeneral->Show();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::PestUserChnlsClick(TObject *Sender)
{
  if (ADEiMenu->Checked)
  {
	PestADEiUserChnls->resetChnlNo_CBox();
	PestADEiUserChnls->Show();
  }
  else if (ADEniMenu->Checked)
  {
	PestADEniUserChnls->resetChnlNo_CBox();
	PestADEniUserChnls->Show();
  }
  else if (SFDMmenu->Checked)
  {
	PestSFDMuserChnls->resetChnlNo_CBox();
	PestSFDMuserChnls->Show();
  }
  else if (TwoRNEmenu->Checked)
  {
	Pest2RNEuserChnls->resetChnlNo_CBox();
	Pest2RNEuserChnls->Show();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::PestAutoChnlsClick(TObject *Sender)
{
  if (ADEiMenu->Checked) PestADEiAutoChnls->Show();
  else if (ADEniMenu->Checked) PestADEniAutoChnls->Show();
  else if (SFDMmenu->Checked) PestSFDMautoChnls->Show();
  else if (TwoRNEmenu->Checked) Pest2RNEautoChnls->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::PestCreateDatasetsMenuClick(TObject *Sender)
{
  PestGeneral->savePestOptions();
  remove("MFIT.sol");
  if (ADEiMenu->Checked)
  {
	MDMi->newPestInstructionFile();
	if (PestUserChnls->Checked) // "UserChannels" option: the Pest optimization is based on a fixed number of channels, specified by the user
	{
	  PestADEiUserChnls->newPestTplFile();
	  PestADEiUserChnls->newPestControlFile();
	}
	else // "AutoChannels" option: multiple PEST runs while automatically decreasing the number of channels
	{
	  int const nMax = PestADEiAutoChnls->Nmax_Edit->Text.ToInt();
	  PestADEiAutoChnls->newPestTplFile(nMax);
	  PestADEiAutoChnls->newPestControlFile(nMax, -999);
	}
  }
  else if (ADEniMenu->Checked)
  {
	MDMed->newPestInstructionFile();
	if (PestUserChnls->Checked)
	{
	  PestADEniUserChnls->newPestTplFile();
	  PestADEniUserChnls->newPestControlFile();
	}
	else
	{
	  int const nMax = PestADEniAutoChnls->Nmax_Edit->Text.ToInt();
	  PestADEniAutoChnls->newPestTplFile(nMax);
	  PestADEniAutoChnls->newPestControlFile(nMax, -999);
	}
  }
  else if (SFDMmenu->Checked)
  {
	MDP_SFDM->newPestInstructionFile();
	if (PestUserChnls->Checked)
	{
	  PestSFDMuserChnls->newPestTplFile();
	  PestSFDMuserChnls->newPestControlFile();
	}
	else
	{
	  int const nMax = PestSFDMautoChnls->Nmax_Edit->Text.ToInt();
	  PestSFDMautoChnls->newPestTplFile(nMax);
	  PestSFDMautoChnls->newPestControlFile(nMax, -999);
	}
  }
  else if (TwoRNEmenu->Checked)
  {
	MDP_2RNE->newPestInstructionFile();
	if (PestUserChnls->Checked)
	{
	  Pest2RNEuserChnls->newPestTplFile();
	  Pest2RNEuserChnls->newPestControlFile();
	}
	else
	{
	  int const nMax = Pest2RNEautoChnls->Nmax_Edit->Text.ToInt();
	  Pest2RNEautoChnls->newPestTplFile(nMax);
	  Pest2RNEautoChnls->newPestControlFile(nMax, -999);
	}
  }
  RunPestChekMenu->Enabled = true;
  RunPestMenu->Enabled = true;
  CalUncertMenu->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::RunPestChekMenuClick(TObject *Sender)
{
  system("Pestchek MFIT > PestchekOutput.txt");
  PestcheckOutputMessage->Memo->Lines->Clear();
  PestcheckOutputMessage->Caption = "PestChek Results";

  TStringList *StrList = new TStringList;
  StrList->LoadFromFile("PestchekOutput.txt");
  PestcheckOutputMessage->Memo->Lines = StrList;
  delete StrList;

  // or:
  /*ifstream pestchek_File ("PestchekOutput.txt", ios::in);
  if (pestchek_File)
  {
	string line;
	while(getline(pestchek_File, line)) PestOutput_Form->Memo->Lines->Add(line.c_str());
  }*/

  PestcheckOutputMessage->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::RunPestMenuClick(TObject *Sender)
{
  // The paths of the Pest.exe, MDMi.exe, MDMed.exe, MDP_SFDM.exe, and MDP_2RNE.exe program folders
  // must be defined in the windows environment variables: Control pannel -> System and security ->
  // Advanced System Settings -> Environment variables -> Path = [...]

  //------------- Cleaning -------------
  PhiN_SplitViewPannel->Close();
  PhiN_Chart->Series[0]->Clear();
  PhiN_Chart->Series[1]->Clear();
  remove("Phi(N).txt");
  PestAbortMenu->Enabled = true;
  PestAbortSpeedButton->Visible = true;
  CalUncertMenu->Enabled = false;
  //boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();  //*** temp
  //----- Getting the number of channels for the first Pest optimization -----
  int n_ini;
  if (PestUserChnls->Checked)
  {
	if (ADEiMenu->Checked) n_ini = MDMi->getN();
	else if (ADEniMenu->Checked) n_ini = MDMed->getN();
	else if (SFDMmenu->Checked) n_ini = MDP_SFDM->getN();
	else if (TwoRNEmenu->Checked) n_ini = MDP_2RNE->getN();
  }
  else // PestAutoChnls->Checked
  {
	if (ADEiMenu->Checked) n_ini = PestADEiAutoChnls->Nmax_Edit->Text.ToInt();
	else if (ADEniMenu->Checked) n_ini = PestADEniAutoChnls->Nmax_Edit->Text.ToInt();
	else if (SFDMmenu->Checked) n_ini = PestSFDMautoChnls->Nmax_Edit->Text.ToInt();
	else if (TwoRNEmenu->Checked) n_ini = Pest2RNEautoChnls->Nmax_Edit->Text.ToInt();
  }
  //---- First Pest optimization, based either on a fixed number of channels
  //----   specified by the user (option "UserChannels"), or on the maximum
  //----   allowed number of channels (option "AutoChannels")
  StatusBar->Panels->Items[0]->Text = "Optimization in progress, please wait...";
  ActivityIndicator1->Animate = true;
  PestGeneral->runPest();
  //---- End of the 1st optimization -------
  StatusBar->Panels->Items[0]->Text = "";
  ActivityIndicator1->Animate = false;
  if (PestAbortSpeedButton->Hint == "Aborting")
  {
	ErrorMessage->showMessage("Optimization stopped by the user");
  }
  else if (process_Model_Output() == false)
  {
	ErrorMessage->showMessage("Error: Pest optimization failed, please modify settings.");
  }
  else // 1st optimization successful
  {
	// Process and display the 1st optimization results
	double phi = phiCalc();
	double phiMinAutoChnls(phi);
	double phiMaxAutoChnls(phi);
	string s1 =  "Displaying the ";
	string sNoChnls = boost::lexical_cast<string>(n_ini);
	string s3 =  "-channel optimized solution";
	string s4 =  s1 + sNoChnls + s3;
	plot_modelBTC(m_modelBTC);
	StatusBar->Panels->Items[1]->Text = s4.c_str();
	Application->ProcessMessages(); // graphical display udpating
	// if "AutoChannels" option >> multiple PEST runs while decreasing the number of channels
	if (PestAutoChnls->Checked)
	{
	  //---- Backup of the first (successful) Pest optimization results ----
	  int nMax(n_ini);
	  init_AutoChnlSolBTCs(nMax);
	  set_AutoChnlSolBTC(nMax, m_modelBTC);
	  ofstream phi_N_file("Phi(N).txt", ios::out | ios::trunc);
	  phi_N_file << "NoChannels PhiMin" << endl;
	  phi_N_file << nMax << " " << phi << endl;
	  PhiN_Chart->Series[0]->AddXY(nMax,phi);
	  remove("MFIT.sol");
	  rename("MFIT.par","MFIT.sol");
	  if (ADEiMenu->Checked)
	  {
		MDMi->initAutoChnlSols(nMax); // initialisation of the [1~nMax] channel solutions, parameter values set to -999
		MDMi->setAutoChnlSol(nMax); // setting the correct (optimized) parameter values of the nMax-channel solution
	  }
	  else if (ADEniMenu->Checked)
	  {
		MDMed->initAutoChnlSols(nMax);
		MDMed->setAutoChnlSol(nMax);
	  }
	  else if (SFDMmenu->Checked)
	  {
		MDP_SFDM->initAutoChnlSols(nMax);
		MDP_SFDM->setAutoChnlSol(nMax);
	  }
	  else if (TwoRNEmenu->Checked)
	  {
		MDP_2RNE->initAutoChnlSols(nMax);
		MDP_2RNE->setAutoChnlSol(nMax);
	  }
	  //-------- Computation of the number of AutoChannels optimzation runs: used for the ProgressBar --------
	  int totalPestAutoChnlRuns;
	  if (((ADEiMenu->Checked)&&(PestADEiAutoChnls->MICP_CheckBox->Checked))
			  ||((ADEniMenu->Checked)&&(PestADEniAutoChnls->MICP_CheckBox->Checked))
			  ||((SFDMmenu->Checked)&&(PestSFDMautoChnls->MICP_CheckBox->Checked))
			||((TwoRNEmenu->Checked)&&(Pest2RNEautoChnls->MICP_CheckBox->Checked)))
	  {
		totalPestAutoChnlRuns = nMax*(nMax+1)/2;
	  }
	  else totalPestAutoChnlRuns = nMax;
	  int nPestAutoChnlRuns(0);
	  int n(nMax);
	  //---------- AutoChnl runs ----------
	  do
	  {
		n--; //(-1 channel)
		if (ADEiMenu->Checked) PestADEiAutoChnls->newPestTplFile(n);
		else if (ADEniMenu->Checked) PestADEniAutoChnls->newPestTplFile(n);
		else if (SFDMmenu->Checked) PestSFDMautoChnls->newPestTplFile(n);
		else if (TwoRNEmenu->Checked) Pest2RNEautoChnls->newPestTplFile(n);
		StatusBar->Panels->Items[0]->Text = "Optimization in progress, please wait...";
		ActivityIndicator1->Animate = true;
		double phiMinN(numeric_limits<double>::max()); // or DBL_MAX
		bool pestOptSuccess(false);
		for (int idRemovedChnl = n+1; idRemovedChnl >= 1; idRemovedChnl--)
		{
		  if (ADEiMenu->Checked) PestADEiAutoChnls->newPestControlFile(n, idRemovedChnl);
		  else if (ADEniMenu->Checked)PestADEniAutoChnls->newPestControlFile(n, idRemovedChnl);
		  else if (SFDMmenu->Checked) PestSFDMautoChnls->newPestControlFile(n, idRemovedChnl);
		  else if (TwoRNEmenu->Checked) Pest2RNEautoChnls->newPestControlFile(n, idRemovedChnl);
		  nPestAutoChnlRuns++;
		  StatusBar->Panels->Items[2]->Text = nPestAutoChnlRuns;
		  StatusBar->Panels->Items[2]->Text = StatusBar->Panels->Items[2]->Text + '/';
		  StatusBar->Panels->Items[2]->Text = StatusBar->Panels->Items[2]->Text + totalPestAutoChnlRuns;
		  ProgressBar1->Position = int(100*double(nPestAutoChnlRuns)/double(totalPestAutoChnlRuns));
		  PestGeneral->runPest(); // new Pest optimization
		  if (PestAbortSpeedButton->Hint == "Aborting")
		  {
			break;
		  }
		  else if (process_Model_Output())
		  {
			pestOptSuccess = true; // at least one successful n-channel optimization
			phi = phiCalc();
			if (phi < phiMinN)
			{
			  phiMinN = phi;
			  set_AutoChnlSolBTC(n, m_modelBTC);
			  remove("MFIT.par.best");
			  rename("MFIT.par", "MFIT.par.best");
			}
			plot_modelBTC(m_modelBTC);
			s1 =  "Currently displaying the ";
			sNoChnls = boost::lexical_cast<string>(n);
			s3 =  "-channel optimized solution";
			s4 =  s1 + sNoChnls + s3;
			Main_Form->StatusBar->Panels->Items[1]->Text = s4.c_str();
			Application->ProcessMessages(); // graphical display updating
			if (((ADEiMenu->Checked)&&(PestADEiAutoChnls->MICP_CheckBox->Checked == false))
				||((ADEniMenu->Checked)&&(PestADEniAutoChnls->MICP_CheckBox->Checked == false))
				||((SFDMmenu->Checked)&&(PestSFDMautoChnls->MICP_CheckBox->Checked == false))
				||((TwoRNEmenu->Checked)&&(Pest2RNEautoChnls->MICP_CheckBox->Checked == false)))
			{
			  break;
			}
		  }
		}
		if (PestAbortSpeedButton->Hint == "Aborting")
		{
		  PhiN_Chart->Series[0]->Clear();
		  PhiN_Chart->Series[1]->Clear();
		  remove("Phi(N).txt");
		  break;
		}
		else if (pestOptSuccess) // at least one successful n-channel optimization
		{
		  phi_N_file << n << " " << phiMinN << endl;
		  PhiN_Chart->Series[0]->AddXY(n,phiMinN);
		  PhiN_Chart->Series[1]->Clear();
		  PhiN_Chart->Series[1]->AddXY(n,phiMinN);
		  if (phiMinN < phiMinAutoChnls) phiMinAutoChnls = phiMinN;
		  if (phiMinN > phiMaxAutoChnls) phiMaxAutoChnls = phiMinN;
		  remove("MFIT.sol");
		  rename("MFIT.par.best", "MFIT.sol");
		  if (ADEiMenu->Checked) MDMi->setAutoChnlSol(n);
		  else if (ADEniMenu->Checked) MDMed->setAutoChnlSol(n);
		  else if (SFDMmenu->Checked) MDP_SFDM->setAutoChnlSol(n);
		  else if (TwoRNEmenu->Checked) MDP_2RNE->setAutoChnlSol(n);
		}
		else // n-channel optimization failed
		{
		  string s5 =  "Error: ";
		  string s6 = boost::lexical_cast<string>(n);
		  string s7 =  "-channel optimization failed";
		  string s8 =  s5 + s6 + s7;
		  ErrorMessage->showMessage(s8);
		  PhiN_Chart->Series[0]->Clear();
		  PhiN_Chart->Series[1]->Clear();
		  remove("Phi(N).txt");
		  break;
		}
	  } while (n > 1);
	  ProgressBar1->Position = 100;
	  Sleep(1000);
	}
	//--------- End of the optimization process ---------
	StatusBar->Panels->Items[0]->Text = "";
	StatusBar->Panels->Items[1]->Text = "";
	StatusBar->Panels->Items[2]->Text = "";
	ProgressBar1->Position = 0;
	ActivityIndicator1->Animate = false;
	Sleep(1000);
	//--------- Display optimization results ---------
	if (PestAbortSpeedButton->Hint != "Aborting")
	{
	  if (PestUserChnls->Checked)
	  {
		EndInversionUserChnls_Form->Show();
		if (PestGeneral->checkTikRegOption() == false)
		{
		  CalUncertMenu->Enabled = true;
		}
	  }
	  else if (PestAutoChnls->Checked)
	  {
		nUpDown->Max = n_ini;
		nUpDown->Position = 1;
		PhiN_Chart->LeftAxis->Minimum = pow(10,floor(log10(phiMinAutoChnls)));
		PhiN_Chart->LeftAxis->Maximum = pow(10,1+floor(log10(phiMaxAutoChnls)));
		PhiN_SplitViewPannel->Visible = true;
		PhiN_SplitViewPannel->Open();
	  }
	}
	//-------------------------------------------------
  }
  PestAbortSpeedButton->Hint = "";
  PestAbortSpeedButton->Visible = false;
  PestAbortMenu->Enabled = false;
  //boost::posix_time::ptime t2 = boost::posix_time::second_clock::local_time(); // *** temp
  //boost::posix_time::time_duration diff = t2 - t1; // *** temp
  //ErrorMessage->showMessage(boost::lexical_cast<string>(diff.total_seconds()));  // *** temp
  notifyChanges();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::FileSaveAsMenuClick(TObject *Sender)
{
  if (SaveDialog1->Execute())
  {
	ofstream mfiFile(SaveDialog1->FileName.c_str(), ios::out | ios::trunc);
	if (mfiFile)
	{
	  mfiFile << setprecision(7); // precision of the numbers written in the MFIT file
	  Caption = "MFIT";
	  Caption = Caption + " - " + ExtractFileName(SaveDialog1->FileName);

	  // Backup of the user observations
	  mfiFile << "OBSERVATIONS : TIME VALUE ; CONCENTRATION VALUE ; WEIGHT VALUE" << endl;
	  int const noUserObs = TCobs_Form->getNoUserObs();
	  mfiFile << noUserObs << endl;
	  if (noUserObs > 0)
	  {
		for (int i = 0; i < noUserObs; i++)
		{
		  double userObsT = TCobs_Form->getTobsValue(i);
		  double userObsC = TCobs_Form->getCobsValue(i);
		  double userObsW = TCobs_Form->getWobsValue(i);
		  mfiFile << userObsT << " " << userObsC << " " << userObsW << endl;
		}
	  }

	  // Backup of the working directory path
	  mfiFile << endl << "WORKING DIRECTORY" << endl;
	  AnsiString const wrkdirpth = WrkDir_Form->getWrkDirPth();
	  mfiFile << wrkdirpth << endl;

	  // Transport simulation parameters
	  mfiFile << endl << "MODEL PARAMETERS" << endl;
	  if (ADEiMenu->Checked)
	  {
		int const modelFlag = 0; // numerical "flag" for ADE_inst
		mfiFile << modelFlag << endl;
		mfiFile << MDMi->getTsimMin() << endl;
		mfiFile << MDMi->getTsimMax() << endl;
		mfiFile << MDMi->getNoTimeSteps() << endl;
		mfiFile << MDMi->getQT() << endl;
		int const n = MDMi->getN();
		mfiFile << n << endl;
		if (n>0)
		{
		  int const noParamPerChnl = MDMi->getNoParamPerChnl(); // number of parameters for each channel (depends on the transport model)
		  mfiFile << noParamPerChnl << endl;
		  for (int idChnl = 1; idChnl <= n; idChnl++) // for each channel
		  {
			for (int idParam = 1; idParam <= noParamPerChnl; idParam++) // for each parameter
			{
			  mfiFile << MDMi->getChnlParam(idChnl, idParam) << " ";
			}
			mfiFile << endl;
		  }
		}
	  }
	  else if (ADEniMenu->Checked)
	  {
		int const modelFlag = 1; // ADE_NonInst
		mfiFile << modelFlag << endl;
		mfiFile << MDMed->getTsimMin() << endl;
		mfiFile << MDMed->getTsimMax() << endl;
		mfiFile << MDMed->getNoTimeSteps() << endl;
		mfiFile << MDMed->getC0() << endl;
		int const n = MDMed->getN();
		mfiFile << n << endl;
		if (n>0)
		{
		  int const noParamPerChnl = MDMed->getNoParamPerChnl();
		  mfiFile << noParamPerChnl << endl;
		  for (int idChnl = 1; idChnl <= n; idChnl++)
		  {
			for (int idParam = 1; idParam <= noParamPerChnl; idParam++)
			{
			  mfiFile << MDMed->getChnlParam(idChnl, idParam) << " ";
			}
			mfiFile << endl;
		  }
		}
	  }
	  else if (SFDMmenu->Checked)
	  {
		int const modelFlag = 2; // SFDM
		mfiFile << modelFlag << endl;
		mfiFile << MDP_SFDM->getTsimMin() << endl;
		mfiFile << MDP_SFDM->getTsimMax() << endl;
		mfiFile << MDP_SFDM->getNoTimeSteps() << endl;
		mfiFile << MDP_SFDM->getQT() << endl;
		int const n = MDP_SFDM->getN();
		mfiFile << n << endl;
		if (n>0)
		{
		  int const noParamPerChnl = MDP_SFDM->getNoParamPerChnl();
		  mfiFile << noParamPerChnl << endl;
		  for (int idChnl = 1; idChnl <= n; idChnl++)
		  {
			for (int idParam = 1; idParam <= noParamPerChnl; idParam++)
			{
			  mfiFile << MDP_SFDM->getChnlParam(idChnl, idParam) << " ";
			}
			mfiFile << endl;
		  }
		}
	  }
	  else if (TwoRNEmenu->Checked)
	  {
		int const modelFlag = 3; // 2RNE
		mfiFile << modelFlag << endl;
		mfiFile << MDP_2RNE->getTsimMin() << endl;
		mfiFile << MDP_2RNE->getTsimMax() << endl;
		mfiFile << MDP_2RNE->getNoTimeSteps() << endl;
		mfiFile << MDP_2RNE->getQT() << endl;
		int const n = MDP_2RNE->getN();
		mfiFile << n << endl;
		if (n>0)
		{
		  int const noParamPerChnl = MDP_2RNE->getNoParamPerChnl();
		  mfiFile << noParamPerChnl << endl;
		  for (int idChnl = 1; idChnl <= n; idChnl++)
		  {
			for (int idParam = 1; idParam <= noParamPerChnl; idParam++)
			{
			  mfiFile << MDP_2RNE->getChnlParam(idChnl, idParam) << " ";
			}
			mfiFile << endl;
		  }
		}
	  }
	  else
	  {
		int const modelFlag = -1; // transport model not yet defined
		mfiFile << modelFlag << endl;
	  }

	  // General Pest Options
	  mfiFile << endl << "PEST OPTIONS" << endl;
	  mfiFile << PestGeneral->checkUserApprovalFlag() << endl;
	  if (PestGeneral->checkUserApprovalFlag())
	  {
		mfiFile << PestGeneral->getRLAMFAC() << endl;
		mfiFile << PestGeneral->checkEXTDRVOption() << endl;
		mfiFile << PestGeneral->checkBOUNDSCALEOption() << endl;
		mfiFile << PestGeneral->checkSVDOption() << endl;
		mfiFile << PestGeneral->getNOPTMAX() << endl;
		mfiFile << PestGeneral->getPHIREDSTP() << endl;
		mfiFile << PestGeneral->getNPHISTP() << endl;
		mfiFile << PestGeneral->getNPHINORED() << endl;
		mfiFile << PestGeneral->getRELPARSTP() << endl;
		mfiFile << PestGeneral->getNRELPAR() << endl;
		mfiFile << PestGeneral->checkTikRegOption() << endl;
		mfiFile << PestGeneral->getTikRegType() << endl;
		mfiFile << PestGeneral->getPHIMLIM() << endl;
		mfiFile << PestGeneral->getPHIMACCEPT() << endl;
		mfiFile << PestGeneral->getIREGADJ() << endl;
		mfiFile << PestGeneral->getNrandSampl() << endl;
	  }

	  // Pest parameters - channel number option
	  mfiFile << endl << "PEST PARAMETERS - CHANNELS" << endl;
	  if (PestUserChnls->Checked) // option "UserChannels" (fixed number of channels, defined by the user)
	  {
		mfiFile << "0" << endl; // UserChannels option flag
		vector< vector<double> > vectPestUserChnlParams; // a series of [Type, Channel No., Use (No=0/Yes=1), Minimum, Maximum, Transform]
		if (ADEiMenu->Checked) vectPestUserChnlParams = PestADEiUserChnls->getPestParams();
		else if (ADEniMenu->Checked) vectPestUserChnlParams = PestADEniUserChnls->getPestParams();
		else if (SFDMmenu->Checked) vectPestUserChnlParams = PestSFDMuserChnls->getPestParams();
		else if (TwoRNEmenu->Checked) vectPestUserChnlParams = Pest2RNEuserChnls->getPestParams();
		int const noPestParams = vectPestUserChnlParams.size();
		mfiFile << noPestParams << endl;
		if (noPestParams > 0)
		{
		  for (int i = 0; i < noPestParams; i++) // for each Pest parameter
		  {
			for (int j = 0; j < 6; j++) // TTUMMT: Type, Channel No., Use (No=0/Yes=1), Minimum, Maximum, Transform
			{
			  mfiFile << vectPestUserChnlParams[i][j] << " ";
			}
			mfiFile << endl;
		  }
		}
	  }
	  else if (PestAutoChnls->Checked) // option "AutoChannels"
	  {
		mfiFile << "1" << endl; // AutoChannels option flag
		if (((ADEiMenu->Checked)&&(PestADEiAutoChnls->MICP_CheckBox->Checked))
			||((ADEniMenu->Checked)&&(PestADEniAutoChnls->MICP_CheckBox->Checked))
			||((SFDMmenu->Checked)&&(PestSFDMautoChnls->MICP_CheckBox->Checked))
			||((TwoRNEmenu->Checked)&&(Pest2RNEautoChnls->MICP_CheckBox->Checked)))
		{ mfiFile << "1" << endl; } else { mfiFile << "0" << endl; } // MICP option (Multiple Initial Condition Preconditionning)
		vector< vector<double> > vectPestAutoChnlParams; // a series of [Type, Estimate (No=0/Yes=1), Minimum, Maximum, Transform]
		if (ADEiMenu->Checked) vectPestAutoChnlParams = PestADEiAutoChnls->getPestParams();
		else if (ADEniMenu->Checked) vectPestAutoChnlParams = PestADEniAutoChnls->getPestParams();
		else if (SFDMmenu->Checked) vectPestAutoChnlParams = PestSFDMautoChnls->getPestParams();
		else if (TwoRNEmenu->Checked) vectPestAutoChnlParams = Pest2RNEautoChnls->getPestParams();
		int const noPestParams = vectPestAutoChnlParams.size();
		mfiFile << noPestParams << endl;
		if (noPestParams > 0)
		{
		  if (ADEiMenu->Checked) mfiFile << PestADEiAutoChnls->Nmax_Edit->Text.ToInt() << endl;
		  else if (ADEniMenu->Checked) mfiFile << PestADEniAutoChnls->Nmax_Edit->Text.ToInt() << endl;
		  else if (SFDMmenu->Checked) mfiFile << PestSFDMautoChnls->Nmax_Edit->Text.ToInt() << endl;
		  else if (TwoRNEmenu->Checked) mfiFile << Pest2RNEautoChnls->Nmax_Edit->Text.ToInt() << endl;
		  for (int i = 0; i < noPestParams; i++)
		  {
			mfiFile << vectPestAutoChnlParams[i][0] << " "; // Type
			mfiFile << vectPestAutoChnlParams[i][1] << " "; // Estimate (No=0/Yes=1)
			mfiFile << vectPestAutoChnlParams[i][2] << " "; // Minimum
			mfiFile << vectPestAutoChnlParams[i][3] << " "; // Maximum
			mfiFile << vectPestAutoChnlParams[i][4] << " "; // Transform
			mfiFile << endl;
		  }
		}
	  }
	  else mfiFile << "-1" << endl; // channel number option (UserChannels or AutoChannels) not yet defined

	  // AutoChnl optimization results
	  mfiFile << endl << "AUTOCHNL OPTIMIZATION RESULTS" << endl;
	  int const nMax = PhiN_Chart->Series[0]->Count(); //nMax = maximum number of channels
	  mfiFile << nMax << endl;
	  if (nMax > 1) // AutoChnl optimization data are available
	  {
		// BTC solutions for 1 to Nmax channels
		for (int n = 1; n <= nMax; n++)
		{
		  vector< vector<double> > const vectBTCsol(get_AutoChnlSolBTC(n));
		  for (unsigned int i = 0; i < vectBTCsol.size(); i++)
		  {
			mfiFile << vectBTCsol[i][0] << " " << vectBTCsol[i][1] << endl;
		  }
		}
		// Phi(N) curve
		for (int n = 1; n <= nMax; n++)
		{
		  mfiFile << PhiN_Chart->Series[0]->YValues->Value[n-1] << endl;
		}
		// Optimized parameters
		if (ADEiMenu->Checked)
		{
		  for (int n = 1; n <= nMax; n++)
		  {
			vector<double> const solParams = MDMi->getAutoChnlSol(n);
			double const qt = solParams[0];
			mfiFile << qt << endl;
			for (unsigned int i = 1; i < solParams.size(); i+=3)
			{
			  int idChnl = 1 + (i-1)/3;
			  double m = solParams[1+(idChnl-1)*3];
			  double h = solParams[2+(idChnl-1)*3];
			  double s = solParams[3+(idChnl-1)*3];
			  mfiFile << m << " " << h << " " << s << endl;
			}
		  }
		}
		else if (ADEniMenu->Checked)
		{
		  for (int n = 1; n <= nMax; n++)
		  {
			vector<double> const solParams = MDMed->getAutoChnlSol(n);
			double const c0 = solParams[0];
			mfiFile << c0 << endl;
			for (unsigned int i = 1; i < solParams.size(); i+=4)
			{
			  int idChnl = 1 + (i-1)/4;
			  double f = solParams[1+(idChnl-1)*4];
			  double h = solParams[2+(idChnl-1)*4];
			  double s = solParams[3+(idChnl-1)*4];
			  double g = solParams[4+(idChnl-1)*4];
			  mfiFile << f << " " << h << " " << s << " " << g << endl;
			}
		  }
		}
		else if (SFDMmenu->Checked)
		{
		  for (int n = 1; n <= nMax; n++)
		  {
			vector<double> const solParams = MDP_SFDM->getAutoChnlSol(n);
			double const qt = solParams[0];
			mfiFile << qt << endl;
			for (unsigned int i = 1; i < solParams.size(); i+=4)
			{
			  int idChnl = 1 + (i-1)/4;
			  double m = solParams[1+(idChnl-1)*4];
			  double b = solParams[2+(idChnl-1)*4];
			  double h = solParams[3+(idChnl-1)*4];
			  double s = solParams[4+(idChnl-1)*4];
			  mfiFile << m << " " << b << " " << h << " " << s << endl;
			}
		  }
		}
		else if (TwoRNEmenu->Checked)
		{
		  for (int n = 1; n <= nMax; n++)
		  {
			vector<double> const solParams = MDP_2RNE->getAutoChnlSol(n);
			double const qt = solParams[0];
			mfiFile << qt << endl;
			for (unsigned int i = 1; i < solParams.size(); i+=6)
			{
			  int idChnl = 1 + (i-1)/6;
			  double m = solParams[1+(idChnl-1)*6];
			  double x = solParams[2+(idChnl-1)*6];
			  double h = solParams[3+(idChnl-1)*6];
			  double s = solParams[4+(idChnl-1)*6];
			  double p = solParams[5+(idChnl-1)*6];
			  double o = solParams[6+(idChnl-1)*6];
			  mfiFile << m << " " << x << " " << h << " " << s << " " << p << " " << o << endl;
			}
		  }
		}
	  }
	  else mfiFile << "-1" << endl; // no AutoChnl optimization data
	  m_UnsavedChanges = false;
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::FileOpenMenuClick(TObject *Sender)
{
  if (OpenDialog1->Execute())
  {
	ifstream mfiFile(OpenDialog1->FileName.c_str()); // Opening the MFIT file for reading
	if (mfiFile)
	{
	  Caption = "MFIT";
	  Caption = Caption + " - " + ExtractFileName(OpenDialog1->FileName);
	  TCobs_Form->clearVectUserObs();
	  TCobs_Form->TCWgrid->RowCount = 2;
	  TCobs_Form->TCWgrid->Rows[1]->Clear();
	  TCobs_Form->TCWgrid->Cells[0][1] = "1";
	  ObservationsMenu->Enabled = true;
	  MDMi->resetForm();
	  MDMed->resetForm();
	  MDP_SFDM->resetForm();
	  MDP_2RNE->resetForm();
	  ADEiMenu->Checked = false;
	  ADEniMenu->Checked = false;
	  SFDMmenu->Checked = false;
	  TwoRNEmenu->Checked = false;
	  BTCcalcMenu->Enabled = false;
	  TM_Menu->Enabled = true;
	  TC_Chart->Series[0]->Clear();
	  TC_Chart->Series[1]->Clear();
	  PestParamMenu->Enabled = false;
	  PestUserChnls->Checked = false;
	  PestAutoChnls->Checked = false;
	  PestADEiUserChnls->resetPestParamGrid();
	  PestADEiAutoChnls->resetPestParamGrid();
	  PestADEiAutoChnls->clearPestParams();
	  PestADEniUserChnls->resetPestParamGrid();
	  PestADEniAutoChnls->resetPestParamGrid();
	  PestADEniAutoChnls->clearPestParams();
	  PestSFDMuserChnls->resetPestParamGrid();
	  PestSFDMautoChnls->resetPestParamGrid();
	  PestSFDMautoChnls->clearPestParams();
	  Pest2RNEuserChnls->resetPestParamGrid();
	  Pest2RNEautoChnls->resetPestParamGrid();
	  Pest2RNEautoChnls->clearPestParams();
	  PestCreateDatasetsMenu->Enabled = false;
	  RunPestChekMenu->Enabled = false;
	  RunPestMenu->Enabled = false;
	  CalUncertMenu->Enabled = false;
	  PhiN_SplitViewPannel->Close();
	  PhiN_Chart->Series[0]->Clear();
	  PhiN_Chart->Series[1]->Clear();

	  // Observations
	  string line;
	  getline(mfiFile, line); // Headline
	  int noUserObs;
	  mfiFile >> noUserObs;
	  if (noUserObs > 0)
	  {
		TCobs_Form->TCWgrid->RowCount = noUserObs + 1;
		vector<double> userObsTCW (3,0); // Time, Concentration, Weight, initialisés à [0,0,0]
		for (int i = 0; i < noUserObs; i++)
		{
		  mfiFile >> userObsTCW[0] >> userObsTCW[1] >> userObsTCW[2];
		  TCobs_Form->addUserObs(userObsTCW);
		  TCobs_Form->TCWgrid->Cells[0][i+1] = i+1;
		  TCobs_Form->TCWgrid->Cells[1][i+1] = userObsTCW[0];
		  TCobs_Form->TCWgrid->Cells[2][i+1] = userObsTCW[1];
		  TCobs_Form->TCWgrid->Cells[3][i+1] = userObsTCW[2];
		  TC_Chart->Series[0]->AddXY(userObsTCW[0],userObsTCW[1]);
		}
		TCobs_Form->checkUserObsMinMax();
		TC_Chart->Visible = true;
	  }

	  // Working directory path
	  string str1, str2, str3;
	  mfiFile >> str1 >> str2 >> str3;
	  AnsiString wrkDirPth = AnsiString(str3.c_str());
	  WrkDir_Form->setWrkDirPth(wrkDirPth);

	  // Transport model parameters
	  mfiFile >> str1 >> str2;
	  int modelFlag, noTimeSteps, n, noParamPerChnl;
	  double tSimMin, tSimMax, qt, c0;
	  mfiFile >> modelFlag;
	  if (modelFlag == 0) // MDMi
	  {
		ADEiMenu->Checked = true;
		mfiFile >> tSimMin >> tSimMax >> noTimeSteps >> qt;
		MDMi->setTsimMin(tSimMin);
		MDMi->setTsimMax(tSimMax);
		MDMi->setNoTimeSteps(noTimeSteps);
		MDMi->setQT(qt);
		mfiFile >> n;
		if (n > 0)
		{
		  MDMi->UpDown1->Position = n;
		  MDMi->ChnlParamGrid->RowCount = 1 + n;
		  mfiFile >> noParamPerChnl;
		  vector<double> chnlParams (noParamPerChnl,0); // a single channel, initialized as [0,0,...,0]
		  for (int idChnl = 1; idChnl <= n; idChnl++)
		  {
			MDMi->ChnlParamGrid->Cells[0][idChnl] = idChnl;
			for (int k = 0; k < noParamPerChnl; k++)
			{
			  mfiFile >> chnlParams[k];
			  MDMi->ChnlParamGrid->Cells[k+1][idChnl] = chnlParams[k];
			}
			MDMi->addChnl(chnlParams);
		  }
		}
	  }
	  else if (modelFlag == 1) // MDMed
	  {
		ADEniMenu->Checked = true;
		mfiFile >> tSimMin >> tSimMax >> noTimeSteps >> c0;
		MDMed->setTsimMin(tSimMin);
		MDMed->setTsimMax(tSimMax);
		MDMed->setNoTimeSteps(noTimeSteps);
		MDMed->setC0(c0);
		mfiFile >> n;
		if (n > 0)
		{
		  MDMed->UpDown1->Position = n;
		  MDMed->ChnlParamGrid->RowCount = 1 + n;
		  mfiFile >> noParamPerChnl;
		  vector<double> chnlParams (noParamPerChnl,0);
		  for (int idChnl = 1; idChnl <= n; idChnl++)
		  {
			MDMed->ChnlParamGrid->Cells[0][idChnl] = idChnl;
			for (int k = 0; k < noParamPerChnl; k++)
			{
			  mfiFile >> chnlParams[k];
			  MDMed->ChnlParamGrid->Cells[k+1][idChnl] = chnlParams[k];
			}
			MDMed->addChnl(chnlParams);
		  }
		}
	  }
	  else if (modelFlag == 2) // MDP_SFDM
	  {
		SFDMmenu->Checked = true;
		mfiFile >> tSimMin >> tSimMax >> noTimeSteps >> qt;
		MDP_SFDM->setTsimMin(tSimMin);
		MDP_SFDM->setTsimMax(tSimMax);
		MDP_SFDM->setNoTimeSteps(noTimeSteps);
		MDP_SFDM->setQT(qt);
		mfiFile >> n;
		if (n > 0)
		{
		  MDP_SFDM->UpDown1->Position = n;
		  MDP_SFDM->ChnlParamGrid->RowCount = 1 + n;
		  mfiFile >> noParamPerChnl;
		  vector<double> chnlParams (noParamPerChnl,0);
		  for (int idChnl = 1; idChnl <= n; idChnl++)
		  {
			MDP_SFDM->ChnlParamGrid->Cells[0][idChnl] = idChnl;
			for (int k = 0; k < noParamPerChnl; k++)
			{
			  mfiFile >> chnlParams[k];
			  MDP_SFDM->ChnlParamGrid->Cells[k+1][idChnl] = chnlParams[k];
			}
			MDP_SFDM->addChnl(chnlParams);
		  }
		}
	  }
	  else if (modelFlag == 3) // MDP_2RNE
	  {
		TwoRNEmenu->Checked = true;
		mfiFile >> tSimMin >> tSimMax >> noTimeSteps >> qt;
		MDP_2RNE->setTsimMin(tSimMin);
		MDP_2RNE->setTsimMax(tSimMax);
		MDP_2RNE->setNoTimeSteps(noTimeSteps);
		MDP_2RNE->setQT(qt);
		mfiFile >> n;
		if (n > 0)
		{
		  MDP_2RNE->UpDown1->Position = n;
		  MDP_2RNE->ChnlParamGrid->RowCount = 1 + n;
		  mfiFile >> noParamPerChnl;
		  vector<double> chnlParams (noParamPerChnl,0);
		  for (int idChnl = 1; idChnl <= n; idChnl++)
		  {
			MDP_2RNE->ChnlParamGrid->Cells[0][idChnl] = idChnl;
			for (int k = 0; k < noParamPerChnl; k++)
			{
			  mfiFile >> chnlParams[k];
			  MDP_2RNE->ChnlParamGrid->Cells[k+1][idChnl] = chnlParams[k];
			}
			MDP_2RNE->addChnl(chnlParams);
		  }
		}
	  }

	  // Pest general options
	  mfiFile >> str1 >> str1;
	  bool pestUserApprovalFlag;
	  mfiFile >> pestUserApprovalFlag;
	  if (pestUserApprovalFlag)
	  {
		PestGeneral->setUserApprovalFlag(true);
		double rlamfac;
		mfiFile >> rlamfac;
		PestGeneral->setRLAMFAC(rlamfac);
		PestGeneral->RLAMFAC_Edit->Text = rlamfac;
		bool extDrvOption;
		mfiFile >> extDrvOption;
		PestGeneral->setEXTDRVOption(extDrvOption);
		PestGeneral->EXTDRV_CheckBox->Checked = extDrvOption;
		bool boundscaleOption;
		mfiFile >> boundscaleOption;
		PestGeneral->setBOUNDSCALEOption(boundscaleOption);
		PestGeneral->BOUNDSCALE_CheckBox->Checked = boundscaleOption;
		bool svdOption;
		mfiFile >> svdOption;
		PestGeneral->setSVDOption(svdOption);
		PestGeneral->SVD_CheckBox->Checked = svdOption;
		if (PestGeneral->SVD_CheckBox->Checked)
		{
		  PestGeneral->BOUNDSCALE_CheckBox->Enabled = true;
		}
		else
		{
		  PestGeneral->BOUNDSCALE_CheckBox->Enabled = false;
		}
		int noptmax;
		mfiFile >> noptmax;
		PestGeneral->setNOPTMAX(noptmax);
		PestGeneral->NOPTMAX_Edit->Text = noptmax;
		double phiredstp;
		mfiFile >> phiredstp;
		PestGeneral->setPHIREDSTP(phiredstp);
		PestGeneral->PHIREDSTP_Edit->Text = FloatToStrF(phiredstp, ffExponent, 2, 2);
		int nphistp;
		mfiFile >> nphistp;
		PestGeneral->setNPHISTP(nphistp);
		PestGeneral->NPHISTP_Edit->Text = nphistp;
		int nphinored;
		mfiFile >> nphinored;
		PestGeneral->setNPHINORED(nphinored);
		PestGeneral->NPHINORED_Edit->Text = nphinored;
		double relparstp;
		mfiFile >> relparstp;
		PestGeneral->setRELPARSTP(relparstp);
		PestGeneral->RELPARSTP_Edit->Text = FloatToStrF(relparstp, ffExponent, 2, 2);
		int nrelpar;
		mfiFile >> nrelpar;
		PestGeneral->setNRELPAR(nrelpar);
		PestGeneral->NRELPAR_Edit->Text = nrelpar;
		bool tikRegOption;
		mfiFile >> tikRegOption;
		PestGeneral->setTikRegOption(tikRegOption);
		PestGeneral->TikReg_CheckBox->Checked = tikRegOption;
		if (PestGeneral->TikReg_CheckBox->Checked)
		{
		  PestGeneral->TikRegType_Label->Enabled = true;
		  PestGeneral->TikRegType_CBox->Enabled = true;
		  PestGeneral->PHIMLIM_Label->Enabled = true;
		  PestGeneral->PHIMLIM_Edit->Enabled = true;
		  PestGeneral->PHIMACCEPT_Label->Enabled = true;
		  PestGeneral->PHIMACCEPT_Edit->Enabled = true;
		  PestGeneral->IREGADJ_Label->Enabled = true;
		  PestGeneral->IREGADJ_Edit->Enabled = true;
		}
		else
		{
		  PestGeneral->TikRegType_Label->Enabled = false;
		  PestGeneral->TikRegType_CBox->Enabled = false;
		  PestGeneral->PHIMLIM_Label->Enabled = false;
		  PestGeneral->PHIMLIM_Edit->Enabled = false;
		  PestGeneral->PHIMACCEPT_Label->Enabled = false;
		  PestGeneral->PHIMACCEPT_Edit->Enabled = false;
		  PestGeneral->IREGADJ_Label->Enabled = false;
		  PestGeneral->IREGADJ_Edit->Enabled = false;
		}
		int tikRegType;
		mfiFile >> tikRegType;
		PestGeneral->setTikRegType(tikRegType);
		if (tikRegType == 2) PestGeneral->TikRegType_CBox->ItemIndex = 1;
		else PestGeneral->TikRegType_CBox->ItemIndex = 0;
		double phimlim;
		mfiFile >> phimlim;
		PestGeneral->setPHIMLIM(phimlim);
		PestGeneral->PHIMLIM_Edit->Text = phimlim;
		double phimaccept;
		mfiFile >> phimaccept;
		PestGeneral->setPHIMACCEPT(phimaccept);
		PestGeneral->PHIMACCEPT_Edit->Text = phimaccept;
		int iregadj;
		mfiFile >> iregadj;
		PestGeneral->setIREGADJ(iregadj);
		PestGeneral->IREGADJ_Edit->Text = iregadj;
		int nRandSampl;
		mfiFile >> nRandSampl;
		PestGeneral->setNrandSampl(nRandSampl);
		PestGeneral->nRandSampl_Edit->Text = nRandSampl;
	  }

	  // Pest parameters - channels
	  mfiFile >> str1 >> str1 >> str1 >> str1;
	  int flagAutoChnls;
	  mfiFile >> flagAutoChnls;
	  if (flagAutoChnls == 0) // "UserChannels" option (number of channels fixed by the user)
	  {
		int noPestParameters;
		mfiFile >> noPestParameters;
		if (noPestParameters > 0)
		{
		  vector< vector<double> > vectPestParams;
		  vector<double> vTTUMMT (6,0); // A Pest parameter for "UserChannel" optimization: Type, Channel No., Use (No=0/Yes=1), Minimum, Maximum, Transform, initialized as [0,0,0,0,0,0]
		  for (int idPestParam = 1; idPestParam <= noPestParameters; idPestParam++)
		  {
			mfiFile >> vTTUMMT[0] >> vTTUMMT[1] >> vTTUMMT[2] >> vTTUMMT[3] >> vTTUMMT[4] >> vTTUMMT[5];
			vectPestParams.push_back(vTTUMMT);
			if (modelFlag == 0) PestADEiUserChnls->setPestGridParam(idPestParam, vTTUMMT);
			else if (modelFlag == 1) PestADEniUserChnls->setPestGridParam(idPestParam, vTTUMMT);
			else if (modelFlag == 2) PestSFDMuserChnls->setPestGridParam(idPestParam, vTTUMMT);
			else if (modelFlag == 3) Pest2RNEuserChnls->setPestGridParam(idPestParam, vTTUMMT);
		  }
		  if (modelFlag == 0) PestADEiUserChnls->setPestParams(vectPestParams);
		  else if (modelFlag == 1) PestADEniUserChnls->setPestParams(vectPestParams);
		  else if (modelFlag == 2) PestSFDMuserChnls->setPestParams(vectPestParams);
		  else if (modelFlag == 3) Pest2RNEuserChnls->setPestParams(vectPestParams);
		  PestUserChnls->Checked = true;
		}
	  }
	  else if (flagAutoChnls == 1) // "AutoChannels" option
	  {
		int flagMICP, noModelParams, nMax;
		mfiFile >> flagMICP;
		vector<double> vTEMMT (5,0); // A pest parameter for "AutoChannels" optimization: Type, Estimate (No=0/Yes=1), Minimum, Maximum, Transform, initialized as [0,0,0,0,0]
		if (modelFlag == 0) // ADEinst
		{
		  mfiFile >> noModelParams;
		  if (noModelParams == 4) // qt,m,h,s
		  {
			mfiFile >> nMax;
			PestADEiAutoChnls->UpDown->Position = nMax;
			for (int row=1; row <1+noModelParams; row++)
			{
			  mfiFile >> vTEMMT[0] >> vTEMMT[1] >> vTEMMT[2] >> vTEMMT[3] >> vTEMMT[4];
			  PestADEiAutoChnls->addParam(vTEMMT);
			  PestADEiAutoChnls->ParamGrid->Cells[1][row] = vTEMMT[1];
			  PestADEiAutoChnls->ParamGrid->Cells[2][row] = FloatToStrF(vTEMMT[2], ffExponent, 3, 2);
			  PestADEiAutoChnls->ParamGrid->Cells[3][row] = FloatToStrF(vTEMMT[3], ffExponent, 3, 2);
			  if (vTEMMT[4] == 0) PestADEiAutoChnls->ParamGrid->Cells[4][row] = "None";
			  else if (vTEMMT[4] == 1) PestADEiAutoChnls->ParamGrid->Cells[4][row] = "Log";
			}
			PestAutoChnls->Checked = true;
			if (flagMICP == 1) PestADEiAutoChnls->MICP_CheckBox->Checked = true;
			else PestADEiAutoChnls->MICP_CheckBox->Checked = false;
		  }
		}
		else if (modelFlag == 1) // ADE_NonInst
		{
		  mfiFile >> noModelParams;
		  if (noModelParams == 5) // c0,f,h,s,g
		  {
			mfiFile >> nMax;
			PestADEniAutoChnls->UpDown->Position = nMax;
			for (int row=1; row <1+noModelParams; row++)
			{
			  mfiFile >> vTEMMT[0] >> vTEMMT[1] >> vTEMMT[2] >> vTEMMT[3] >> vTEMMT[4];
			  PestADEniAutoChnls->addParam(vTEMMT);
			  PestADEniAutoChnls->ParamGrid->Cells[1][row] = vTEMMT[1];
			  PestADEniAutoChnls->ParamGrid->Cells[2][row] = FloatToStrF(vTEMMT[2], ffExponent, 3, 2);
			  PestADEniAutoChnls->ParamGrid->Cells[3][row] = FloatToStrF(vTEMMT[3], ffExponent, 3, 2);
			  if (vTEMMT[4] == 0) PestADEniAutoChnls->ParamGrid->Cells[4][row] = "None";
			  else if (vTEMMT[4] == 1) PestADEniAutoChnls->ParamGrid->Cells[4][row] = "Log";
			}
			PestAutoChnls->Checked = true;
			if (flagMICP == 1) PestADEniAutoChnls->MICP_CheckBox->Checked = true;
			else PestADEniAutoChnls->MICP_CheckBox->Checked = false;
		  }
		}
		else if (modelFlag == 2) // SFDM
		{
		  mfiFile >> noModelParams;
		  if (noModelParams == 5) // qt,m,b,h,s
		  {
			mfiFile >> nMax;
			PestSFDMautoChnls->UpDown->Position = nMax;
			for (int row=1; row <1+noModelParams; row++)
			{
			  mfiFile >> vTEMMT[0] >> vTEMMT[1] >> vTEMMT[2] >> vTEMMT[3] >> vTEMMT[4];
			  PestSFDMautoChnls->addParam(vTEMMT);
			  PestSFDMautoChnls->ParamGrid->Cells[1][row] = vTEMMT[1];
			  PestSFDMautoChnls->ParamGrid->Cells[2][row] = FloatToStrF(vTEMMT[2], ffExponent, 3, 2);
			  PestSFDMautoChnls->ParamGrid->Cells[3][row] = FloatToStrF(vTEMMT[3], ffExponent, 3, 2);
			  if (vTEMMT[4] == 0) PestSFDMautoChnls->ParamGrid->Cells[4][row] = "None";
			  else if (vTEMMT[4] == 1) PestSFDMautoChnls->ParamGrid->Cells[4][row] = "Log";
			}
			PestAutoChnls->Checked = true;
			if (flagMICP == 1) PestSFDMautoChnls->MICP_CheckBox->Checked = true;
			else PestSFDMautoChnls->MICP_CheckBox->Checked = false;
		  }
		}
		else if (modelFlag == 3) // 2RNE
		{
		  mfiFile >> noModelParams;
		  if (noModelParams == 7) // qt,m,x,h,s,p,o
		  {
			mfiFile >> nMax;
			Pest2RNEautoChnls->UpDown->Position = nMax;
			for (int row=1; row <1+noModelParams; row++)
			{
			  mfiFile >> vTEMMT[0] >> vTEMMT[1] >> vTEMMT[2] >> vTEMMT[3] >> vTEMMT[4];
			  Pest2RNEautoChnls->addParam(vTEMMT);
			  Pest2RNEautoChnls->ParamGrid->Cells[1][row] = vTEMMT[1];
			  Pest2RNEautoChnls->ParamGrid->Cells[2][row] = FloatToStrF(vTEMMT[2], ffExponent, 3, 2);
			  Pest2RNEautoChnls->ParamGrid->Cells[3][row] = FloatToStrF(vTEMMT[3], ffExponent, 3, 2);
			  if (vTEMMT[4] == 0) Pest2RNEautoChnls->ParamGrid->Cells[4][row] = "None";
			  else if (vTEMMT[4] == 1) Pest2RNEautoChnls->ParamGrid->Cells[4][row] = "Log";
			}
			PestAutoChnls->Checked = true;
			if (flagMICP == 1) Pest2RNEautoChnls->MICP_CheckBox->Checked = true;
			else Pest2RNEautoChnls->MICP_CheckBox->Checked = false;
		  }
		}
	  }

	  // AutoChnl optimization results
	  mfiFile >> str1 >> str1 >> str1;
	  int nMax;
	  mfiFile >> nMax;
	  if (nMax > 1) // available AutoChnl optimization data
	  {
		// BTC solutions for 1 to Nmax channels
		vector<double> vTC (2,0); // time, concentration (initialized as [0,0])
		for (int n = 1; n <= nMax; n++)
		{
		   m_modelBTC.clear();
		   for (int i = 0; i < noTimeSteps; i++)
		   {
			 mfiFile >> vTC[0] >> vTC[1];
			 m_modelBTC.push_back(vTC);
		   }
		   if (n==1) init_AutoChnlSolBTCs(nMax);
		   set_AutoChnlSolBTC(n, m_modelBTC);
		}
		// plot by default the n=1 channel BTC solution
		plot_modelBTC(get_AutoChnlSolBTC(1));
		// Phi(N) curve
		double phiN;
		double phiMax(-numeric_limits<double>::max()); // or -DBL_MAX
		double phiMin(numeric_limits<double>::max()); // or DBL_MAX
		for (int n = 1; n <= nMax; n++)
		{
		  mfiFile >> phiN;
		  if (phiN < phiMin) phiMin = phiN;
		  if (phiN > phiMax) phiMax = phiN;
		  PhiN_Chart->Series[0]->AddXY(n,phiN);
		}
		PhiN_Chart->Series[1]->AddXY(1,PhiN_Chart->Series[0]->YValues->Value[0]);
		PhiN_Chart->LeftAxis->Minimum = pow(10,floor(log10(phiMin)));
		PhiN_Chart->LeftAxis->Maximum = pow(10,1+floor(log10(phiMax)));
		nUpDown->Max = nMax;
		nUpDown->Position = 1;
		PhiN_SplitViewPannel->Visible = true;
		PhiN_SplitViewPannel->Open();
		// AutoChnl solution parameters
		double qt, m, h, s, c0, f, g, b, x, p, o;
		if (modelFlag == 0) // ADE Inst
		{
		  MDMi->initAutoChnlSols(nMax);
		  for (int n = 1; n <= nMax; n++)
		  {
			mfiFile >> qt;
			MDMi->setAutoChnlSolParam(n, 0, 0, qt);
			for (int idChnl = 1; idChnl <= n; idChnl++)
			{
			  mfiFile >> m >> h >> s;
			  MDMi->setAutoChnlSolParam(n, 1, idChnl, m);
			  MDMi->setAutoChnlSolParam(n, 2, idChnl, h);
			  MDMi->setAutoChnlSolParam(n, 3, idChnl, s);
			}
		  }
		}
		else if (modelFlag == 1) // ADE NonInst
		{
		  MDMed->initAutoChnlSols(nMax);
		  for (int n = 1; n <= nMax; n++)
		  {
			mfiFile >> c0;
			MDMed->setAutoChnlSolParam(n, 0, 0, c0);
			for (int idChnl = 1; idChnl <= n; idChnl++)
			{
			  mfiFile >> f >> h >> s >> g;
			  MDMed->setAutoChnlSolParam(n, 1, idChnl, f);
			  MDMed->setAutoChnlSolParam(n, 2, idChnl, h);
			  MDMed->setAutoChnlSolParam(n, 3, idChnl, s);
			  MDMed->setAutoChnlSolParam(n, 4, idChnl, g);
			}
		  }
		}
		else if (modelFlag == 2) // SFDM
		{
		  MDP_SFDM->initAutoChnlSols(nMax);
		  for (int n = 1; n <= nMax; n++)
		  {
			mfiFile >> qt;
			MDP_SFDM->setAutoChnlSolParam(n, 0, 0, qt);
			for (int idChnl = 1; idChnl <= n; idChnl++)
			{
			  mfiFile >> m >> b >> h >> s;
			  MDP_SFDM->setAutoChnlSolParam(n, 1, idChnl, m);
			  MDP_SFDM->setAutoChnlSolParam(n, 2, idChnl, b);
			  MDP_SFDM->setAutoChnlSolParam(n, 3, idChnl, h);
			  MDP_SFDM->setAutoChnlSolParam(n, 4, idChnl, s);
			}
		  }
		}
		else if (modelFlag == 3) // 2RNE
		{
		  MDP_2RNE->initAutoChnlSols(nMax);
		  for (int n = 1; n <= nMax; n++)
		  {
			mfiFile >> qt;
			MDP_2RNE->setAutoChnlSolParam(n, 0, 0, qt);
			for (int idChnl = 1; idChnl <= n; idChnl++)
			{
			  mfiFile >> m >> x >> h >> s >> p >> o;
			  MDP_2RNE->setAutoChnlSolParam(n, 1, idChnl, m);
			  MDP_2RNE->setAutoChnlSolParam(n, 2, idChnl, x);
			  MDP_2RNE->setAutoChnlSolParam(n, 3, idChnl, h);
			  MDP_2RNE->setAutoChnlSolParam(n, 4, idChnl, s);
			  MDP_2RNE->setAutoChnlSolParam(n, 5, idChnl, p);
			  MDP_2RNE->setAutoChnlSolParam(n, 6, idChnl, o);
			}
		  }
		}
	  }

	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::FileExitMenuClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::PestAbortMenuClick(TObject *Sender)
{
  PestAbortSpeedButton->Hint = "Aborting";
  ofstream pestStopFile("pest.stp", ios::out | ios::trunc);
  pestStopFile << 1 << endl;
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::FormCreate(TObject *Sender)
{
  PhiN_SplitViewPannel->Close();
  m_UnsavedChanges = false;
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::nUpDownClick(TObject *Sender, TUDBtnType Button)
{
  PhiN_Chart->Series[1]->Clear();
  int const n(nUpDown->Position);
  double const newHighlightedX = PhiN_Chart->Series[0]->XValues->Value[n - 1];
  double const newHighlightedY = PhiN_Chart->Series[0]->YValues->Value[n - 1];
  PhiN_Chart->Series[1]->AddXY(newHighlightedX,newHighlightedY);
  plot_modelBTC(get_AutoChnlSolBTC(n));
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::PhiN_CancelClick(TObject *Sender)
{
  PhiN_SplitViewPannel->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::PhiN_OKClick(TObject *Sender)
{
  int const n(nUpDown->Position);
  if (ADEiMenu->Checked)
  {
	MDMi->setParamGridFromSol(n);
	MDMi->Show();
  }
  else if (ADEniMenu->Checked)
  {
	MDMed->setParamGridFromSol(n);
	MDMed->Show();
  }
  else if (SFDMmenu->Checked)
  {
	MDP_SFDM->setParamGridFromSol(n);
	MDP_SFDM->Show();
  }
  else if (TwoRNEmenu->Checked)
  {
	MDP_2RNE->setParamGridFromSol(n);
	MDP_2RNE->Show();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::OpenPhiN_SplitViewPannelClick(TObject *Sender)
{
  PhiN_SplitViewPannel->Visible = true;
  PhiN_SplitViewPannel->Open();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::ClosePhiN_SplitViewPannelClick(TObject *Sender)
{
  PhiN_SplitViewPannel->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::notifyChanges()
{
  m_UnsavedChanges = true;
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  if (m_UnsavedChanges == true)
  {
	int returnVal = MessageDlg("Save changes?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel,0);
	if (returnVal == mrYes)
	{
	  FileSaveAsMenuClick(Sender);
	}
	else if (returnVal == mrCancel)
	{
	  CanClose = false;
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::FileNewMenuClick(TObject *Sender)
{
  bool reset = true;
  if (m_UnsavedChanges == true)
  {
	int returnVal = MessageDlg("Save changes?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel,0);
	if (returnVal == mrYes)
	{
	  reset = false;
	  FileSaveAsMenuClick(Sender);
	}
	else if (returnVal == mrCancel)
	{
	  reset = false;
	}
  }
  if (reset)
  {
	Caption = "MFIT";
	TC_Chart->Series[0]->Clear();
	TC_Chart->Series[1]->Clear();
	TC_Chart->Visible = false;
	PhiN_Chart->Series[0]->Clear();
	PhiN_Chart->Series[1]->Clear();
	PhiN_SplitViewPannel->Close();
	TCobs_Form->clearVectUserObs();
	TCobs_Form->TCWgrid->RowCount = 2;
	TCobs_Form->TCWgrid->Rows[1]->Clear();
	TCobs_Form->TCWgrid->Cells[0][1] = "1";
	ObservationsMenu->Enabled = true;
	MDMi->resetForm();
	MDMed->resetForm();
	MDP_SFDM->resetForm();
	MDP_2RNE->resetForm();
	ADEiMenu->Checked = false;
	ADEniMenu->Checked = false;
	SFDMmenu->Checked = false;
	TwoRNEmenu->Checked = false;
	BTCcalcMenu->Enabled = false;
	TM_Menu->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Form::CalUncertMenuClick(TObject *Sender)
{
  int nRandSets = PestGeneral->getNrandSampl();

  string line, someText, PARNME, PARTRANS, PARCHGLIM, PARGP;
  int NOBS, NPAR, DERCOM;
  double PARVAL1, PARLBND, PARUBND, SCALE, OFFSET, stdDev, phiRef, phi, PARVALOPT, PARVALRAND;
  double PHISTOPTHRESH;
  double obsRefVar; // Observation reference variance

  PestAbortMenu->Enabled = true;
  PestAbortSpeedButton->Visible = true;
  StatusBar->Panels->Items[0]->Text = "Post-calibration analysis in progress, please wait...";
  ActivityIndicator1->Animate = true;
  PostCalHist_Form->Param_CBox->Items->Clear();

  //--- Prior parameter uncertainty file ---
  ofstream priorParUncertFile("priorParUncert.txt", ios::out | ios::trunc);
  priorParUncertFile << "# Prior parameter uncertainty file" << endl << endl;
  priorParUncertFile << "START STANDARD_DEVIATION" << endl;
  ifstream pestRunRecordFile("mfit.rec", ios::in); // Open the PEST Run Record file for reading
  if (pestRunRecordFile) // if opening sucess
  {
	do getline(pestRunRecordFile, line); while (line != "Case dimensions:-");
	getline(pestRunRecordFile, line);
	for (int i = 0; i < 4; i++) pestRunRecordFile >> someText;
	pestRunRecordFile >> NPAR;
	for (int i = 0; i < 3; i++) getline(pestRunRecordFile, line);
	for (int i = 0; i < 4; i++) pestRunRecordFile >> someText;
	pestRunRecordFile >> NOBS;
	do getline(pestRunRecordFile, line); while (line != "Parameter definitions:-");
	for (int i = 0; i < 3; i++) getline(pestRunRecordFile, line);
	for (int i = 0; i < NPAR; i++)
	{
	  pestRunRecordFile >> PARNME >> PARTRANS >> PARCHGLIM >> PARVAL1 >> PARLBND >> PARUBND;
	  if (PARTRANS == "none")
	  {
		stdDev = (PARUBND-PARLBND)/double(4); //*** see Pest manual II, p81
	  }
	  else // PARTRANS == "log"
	  {
		stdDev = (log10(PARUBND)-log10(PARLBND))/double(4); //*** see Pest manual II, p81
	  }
	  priorParUncertFile << "  " << PARNME << " " << stdDev << endl;
	  PostCalHist_Form->Param_CBox->Items->Add(PARNME.c_str());
	}
	PostCalHist_Form->Param_CBox->ItemIndex = 0;
	do getline(pestRunRecordFile, line); while (line != "Objective function ----->");
	getline(pestRunRecordFile, line);
	for (int i = 0; i < 8; i++) pestRunRecordFile >> someText;
	pestRunRecordFile >> phiRef;
	obsRefVar = phiRef/double(NOBS);
	pestRunRecordFile.close();
  }
  priorParUncertFile << "END STANDARD_DEVIATION" << endl;
  priorParUncertFile.close();

  //--- PREDUNC7 run: computing the covariance matrix pertaining to the posterior parameter probability distribution
  ofstream pred7instrFile("pred7instr.txt", ios::out | ios::trunc);
  pred7instrFile << "mfit.pst" << endl;
  pred7instrFile << obsRefVar << endl;
  pred7instrFile << "priorParUncert.txt" << endl;
  pred7instrFile << "postParCovMat.txt" << endl;
  pred7instrFile << "postParUncert.txt" << endl;
  pred7instrFile << 1 << endl;
  pred7instrFile.close();
  system("PREDUNC7 < pred7instr.txt");

  //--- New PEST control file with updated (optimized) initial parameter values
  ifstream pestControlFile("mfit.pst", ios::in); // Open the PEST control file for reading
  ifstream optParamFile("mfit.par", ios::in); // Open the PEST-optimized parameter file for reading
  ofstream postControlFile("postmfit.pst", ios::out | ios::trunc);
  if ((pestControlFile)&&(optParamFile))
  {
	do
	{
	  getline(pestControlFile, line);
	  postControlFile << line << endl;
	}
	while (line != "* parameter data");
	getline(optParamFile, line);
	for (int i = 0; i < NPAR; i++)
	{
	  pestControlFile >> PARNME >> PARTRANS >> PARCHGLIM >> PARVAL1 >> PARLBND;
	  pestControlFile >> PARUBND >> PARGP >> SCALE >> OFFSET >> DERCOM;
	  optParamFile >> PARNME >> PARVALOPT >> someText >> someText;
	  postControlFile << PARNME << "  " << PARTRANS << "  " << PARCHGLIM << "  ";
	  postControlFile << PARVALOPT << "  " << PARLBND << "  " << PARUBND << "  ";
	  postControlFile << PARGP << "  " << SCALE << "  " << OFFSET << "  " << DERCOM << endl;
	}
	getline(pestControlFile, line);
	while (getline(pestControlFile, line)) postControlFile << line << endl;
	pestControlFile.close();
	optParamFile.close();
  }
  postControlFile.close();

  //--- RANDPAR run: sampling the posterior parameter covariance matrix
  ofstream randparInstrFile("randparInstr.txt", ios::out | ios::trunc);
  randparInstrFile << "postmfit.pst" << endl;
  randparInstrFile << "n" << endl << "e" << endl << "y" << endl;
  randparInstrFile << "postParUncert.txt" << endl << endl;
  randparInstrFile << "uncalrand" << endl << nRandSets << endl << endl;
  randparInstrFile.close();
  system("RANDPAR < randparInstr.txt");

  //--- PEST Recalibration
  PostCalHist_Form->clearVectRecalSets();
  for (int i = 1; i <= nRandSets; i++)
  {
	//--- New PEST control file with initial parameter values based on RANDPAR files
	ifstream pestControlFile("mfit.pst", ios::in);
	string randParFileName = "uncalrand" + boost::lexical_cast<std::string>(i) + ".par";
	ifstream randParFile(randParFileName.c_str(), ios::in);
	ofstream postControlFile("postmfit.pst", ios::out | ios::trunc);
	if ((pestControlFile)&&(randParFile))
	{
	  for (int i = 0; i < 8; i++)
	  {
		getline(pestControlFile, line);
		postControlFile << line << endl;
	  }
	  getline(pestControlFile, line);
	  PHISTOPTHRESH = 1.05 * phiRef; //*** Recalibration will stop when Phi < 1.05 phiRef
	  postControlFile << line << "  " << PHISTOPTHRESH << endl;
	  do
	  {
		getline(pestControlFile, line);
		postControlFile << line << endl;
	  }
	  while (line != "* parameter data");
	  getline(randParFile, line);
	  for (int i = 0; i < NPAR; i++)
	  {
		pestControlFile >> PARNME >> PARTRANS >> PARCHGLIM >> PARVAL1 >> PARLBND;
		pestControlFile >> PARUBND >> PARGP >> SCALE >> OFFSET >> DERCOM;
		randParFile >> PARNME >> PARVALRAND >> someText >> someText;
		postControlFile << PARNME << "  " << PARTRANS << "  " << PARCHGLIM << "  ";
		postControlFile << PARVALRAND << "  " << PARLBND << "  " << PARUBND << "  ";
		postControlFile << PARGP << "  " << SCALE << "  " << OFFSET << "  " << DERCOM << endl;
	  }
	  getline(pestControlFile, line);
	  while (getline(pestControlFile, line)) postControlFile << line << endl;
	  pestControlFile.close();
	  randParFile.close();
	}
	postControlFile.close();

	//--- PEST optimization
	String cmdLine = "PEST postmfit";
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

	//--- Saving the results
	ifstream pestRunRecordFile("postmfit.rec", ios::in);
	if ((pestRunRecordFile)&&(PestAbortSpeedButton->Hint != "Aborting"))
	{
	  do
	  {
		getline(pestRunRecordFile, line);
	  }
	  while ((line != "Objective function ----->")&&(line != "Error condition prevents continued PEST execution:-"));
	  if (line == "Objective function ----->")
	  {
		getline(pestRunRecordFile, line);
		for (int i = 0; i < 8; i++) pestRunRecordFile >> someText;
		pestRunRecordFile >> phi;
		if (phi < 1.05 * phiRef)
		{
		  ifstream postOptParamFile("postmfit.par", ios::in);
		  if (postOptParamFile)
		  {
			getline(postOptParamFile, line);
			vector<double> vRecal; // a set of recalibrated parameters (+ associated phi value)
			for (int i = 0; i < NPAR; i++)
			{
			  postOptParamFile >> PARNME >> PARVALOPT >> someText >> someText;
			  vRecal.push_back(PARVALOPT);
			}
			postOptParamFile.close();
			vRecal.push_back(phi);
			PostCalHist_Form->addRecalSet(vRecal);
		  }
		}
	  }
	  pestRunRecordFile.close();
	}
	ProgressBar1->Position = 100*i/double(nRandSets);
	if (PestAbortSpeedButton->Hint == "Aborting")
	{
	  ErrorMessage->showMessage("Optimization stopped by the user");
	  break;
	}
  }

  int noRecalSets = PostCalHist_Form->getNoRecalSets();
  if ((PestAbortSpeedButton->Hint != "Aborting")&&(noRecalSets>0))
  {
	//--- Default histogram: 1st parameter
	int idParam(0);
	double minParVal = 0.99*PostCalHist_Form->getParMin(idParam);
	double maxParVal = 1.01*PostCalHist_Form->getParMax(idParam);
	int nBins(2.0*pow(noRecalSets,1.0/3.0)); // Rice rule
	double binWidth = (maxParVal-minParVal)/double(nBins);
	PostCalHist_Form->Hist_Chart->Series[0]->Clear();
	for (int i = 0; i < nBins; i++)
	{
	  double binMin = minParVal + i*binWidth;
	  double binMax = binMin + binWidth;
	  double binCenter = 0.5*(binMin + binMax);
	  int binFreq(0);
	  for (int j = 0; j < noRecalSets; j++)
	  {
		double parVal = PostCalHist_Form->getRecalParamValue(j, idParam);
		if ((parVal>=binMin)&&(parVal<binMax)) binFreq++;
	  }
	  PostCalHist_Form->Hist_Chart->Series[0]->AddXY(binCenter,binFreq);
	}
	PostCalHist_Form->Caption = "Post calibration analysis - " + PostCalHist_Form->Param_CBox->Text;
	PostCalHist_Form->Show();
  }
  StatusBar->Panels->Items[0]->Text = "";
  ActivityIndicator1->Animate = false;
  ProgressBar1->Position = 0;
  PestAbortSpeedButton->Hint = "";
  PestAbortMenu->Enabled = false;
  PestAbortSpeedButton->Visible = false;
}
//---------------------------------------------------------------------------


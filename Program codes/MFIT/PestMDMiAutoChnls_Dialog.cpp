//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PestMDMiAutoChnls_Dialog.h"
#include "PestGeneralForm.h"
#include "MainForm.h"
#include "MDMi_Dialog.h"
#include "TCobs_dialog.h"
#include "Error_MessageBox.h"
#include <fstream>
#include <string>
#include <sstream>
#include "spline.h"

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPestADEiAutoChnls *PestADEiAutoChnls;
//---------------------------------------------------------------------------
__fastcall TPestADEiAutoChnls::TPestADEiAutoChnls(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::resetPestParamGrid()
{
  ParamGrid->Cells[0][0]="Parameter";
  ParamGrid->Cells[1][0]="Estimate (No=0/Yes=1)";
  ParamGrid->Cells[2][0]="Minimum";
  ParamGrid->Cells[3][0]="Maximum";
  ParamGrid->Cells[0][1]="Total Flowrate";
  ParamGrid->Cells[0][2]="Mass";
  ParamGrid->Cells[0][3]="T0";
  ParamGrid->Cells[0][4]="Pe";
  for (int row=1; row<5; row++) // 4 parameters in the transport model: qt,m,to(h),pe(s)
  {
	ParamGrid->Cells[1][row]="1";
	ParamGrid->Cells[2][row]="1.0E-10";
	ParamGrid->Cells[3][row]="1.0E+10";
  }
  ParamGrid->Col=1;
  ParamGrid->Row=1;
  UpDown->Position = 10;
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::FormCreate(TObject *Sender)
{
  resetPestParamGrid();
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::ParamGridClick(TObject *Sender)
{
  if(ParamGrid->Col == 1) // User click in the column "Estimate"
  {
	TRect Recto = ParamGrid->CellRect(ParamGrid->Col, ParamGrid->Row);
	UseParam_CBox->Top = ParamGrid->Top;
	UseParam_CBox->Left = ParamGrid->Left;
	UseParam_CBox->Top = UseParam_CBox->Top + Recto.Top + ParamGrid->GridLineWidth;
	UseParam_CBox->Left = UseParam_CBox->Left + Recto.Left + ParamGrid->GridLineWidth + 1;
	UseParam_CBox->Height = (Recto.Bottom - Recto.Top) + 1;
	UseParam_CBox->Width = Recto.Right - Recto.Left;
	UseParam_CBox->Text = ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row];
	UseParam_CBox->Visible = true;
  }
  else
  {
	UseParam_CBox->Visible = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::UseParam_CBoxChange(TObject *Sender)
{
  ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row] = UseParam_CBox->Text;
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::clearPestParams()
{
  m_PestParams.clear();
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::addParam(std::vector<double> vTEMM)
{
  m_PestParams.push_back(vTEMM);
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::OK_ButtonClick(TObject *Sender)
{
  Main_Form->PestUserChnls->Checked = false;
  Main_Form->PestAutoChnls->Checked = true;
  Main_Form->PestCreateDatasetsMenu->Enabled = true;
  clearPestParams();

  vector<double> vTEMM (4,0); // Type, Estimate (No=0/Yes=1), Minimum, Maximum, initialized as [0,0,0,0]

  for (int row=1; row<5; row++) // 4 parameters in the transport model: qt,m,t0(h),pe(s)
  {
	vTEMM[0] = row; // parameter type numeric code (qt = 1, mass = 2, etc.)
	vTEMM[1] = ParamGrid->Cells[1][row].ToInt(); // Estimate (No=0/Yes=1)
	vTEMM[2] = ParamGrid->Cells[2][row].ToDouble(); // Minimum
	vTEMM[3] = ParamGrid->Cells[3][row].ToDouble(); // Maximum
	addParam(vTEMM);
  }
  Hide();
  Main_Form->notifyChanges();
}
//---------------------------------------------------------------------------
vector< vector<double> > __fastcall TPestADEiAutoChnls::getPestParams() const
{
  return m_PestParams;
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::BackToSavedParams()
{
  vector< vector<double> > const pestParamVector = getPestParams();
  if (pestParamVector.size()>0)
  {
	for (int row=1; row<5; row++) // 4 parameters in the transport model: qt,m,to(h),pe(s)
	{
	  ParamGrid->Cells[1][row] = pestParamVector[row-1][1];
	  ParamGrid->Cells[2][row] = FloatToStrF(pestParamVector[row-1][2], ffExponent, 3, 2);
	  ParamGrid->Cells[3][row] = FloatToStrF(pestParamVector[row-1][3], ffExponent, 3, 2);
	}
  }
  else // pestParamVector is empty
  {
	resetPestParamGrid();
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::Cancel_ButtonClick(TObject *Sender)
{
  BackToSavedParams();
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::FormClose(TObject *Sender, TCloseAction &Action)
{
  BackToSavedParams();
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::newPestTplFile(int n)
{
  if (n < 1)
  {
	ErrorMessage->ErrorMessageLabel->Caption = "Error: number of channels < 1";
	ErrorMessage->Show();
  }
  else
  {
	vector< vector<double> > const pestAutoChnlParams(getPestParams()); // a series (vector) of TEMM values (Type, Estimate (No=0/Yes=1), Minimum, Maximum)
	ofstream tplFile("MFIT.tpl", ios::out | ios::trunc);
	tplFile << "ptf #" << endl;
	tplFile << MDMi->getTsimMin() << endl;
	tplFile << MDMi->getTsimMax() << endl;
	tplFile << MDMi->getNoTimeSteps() << endl;
	//---- checking whether an optimization of qt is required -----
	bool optFlag_qt(false);
	for (unsigned int k = 0; k <pestAutoChnlParams.size(); k++)
	{
	  if ((pestAutoChnlParams[k][0] == 1) && (pestAutoChnlParams[k][1] == 1))
	  {
		optFlag_qt = true;
		tplFile << "#qt" << "                #" << endl;
		break;
	  }
	}
	if (!optFlag_qt) // User-fixed parameter (no optimization of qt)
	{
	  tplFile << MDMi->getQT() << endl;
	}
	//-------------------------------------------------------------------
	tplFile << n << endl;
	tplFile << endl;
	for (int idChnl = 1; idChnl <= n; idChnl++) // for each channel
	{
	  for (int idParam = 2; idParam <= 4; idParam++) // for each parameter except qt
	  {
		bool optFlag = false; // indicates if a parameter optimization is required or not
		for (unsigned int k = 0; k < pestAutoChnlParams.size(); k++)
		{
		  if ((pestAutoChnlParams[k][0] == idParam) // checking whether the parameter [idParam] is listed in the Pest parameter vector
			&&(pestAutoChnlParams[k][1] == 1)) // the parameter optimization is required!
		  {
			optFlag = true;
			if (idParam == 2) {tplFile << "#m" << idChnl << "                #" << endl;}
			else if (idParam == 3) {tplFile << "#h" << idChnl << "                #" << endl;}
			else if (idParam == 4) {tplFile << "#s" << idChnl << "                #" << endl;}
		  }
		}
		if (!optFlag) // User-fixed parameter (no optimization). The retained parameter value is that of the first channel.
		{
		  if (idParam == 2) // Mass: special case
		  {
			tplFile << MDMi->getChnlParam(1, 1)/double(n) << endl; // Uniform distribution between the channels
		  }
		  else {tplFile << MDMi->getChnlParam(1, idParam-1) << endl;} // Non-optimized parameter values = first channel parameter values
		}
	  }
	  if (idChnl < n) {tplFile << endl;}
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestADEiAutoChnls::newPestControlFile(int n, int idRemovedChnl)
{
  int const noTimeSteps = MDMi->getNoTimeSteps();
  if (n < 1)
  {
	ErrorMessage->ErrorMessageLabel->Caption = "Error: unvalid n value";
	ErrorMessage->Show();
  }
  else
  {
	vector< vector<double> > const pestAutoChnlParams(getPestParams()); // same vector as PestMDMiAutoChnls->ParamGrid
	int optFlag_qt(pestAutoChnlParams[0][1]); // indicates if the parameter must be optimized (=1) or not (=0)
	int optFlag_m(pestAutoChnlParams[1][1]);
	int optFlag_h(pestAutoChnlParams[2][1]);
	int optFlag_s(pestAutoChnlParams[3][1]);
	ofstream pstFile("MFIT.pst", ios::out | ios::trunc);
	pstFile << "pcf" << endl;
	//============ Control Data Section ============
	pstFile << "* control data" << endl;
	string const RSTFLE = "norestart";
	string PESTMODE;
	if ((PestGeneral->checkTikRegOption() == false) // Tikhonov regularisation mode not selected...
		|| ((PestGeneral->checkTikRegOption() == true) && (PestGeneral->getTikRegType() == 1) // or Tikhonov regularisation with "preferred homogeneity" option...
			&& (n == 1)) // but only one channel...
		|| ((PestGeneral->checkTikRegOption() == true) && (PestGeneral->getTikRegType() == 1) // or Tikhonov regularisation with "preferred homogeneity" option...
			&& (optFlag_m == 0) && (optFlag_h == 0) && (optFlag_s == 0))) // multiple channels, but only qt to optimize
	{
	  PESTMODE = "estimation";
	}
	else PESTMODE = "regularisation";
	int const NPAR = optFlag_qt + n*(optFlag_m + optFlag_h + optFlag_s);
	int const NPARGP(1);
	int NPRIOR, NOBSGP;
	if (PESTMODE == "estimation")
	{
	  NPRIOR = 0;
	  NOBSGP = 1;
	}
	else // PESTMODE == "regularisation"
	{
	  if (PestGeneral->getTikRegType() == 1) // preferred homogeneity
	  {
		NPRIOR = (optFlag_m + optFlag_h + optFlag_s)*(n*(n-1))/2; // number of prior informations for the "preferred homogeneity" regularization: m1=m2, m1=m3, ... etc.
		NOBSGP = 1 + optFlag_m + optFlag_h + optFlag_s;
	  }
	  else // preferred value
	  {
		NPRIOR = NPAR;
		NOBSGP = 1 + optFlag_qt + optFlag_m + optFlag_h + optFlag_s;
	  }
	}
	int const NTPLFLE(1);
	int const NINSFLE(1);
	string const PRECIS = "single";
	string const DPOINT = "point";
	int const NUMCOM(1);
	int JACFILE;
	if (PestGeneral->checkEXTDRVOption()) {JACFILE = 1;} else {JACFILE = 0;} // 1 if use of external (analytical) derivatives, 0 if use of finite-difference derivatives
	int const MESSFILE(0);
	pstFile << RSTFLE << " " << PESTMODE << endl;
	pstFile << NPAR << "  " << noTimeSteps << "  " << NPARGP << "  " << NPRIOR << "  " << NOBSGP << endl;
	pstFile << NTPLFLE << "  " << NINSFLE << "  " << PRECIS << "  " << DPOINT << "  " << NUMCOM << "  " << JACFILE << "  " << MESSFILE << endl;
	pstFile << "10.0  2  0.3  0.01  8"; // RLAMBDA1, RLAMFAC, PHIRATSUF, PHIREDLAM, NUMLAM
	pstFile << "  lamforgive  derforgive" << endl;
	pstFile << "10.0  10.0  0.001" << endl; // RELPARMAX, FACPARMAX, FACORIG
	pstFile << "0.1  1"; // PHIREDSWH, NOPTSWITCH
	if (PestGeneral->checkSVDOption()) pstFile << "  boundscale"; // BOUNDSCALE
	pstFile << endl;
	pstFile << PestGeneral->getNOPTMAX() << "  ";
	pstFile << "0.005  4  4  0.005  4" << endl; // PHIREDSTP, NPHISTP, NPHINORED, RELPARSTP, NRELPAR
	pstFile << "0  0  0"; // ICOV, ICOR, and IEIG
	pstFile << "  parsaveitn" << endl;
	//============ Singular Value Decomposition Section ============
	pstFile << "* singular value decomposition" << endl;
	if (PestGeneral->checkSVDOption()) {pstFile << "2";} else {pstFile << "0";} //*** 2 or 1
	pstFile << endl;
	pstFile << NPAR << "  5.0E-7" << endl; // MAXSING = NPAR, EIGTHRESH
	pstFile << "1" << endl;
	//============ Parameter Groups Section ============
	pstFile << "* parameter groups" << endl;
	string const PARGPNME = "pgnam";
	string const INCTYP = "relative";
	double const DERINC = 0.01;
	double const DERINCLB = 0.0;
	string const FORCEN = "switch";
	double const DERINCMUL = 1.5;
	string const DERMTHD = "parabolic";
	pstFile << PARGPNME << "  " << INCTYP << "  " << DERINC << "  " << DERINCLB;
	pstFile << "  " << FORCEN << "  " << DERINCMUL << "  " << DERMTHD << endl;
	//============ Parameter Data Section ============
	pstFile << "* parameter data" << endl;
	vector<double> mVect, hVect, sVect;
	//---- if not the first iteration (idRemovedChnl != -999): removing one of the previous optimized channels ----
	if (idRemovedChnl != -999)
	{
	  ifstream optResFile ("MFIT.sol");
	  if (optResFile)
	  {
		string line, paramName;
		char idParam;
		int idChnl;
		double paramValue, scale, offset;
		getline(optResFile, line);
		while (getline(optResFile, line))
		{
		  istringstream (line) >> paramName >> paramValue >> scale >> offset;
		  idParam = paramName[0];
		  istringstream(paramName.substr(1,paramName.size()-1)) >> idChnl;
		  if (idChnl != idRemovedChnl)
		  {
			if (idParam == 'm') mVect.push_back(paramValue);
			if (idParam == 'h') hVect.push_back(paramValue);
			if (idParam == 's') sVect.push_back(paramValue);
		  }
		}
	  }
	}
	//---- first processed: qt parameter ----
	if (optFlag_qt == 1) // optimization is required
	{
	  pstFile << "qt"; // PARNME
	  pstFile << "  Log"; // PARTRANS
	  pstFile << "  factor"; // PARCHGLIM
	  double qt_init = MDMi->getQT(); // initial user-specified value
	  // Is an optimized value available from a previous iteration? If yes this optimized value is used rather than the user-specified value.
	  ifstream optResFile ("MFIT.sol");
	  if (optResFile)
	  {
		string line, paramName;
		double paramValue, scale, offset;
		getline(optResFile, line);
		while (getline(optResFile, line))
		{
		  istringstream (line) >> paramName >> paramValue >> scale >> offset;
		  if (paramName == "qt")
		  {
			qt_init = paramValue;
			break;
		  }
		}
	  }
	  pstFile << "  " << qt_init; // PARVAL1 (initial parameter value)
	  pstFile << "  " << pestAutoChnlParams[0][2] ; // PARLBND (parameter's lower bound)
	  pstFile << "  " << pestAutoChnlParams[0][3] ; // PARUBND (parameter's upper bound)
	  pstFile << "  " << "pgnam"; // PARGP
	  pstFile << "  " << 1.0; // SCALE
	  pstFile << "  " << 0.0; // OFFSET
	  int DERCOM;
	  if (PestGeneral->checkEXTDRVOption()) {DERCOM = 0;} else {DERCOM = 1;}
	  pstFile << "  " << DERCOM; // 0 if use of external (analytical) derivatives, 1 if use of finite-difference derivatives
	  pstFile << endl;
	}
	//---------- Then, for each channel ----------
	for (int idChnl = 1; idChnl <= n; idChnl++)
	{
	  //------ m ------
	  if (optFlag_m == 1) // must be optimized
	  {
		pstFile << "m" << idChnl; // PARNME
		pstFile << "  Log"; // PARTRANS
		pstFile << "  factor"; // PARCHGLIM
		double m_init;
		if (mVect.size()>0) // Is an optimized value available from a previous iteration? If yes this optimized value is used rather than the default value (see below).
		{
		  m_init = mVect[idChnl-1];
		}
		else m_init = MDMi->getChnlParam(1, 1)/double(n);
		pstFile << "  " << m_init; // PARVAL1 (initial parameter value)
		pstFile << "  " << pestAutoChnlParams[1][2] << "  " << pestAutoChnlParams[1][3] ; // PARLBND, PARUBND
		int DERCOM;
		if (PestGeneral->checkEXTDRVOption()) {DERCOM = 0;} else {DERCOM = 1;}
		pstFile << "  " << "pgnam" << "  " << 1.0 << "  " << 0.0 << "  " << DERCOM << endl; // PARGP, SCALE, OFFSET, DERCOM
	  }
	  //------ h(T0) ------
	  if (optFlag_h == 1)
	  {
		pstFile << "h" << idChnl;
		pstFile << "  Log";
		pstFile << "  factor";
		double h_init;
		if (hVect.size()>0)	h_init = hVect[idChnl-1];
		else
		{
		  if (n > 1) // h(T0) values are uniformly distributed between tMin et tMax
		  {
			h_init = TCobs_Form->getTobsC05() + (idChnl-1)*(TCobs_Form->getTobsC95()-TCobs_Form->getTobsC05())/(n-1);
		  }
		  else // only one channel
		  {
			h_init = 0.5*TCobs_Form->getTobsC05() + 0.5*TCobs_Form->getTobsC95();
		  }
		}
		pstFile << "  " << h_init;
		pstFile << "  " << pestAutoChnlParams[2][2] << "  " << pestAutoChnlParams[2][3] ;
		int DERCOM;
		if (PestGeneral->checkEXTDRVOption()) {DERCOM = 0;} else {DERCOM = 1;}
		pstFile << "  " << "pgnam" << "  " << 1.0 << "  " << 0.0 << "  " << DERCOM << endl;
	  }
	  //------ s(Pe) ------
	  if (optFlag_s == 1)
	  {
		pstFile << "s" << idChnl;
		pstFile << "  Log";
		pstFile << "  factor";
		double s_init;
		if (sVect.size()>0) s_init = sVect[idChnl-1];
		else // Pe values are tuned in order to obtain well separated concentration peaks
		{
		  double hForsinit;
		  if (hVect.size()>0) hForsinit = hVect[idChnl-1];
		  else
		  {
			if (n > 1) // h(T0) values are uniformly distributed between tMin et tMax
			{
			  hForsinit = TCobs_Form->getTobsC05() + (idChnl-1)*(TCobs_Form->getTobsC95()-TCobs_Form->getTobsC05())/(n-1);
			}
			else // only one channel
			{
			  hForsinit = 0.5*TCobs_Form->getTobsC05() + 0.5*TCobs_Form->getTobsC95();
			}
		  }
		  s_init = pow(15*n*hForsinit/(TCobs_Form->getTobsC95()-TCobs_Form->getTobsC05()),2);
		}
		pstFile << "  " << s_init;
		pstFile << "  " << pestAutoChnlParams[3][2] << "  " << pestAutoChnlParams[3][3] ;
		int DERCOM;
		if (PestGeneral->checkEXTDRVOption()) {DERCOM = 0;} else {DERCOM = 1;}
		pstFile << "  " << "pgnam" << "  " << 1.0 << "  " << 0.0 << "  " << DERCOM << endl;
	  }
	}
	//---------- Observation Groups Section ----------
	pstFile << "* observation groups" << endl;
	pstFile << "Conc" << endl;
	if (PESTMODE == "regularisation")
	{
	  if ((PestGeneral->getTikRegType() == 2) && (optFlag_qt == 1)) pstFile << "regulQT" << endl;
	  if (optFlag_m == 1) pstFile << "regulM" << endl;
	  if (optFlag_h == 1) pstFile << "regulH" << endl;
	  if (optFlag_s == 1) pstFile << "regulS" << endl;
	}
	//============ Observation Data Section ============
	pstFile << "* observation data" << endl;
	// The "observed" concentration and weigth values are interpolated at simulation time values
	vector<double> vectUserObsT;
	vector<double> vectUserObsC;
	vector<double> vectUserObsW;
	for (int i = 0; i < TCobs_Form->getNoUserObs(); i++)
	{
	  vectUserObsT.push_back(TCobs_Form->getTobsValue(i));
	  vectUserObsC.push_back(TCobs_Form->getCobsValue(i));
	  vectUserObsW.push_back(TCobs_Form->getWobsValue(i));
	}
	tk::spline s1, s2; // see http://kluge.in-chemnitz.de/opensource/spline/
	s1.set_points(vectUserObsT,vectUserObsC);
	s2.set_points(vectUserObsT,vectUserObsW);
	double tsimMin = MDMi->getTsimMin();
	double tsimMax = MDMi->getTsimMax();
	for (int i = 0; i < noTimeSteps; i++)
	{
	  pstFile << "o" << i+1; // OBSNME
	  double tSim = tsimMin + i*(tsimMax - tsimMin) / double(noTimeSteps - 1);
	  double sUserObsC_tSim = s1(tSim); // Corresponding concentration value, interpolated from the Observation_Dialog StringGrig
	  if (sUserObsC_tSim<0) sUserObsC_tSim = 0;
	  double sUserObsW_tSim = s2(tSim); // Corresponding weight value, interpolated from the Observation_Dialog StringGrig
	  if (sUserObsW_tSim<0) sUserObsW_tSim = 0;
	  pstFile << "  " << sUserObsC_tSim; // OBSVAL
	  pstFile << "  " << sUserObsW_tSim; // WEIGHT
	  pstFile << "  " << "Conc" << endl; //OBGNME
	}
	//========= Derivative Command Line Section ==========
	if (PestGeneral->checkEXTDRVOption())
	{
	  pstFile << "* derivatives command line" << endl;
	  pstFile << "MDMi /d" << endl; // derivative command line...
	  pstFile << "Deriv.txt"; // EXTDERFLE
	  pstFile << endl;
	}
	//============ Model Command Line Section ============
	pstFile << "* model command line" << endl;
	pstFile << "MDMi" << endl;
	//============ Model Input/Output Section ============
	pstFile << "* model input/output" << endl;
	pstFile << "MFIT.tpl"; // TEMPFLE
	pstFile << "  " << "Input.txt"; // INFLE
	pstFile << endl;
	pstFile << "MFIT.ins"; // INSFLE
	pstFile << "  " << "Output.txt"; // OUTFLE
	pstFile << endl;
	//============ Prior Information Section ============
	pstFile << "* prior information" << endl;
	if ((PESTMODE == "regularisation")&&(PestGeneral->getTikRegType() == 1)) // preferred homogeneity
	{
	  int noPi(1);
	  if (optFlag_m == 1)
	  {
		for (int i = 1; i < n; i++)
		{
		  for (int j = 1; j < n-i+1; j++)
		  {
			pstFile << "pi" << noPi << "  1.0 * ";
			noPi++;
			pstFile << "log(m" << i << ") - 1.0 * log(m" << i+j << ")";
			pstFile << " = 0.0 1.0 regulM" << endl;
		  }
		}
	  }
	  if (optFlag_h == 1)
	  {
		for (int i = 1; i < n; i++)
		{
		  for (int j = 1; j < n-i+1; j++)
		  {
			pstFile << "pi" << noPi << "  1.0 * ";
			noPi++;
			pstFile << "log(h" << i << ") - 1.0 * log(h" << i+j << ")";
			pstFile << " = 0.0 1.0 regulH" << endl;
		  }
		}
	  }
	  if (optFlag_s == 1)
	  {
		for (int i = 1; i < n; i++)
		{
		  for (int j = 1; j < n-i+1; j++)
		  {
			pstFile << "pi" << noPi << "  1.0 * ";
			noPi++;
			pstFile << "log(s" << i << ") - 1.0 * log(s" << i+j << ")";
			pstFile << " = 0.0 1.0 regulS" << endl;
		  }
		}
	  }
	}
	else if ((PESTMODE == "regularisation")&&(PestGeneral->getTikRegType() == 2)) // preferred value
	{
	  int noPi(1);
	  if (optFlag_qt == 1)
	  {
		double qt_init = MDMi->getQT(); // initial user-specified value
		pstFile << "pi" << noPi << "  1.0 * ";
		noPi++;
		pstFile << "log(qt) = " << log10(qt_init);
		pstFile << " 1.0 regulQT" << endl;
	  }
	  if (optFlag_m == 1)
	  {
		double m_init = MDMi->getChnlParam(1, 1);
		for (int i = 1; i <= n; i++)
		{
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  pstFile << "log(m" << i << ") = " << log10(m_init);
		  pstFile << " 1.0 regulM" << endl;
		}
	  }
	  if (optFlag_h == 1)
	  {
		double h_init = MDMi->getChnlParam(1, 2);
		for (int i = 1; i <= n; i++)
		{
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  pstFile << "log(h" << i << ") = " << log10(h_init);
		  pstFile << " 1.0 regulH" << endl;
		}
	  }
	  if (optFlag_s == 1)
	  {
		double s_init = MDMi->getChnlParam(1, 3);
		for (int i = 1; i <= n; i++)
		{
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  pstFile << "log(s" << i << ") = " << log10(s_init);
		  pstFile << " 1.0 regulS" << endl;
		}
	  }
	}
	//============= Regularisation Section ============
	if (PESTMODE == "regularisation")
	{
	  double phimlim = PestGeneral->getPHIMLIM();
	  double phimaccept = 1.05*phimlim;
	  pstFile << "* regularisation" << endl;
	  pstFile << phimlim << "  " << phimaccept << endl; // PHIMLIM, PHIMACCEPT
	  pstFile << "1.0  1.0E-15  1.0E+15" << endl; //WFINIT, WFMIN, WFMAX
	  pstFile << "1.3  2.0E-3  1" << endl; // WFFAC, WFTOL, IREGADJ
	}
  }
}
//---------------------------------------------------------------------------


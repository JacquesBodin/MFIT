//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PestSFDMuserChnls_Dialog.h"
#include "PestGeneralForm.h"
#include "MainForm.h"
#include "MDP_SFDM_Dialog.h"
#include "TCobs_dialog.h"
#include "Error_MessageBox.h"
#include <fstream>
#include <string>
#include "spline.h"

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPestSFDMuserChnls *PestSFDMuserChnls;
//---------------------------------------------------------------------------
__fastcall TPestSFDMuserChnls::TPestSFDMuserChnls(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::FormCreate(TObject *Sender)
{
  ParamGrid->Cells[0][0]="Pest Parameter No.";
  ParamGrid->Cells[1][0]="Type";
  ParamGrid->Cells[2][0]="Channel No.";
  ParamGrid->Cells[3][0]="Use (No=0/Yes=1)";
  ParamGrid->Cells[4][0]="Minimum";
  ParamGrid->Cells[5][0]="Maximum";
  ParamGrid->Cells[6][0]="Transform";
  for (int i=1; i<ParamGrid->RowCount; i++)
  {
	ParamGrid->Cells[0][i]=i;
	ParamGrid->Cells[1][i]="None";
	ParamGrid->Cells[2][i]="1";
	ParamGrid->Cells[3][i]="0";
	ParamGrid->Cells[4][i]="1.0E-10";
	ParamGrid->Cells[5][i]="1.0E+10";
	ParamGrid->Cells[6][i]="Log";
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::resetPestParamGrid()
{
  for (int row = 1; row < ParamGrid->RowCount; row++)
  {
	ParamGrid->Cells[0][row]=row;
	ParamGrid->Cells[1][row]="None";
	ParamGrid->Cells[2][row]="1";
	ParamGrid->Cells[3][row]="0";
	ParamGrid->Cells[4][row]="1.0E-10";
	ParamGrid->Cells[5][row]="1.0E+10";
	ParamGrid->Cells[6][row]="Log";
  }
  ParamGrid->Col=1;
  ParamGrid->Row=1;
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::resetChnlNo_CBox()
{
	ChnlNo_CBox->Items->Clear();
	for (int i = 1; i <= MDP_SFDM->getN(); i++) ChnlNo_CBox->Items->Add(i);
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::setPestGridParam(int idPestParam, std::vector<double> vTTUMMT)
{ //vTTUMMT: Type, Channel No., Use (No=0/Yes=1), Minimum, Maximum, Transform
  if (vTTUMMT[0] == -1)
  {
	ParamGrid->Cells[1][idPestParam] = "Total Flowrate";
	ParamGrid->Cells[2][idPestParam] = "All";
  }
  else if (vTTUMMT[0] == 0) ParamGrid->Cells[1][idPestParam] = "None";
  else if (vTTUMMT[0] == 1) ParamGrid->Cells[1][idPestParam] = "Mass";
  else if (vTTUMMT[0] == 2) ParamGrid->Cells[1][idPestParam] = "Beta Coeff.";
  else if (vTTUMMT[0] == 3) ParamGrid->Cells[1][idPestParam] = "T0";
  else if (vTTUMMT[0] == 4) ParamGrid->Cells[1][idPestParam] = "Pe";
  if (ParamGrid->Cells[1][idPestParam] != "Total Flowrate") ParamGrid->Cells[2][idPestParam] = vTTUMMT[1];
  ParamGrid->Cells[3][idPestParam] = vTTUMMT[2];
  ParamGrid->Cells[4][idPestParam] = FloatToStrF(vTTUMMT[3], ffExponent, 3, 2);
  ParamGrid->Cells[5][idPestParam] = FloatToStrF(vTTUMMT[4], ffExponent, 3, 2);
  if (vTTUMMT[5] == 0) ParamGrid->Cells[6][idPestParam] = "None";
  else if (vTTUMMT[5] == 1) ParamGrid->Cells[6][idPestParam] = "Log";
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::ParamGridClick(TObject *Sender)
{
  PestParam_CBox->Visible = false;
  ChnlNo_CBox->Visible = false;
  UseParam_CBox->Visible = false;
  ParTrans_CBox->Visible = false;

  if(ParamGrid->Col == 1) // if the user clicks in the column "Type"
  {
	TRect Recto = ParamGrid->CellRect(ParamGrid->Col, ParamGrid->Row);
	PestParam_CBox->Top = ParamGrid->Top;
	PestParam_CBox->Left = ParamGrid->Left;
	PestParam_CBox->Top = PestParam_CBox->Top + Recto.Top + ParamGrid->GridLineWidth;
	PestParam_CBox->Left = PestParam_CBox->Left + Recto.Left + ParamGrid->GridLineWidth + 1;
	PestParam_CBox->Height = (Recto.Bottom - Recto.Top) + 1;
	PestParam_CBox->Width = Recto.Right - Recto.Left;
	PestParam_CBox->Text = ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row];
	PestParam_CBox->Visible = true;
  }

  if(ParamGrid->Col == 2) // User click in the column "Channel No."
  {
	if (ParamGrid->Cells[ParamGrid->Col-1][ParamGrid->Row] != "Total Flowrate")
	{
	  ChnlNo_CBox->Items->Clear();
	  for (int i = 1; i <= MDP_SFDM->getN(); i++)
	  {
		ChnlNo_CBox->Items->Add(i);
	  }
	  TRect Recto = ParamGrid->CellRect(ParamGrid->Col, ParamGrid->Row);
	  ChnlNo_CBox->Top = ParamGrid->Top;
	  ChnlNo_CBox->Left = ParamGrid->Left;
	  ChnlNo_CBox->Top = ChnlNo_CBox->Top + Recto.Top + ParamGrid->GridLineWidth;
	  ChnlNo_CBox->Left = ChnlNo_CBox->Left + Recto.Left + ParamGrid->GridLineWidth + 1;
	  ChnlNo_CBox->Height = (Recto.Bottom - Recto.Top) + 1;
	  ChnlNo_CBox->Width = Recto.Right - Recto.Left;
	  ChnlNo_CBox->Text = ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row];
	  ChnlNo_CBox->Visible = true;
	}
  }

  if(ParamGrid->Col == 3) // User click in the column "Use (No=0/Yes=1)"
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

  if(ParamGrid->Col == 6) // User click in the column "Transform"
  {
	TRect Recto = ParamGrid->CellRect(ParamGrid->Col, ParamGrid->Row);
	ParTrans_CBox->Top = ParamGrid->Top;
	ParTrans_CBox->Left = ParamGrid->Left;
	ParTrans_CBox->Top = ParTrans_CBox->Top + Recto.Top + ParamGrid->GridLineWidth;
	ParTrans_CBox->Left = ParTrans_CBox->Left + Recto.Left + ParamGrid->GridLineWidth + 1;
	ParTrans_CBox->Height = (Recto.Bottom - Recto.Top) + 1;
	ParTrans_CBox->Width = Recto.Right - Recto.Left;
	ParTrans_CBox->Text = ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row];
	ParTrans_CBox->Visible = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::ParamGridTopLeftChanged(TObject *Sender)
{
  PestParam_CBox->Visible = false;
  ChnlNo_CBox->Visible = false;
  UseParam_CBox->Visible = false;
  ParTrans_CBox->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::PestParam_CBoxChange(TObject *Sender)
{
  ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row] = PestParam_CBox->Text;
  if (PestParam_CBox->Text == "Total Flowrate")
  {
	ParamGrid->Cells[ParamGrid->Col+1][ParamGrid->Row] = "All";
  }
  else if (ParamGrid->Cells[ParamGrid->Col+1][ParamGrid->Row] == "All")
  {
	ParamGrid->Cells[ParamGrid->Col+1][ParamGrid->Row] = "1";
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::ChnlNo_CBoxChange(TObject *Sender)
{
  ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row] = ChnlNo_CBox->Text;
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::UseParam_CBoxChange(TObject *Sender)
{
  ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row] = UseParam_CBox->Text;
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::ParTrans_CBoxChange(TObject *Sender)
{
  ParamGrid->Cells[ParamGrid->Col][ParamGrid->Row] = ParTrans_CBox->Text;
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::Import_ButtonClick(TObject *Sender)
{
  if (OpenDialog1->Execute())
  {
	 TStringList *StrList = new TStringList;
	 StrList->LoadFromFile(OpenDialog1->FileName.c_str());
	 for(int i=1; i < ParamGrid->RowCount; i++)
	 {
	   ParamGrid->Cells[1][i]="None";
	   ParamGrid->Cells[2][i]="1";
	   ParamGrid->Cells[3][i]="0";
	   ParamGrid->Cells[4][i]="1.0E-10";
	   ParamGrid->Cells[5][i]="1.0E+10";
	   ParamGrid->Cells[6][i]="Log";
	 }
	 for(int i=0; i < StrList->Count; i++)
	 {
	   ParamGrid->Rows[i+1]->StrictDelimiter = true;
	   ParamGrid->Rows[i+1]->CommaText = IntToStr(i+1) + "," + StrList->Strings[i];
	 }
	 delete StrList;
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::OK_ButtonClick(TObject *Sender)
{
  PestParam_CBox->Visible = false;
  ChnlNo_CBox->Visible = false;
  UseParam_CBox->Visible = false;
  ParTrans_CBox->Visible = false;
  Main_Form->PestUserChnls->Checked = true;
  Main_Form->PestAutoChnls->Checked = false;
  Main_Form->PestCreateDatasetsMenu->Enabled = true;
  m_PestParams.clear();
  vector<double> vTTUMMT (6,0); // Type, Channel No., Use (No=0/Yes=1), Minimum, Maximum, Transform, initialized as [0,0,0,0,0,0]
  for (int row = 1; row < ParamGrid->RowCount; row++)
  {
	//------------- A different flag for each type of parameter -----------------
	if (ParamGrid->Cells[1][row] == "None") vTTUMMT[0] = 0;
	else if (ParamGrid->Cells[1][row] == "Total Flowrate") vTTUMMT[0] = -1;
	else if (ParamGrid->Cells[1][row] == "Mass") vTTUMMT[0] = 1;
	else if (ParamGrid->Cells[1][row] == "Beta Coeff.") vTTUMMT[0] = 2;
	else if (ParamGrid->Cells[1][row] == "T0") vTTUMMT[0] = 3;
	else if (ParamGrid->Cells[1][row] == "Pe") vTTUMMT[0] = 4;
	//------------------------------ Channel No. --------------------------------
	if (ParamGrid->Cells[2][row] == "All") vTTUMMT[1] = 0; // (qt parameter)
	else vTTUMMT[1] = ParamGrid->Cells[2][row].ToInt();
	//---------------------------- Use (No=0/Yes=1) -----------------------------
	vTTUMMT[2] = ParamGrid->Cells[3][row].ToInt();
	//------------------------------- Min / Max ---------------------------------
	vTTUMMT[3] = ParamGrid->Cells[4][row].ToDouble();
	vTTUMMT[4] = ParamGrid->Cells[5][row].ToDouble();
	//------------------------------- Transform ---------------------------------
	if (ParamGrid->Cells[6][row] == "None") vTTUMMT[5] = 0;
	else if(ParamGrid->Cells[6][row] == "Log") vTTUMMT[5] = 1; // indicates that the optimization must be based on the log of the parameter
	//---------------------------------------------------------------------------
	m_PestParams.push_back(vTTUMMT);
  }
  Hide();
  Main_Form->notifyChanges();
}
//---------------------------------------------------------------------------
vector< vector<double> > __fastcall TPestSFDMuserChnls::getPestParams() const
{
  return m_PestParams;
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::setPestParams(vector< vector<double> > newPestParams)
{
  m_PestParams = newPestParams;
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::BackToSavedParams()
{
  PestParam_CBox->Visible = false;
  ChnlNo_CBox->Visible = false;
  UseParam_CBox->Visible = false;
  ParTrans_CBox->Visible = false;
  vector< vector<double> > const pestParamVector = PestSFDMuserChnls->getPestParams();
  if (pestParamVector.size()>0)
  {
	for (int row = 1; row < ParamGrid->RowCount; row++)
	{
	  if (pestParamVector[row-1][0] == 0)
	  {
		ParamGrid->Cells[1][row] = "None";
	  }
	  else if (pestParamVector[row-1][0] == -1)
	  {
		ParamGrid->Cells[1][row] = "Total Flowrate";
		ParamGrid->Cells[2][row] = "All";
	  }
	  else if (pestParamVector[row-1][0] == 1)
	  {
		ParamGrid->Cells[1][row] = "Mass";
	  }
	  else if (pestParamVector[row-1][0] == 2)
	  {
		ParamGrid->Cells[1][row] = "Beta Coeff.";
	  }
	  else if (pestParamVector[row-1][0] == 3)
	  {
		ParamGrid->Cells[1][row] = "T0";
	  }
	  else if (pestParamVector[row-1][0] == 4)
	  {
		ParamGrid->Cells[1][row] = "Pe";
	  }
	  if (ParamGrid->Cells[1][row] != "Total Flowrate")
	  {
		ParamGrid->Cells[2][row] = pestParamVector[row-1][1];
	  }
	  ParamGrid->Cells[3][row] = pestParamVector[row-1][2];
	  ParamGrid->Cells[4][row] = FloatToStrF(pestParamVector[row-1][3], ffExponent, 3, 2);   // le 1er chiffre est le nb de caractères avant le signe "E", le second chiffre est le nombre de caractères aprés
	  ParamGrid->Cells[5][row] = FloatToStrF(pestParamVector[row-1][4], ffExponent, 3, 2);
	  if (pestParamVector[row-1][5] == 0)
	  {
		ParamGrid->Cells[6][row] = "None";
	  }
	  else if (pestParamVector[row-1][5] == 1)
	  {
		ParamGrid->Cells[6][row] = "Log";
	  }
	}
  }
  else // pestParamVector is empty
  {
	resetPestParamGrid();
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::Cancel_ButtonClick(TObject *Sender)
{
  BackToSavedParams();
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::FormClose(TObject *Sender, TCloseAction &Action)
{
  BackToSavedParams();
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::newPestTplFile()
{
  int const n = MDP_SFDM->getN(); // number of channels
  if (n < 1)
  {
	ErrorMessage->ErrorMessageLabel->Caption = "Error: number of channels < 1";
	ErrorMessage->Show();
  }
  else
  {
	vector< vector<double> > const pestUserChnlParams(getPestParams()); // a series (vector) of TTUMMT values (Type, Channel No., Use (No=0/Yes=1), Minimum, Maximum, Transform)
	ofstream tplFile("MFIT.tpl", ios::out | ios::trunc);
	tplFile << "ptf #" << endl;
	tplFile << MDP_SFDM->getTsimMin() << endl;
	tplFile << MDP_SFDM->getTsimMax() << endl;
	tplFile << MDP_SFDM->getNoTimeSteps() << endl;
	//---- checking whether an optimization of qt is required -----
	bool optFlag_qt = false; // code indiquant si le paramètre qt doit être inversé ou non
	for (unsigned int k = 0; k <pestUserChnlParams.size(); k++)
	{
	  if ((pestUserChnlParams[k][0] == -1) && (pestUserChnlParams[k][2] == 1))
	  {
		optFlag_qt = true;
		tplFile << "#qt" << "                #" << endl;
		break;
	  }
	}
	if (!optFlag_qt) // qt value fixed by the user, no optimization required
	{
	  tplFile << MDP_SFDM->getQT() << endl;
	}
	//-------------------------------------------------------------------
	tplFile << n << endl;
	tplFile << endl;
	for (int idChnl = 1; idChnl <= n; idChnl++) // for each channel
	{
	  for (int idParam = 1; idParam <= 4; idParam++) // for each parameter (except qt)
	  {
		bool flag = false; // indicates if a parameter optimization is required or not
		for (unsigned int k = 0; k < pestUserChnlParams.size(); k++) // checking whether the pair [idChnl][idParam] is listed in the Pest parameter vector
		{
		  if ((pestUserChnlParams[k][0] == idParam)
			&&(pestUserChnlParams[k][1] == idChnl)
			&&(pestUserChnlParams[k][2] == 1)) // the parameter optimization is required!
		  {
			flag = true;
			if (idParam == 1) {tplFile << "#m" << idChnl << "                #" << endl;}
			else if (idParam == 2) {tplFile << "#b" << idChnl << "                #" << endl;}
			else if (idParam == 3) {tplFile << "#h" << idChnl << "                #" << endl;}
			else if (idParam == 4) {tplFile << "#s" << idChnl << "                #" << endl;}
		  }
		}
		if (!flag) // User-fixed parameter (no optimization)
		{
		  tplFile << MDP_SFDM->getChnlParam(idChnl, idParam) << endl;
		}
	  }
	  if (idChnl < n) tplFile << endl;
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall TPestSFDMuserChnls::newPestControlFile()
{
  int const noTimeSteps = MDP_SFDM->getNoTimeSteps();
  vector< vector<double> > const pestUserChnlParams(getPestParams()); // a series (vector) of TTUMMT values (Type, Channel No., Use (No=0/Yes=1), Minimum, Maximum, Transform)
  ofstream pstFile("MFIT.pst", ios::out | ios::trunc);
  pstFile << "pcf" << endl;
  //============ Control Data Section ============
  pstFile << "* control data" << endl;
  string const RSTFLE = "norestart";
  int NPAR(0);
  int NqtPAR(0);
  int NmPAR(0);
  int NbPAR(0);
  int NhPAR(0);
  int NsPAR(0);
  for (unsigned int i = 0; i < pestUserChnlParams.size(); i++)
  {
	if ((pestUserChnlParams[i][0] != 0)&&(pestUserChnlParams[i][2] == 1))
	{
	  NPAR++; // number of (bulk) parameters to estimate
	  if (pestUserChnlParams[i][0] == -1) NqtPAR = 1; // number of qt-parameter to estimate
	  else if (pestUserChnlParams[i][0] == 1) NmPAR++; // number of m-parameters to estimate
	  else if (pestUserChnlParams[i][0] == 2) NbPAR++; // number of b-parameters to estimate
	  else if (pestUserChnlParams[i][0] == 3) NhPAR++; // number of h-parameters to estimate
	  else if (pestUserChnlParams[i][0] == 4) NsPAR++; // number of s-parameters to estimate
	}
  }
  string PESTMODE;
  if ((PestGeneral->checkTikRegOption() == false) // Tikhonov regularisation mode not selected...
	  || ((PestGeneral->checkTikRegOption() == true) && (PestGeneral->getTikRegType() == 1) // // or Tikhonov regularisation with "preferred homogeneity" option...
		  && (NmPAR <= 1) && (NbPAR <= 1) && (NhPAR <= 1) && (NsPAR <= 1))) // but only single parameters to optimize
  {
	PESTMODE = "estimation";
  }
  else PESTMODE = "regularisation";
  int const NPARGP(1);
  int NPRIOR, NOBSGP;
  if (PESTMODE == "estimation")
  {
	NPRIOR = 0;
	NOBSGP = 1;
  }
  else // PESTMODE == "regularisation"
  {
	NOBSGP = 1;
	if (PestGeneral->getTikRegType() == 1) // preferred homogeneity
	{
	  NPRIOR = (NmPAR*(NmPAR-1) + NbPAR*(NbPAR-1) + NhPAR*(NhPAR-1) + NsPAR*(NsPAR-1)) / 2; // number of prior informations for the "preferred homogeneity" regularization: m1=m2, m1=m3, ... etc.
	  if (NmPAR > 1) NOBSGP++;
	  if (NbPAR > 1) NOBSGP++;
	  if (NhPAR > 1) NOBSGP++;
	  if (NsPAR > 1) NOBSGP++;
	}
	else // preferred value
	{
	  NPRIOR = NPAR;
	  if (NqtPAR > 0) NOBSGP++;
	  if (NmPAR > 0) NOBSGP++;
	  if (NbPAR > 0) NOBSGP++;
	  if (NhPAR > 0) NOBSGP++;
	  if (NsPAR > 0) NOBSGP++;
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
  pstFile << PestGeneral->getPHIREDSTP() << "  ";
  pstFile << PestGeneral->getNPHISTP() << "  ";
  pstFile << PestGeneral->getNPHINORED() << "  ";
  pstFile << PestGeneral->getRELPARSTP() << "  ";
  pstFile << PestGeneral->getNRELPAR() << endl;
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
  for (unsigned int i = 0; i < pestUserChnlParams.size(); i++)
  {
	if ((pestUserChnlParams[i][0] != 0)&&(pestUserChnlParams[i][2] == 1))
	{
	  int idChnl = pestUserChnlParams[i][1];
	  // PARNME
	  if (pestUserChnlParams[i][0] == -1) pstFile << "qt";
	  else if (pestUserChnlParams[i][0] == 1)	pstFile << "m" << idChnl;
	  else if (pestUserChnlParams[i][0] == 2)	pstFile << "b" << idChnl;
	  else if (pestUserChnlParams[i][0] == 3) pstFile << "h" << idChnl;
	  else if (pestUserChnlParams[i][0] == 4) pstFile << "s" << idChnl;
	  // PARTRANS
	  if (pestUserChnlParams[i][5] == 0) pstFile << "  " << "None";
	  else if (pestUserChnlParams[i][5] == 1) pstFile << "  " << "Log";
	  pstFile << "  " << "factor"; // PARCHGLIM
	  // PARVAL1 (initial parameter value)
	  if (pestUserChnlParams[i][0] == -1) pstFile << "  " << MDP_SFDM->getQT();
	  else if (pestUserChnlParams[i][0] == 1) pstFile << "  " << MDP_SFDM->getChnlParam(idChnl, 1);
	  else if (pestUserChnlParams[i][0] == 2) pstFile << "  " << MDP_SFDM->getChnlParam(idChnl, 2);
	  else if (pestUserChnlParams[i][0] == 3) pstFile << "  " << MDP_SFDM->getChnlParam(idChnl, 3);
	  else if (pestUserChnlParams[i][0] == 4) pstFile << "  " << MDP_SFDM->getChnlParam(idChnl, 4);
	  pstFile << "  " << pestUserChnlParams[i][3] ; // PARLBND (parameter's lower bound)
	  pstFile << "  " << pestUserChnlParams[i][4] ; // PARUBND (parameter's upper bound)
	  pstFile << "  " << "pgnam"; // PARGP
	  pstFile << "  " << 1.0; // SCALE
	  pstFile << "  " << 0.0; // OFFSET
	  int DERCOM;
	  if (PestGeneral->checkEXTDRVOption()) {DERCOM = 0;} else {DERCOM = 1;}
	  pstFile << "  " << DERCOM; // 0 if use of external (analytical) derivatives, 1 if use of finite-difference derivatives
	  pstFile << endl;
	}
  }
  //---------- Observation Groups Section ----------
  pstFile << "* observation groups" << endl;
  pstFile << "Conc" << endl;
  if (PESTMODE == "regularisation")
  {
	if (PestGeneral->getTikRegType() == 1) // preferred homogeneity
	{
	  if (NmPAR > 1) pstFile << "regulM" << endl;
	  if (NbPAR > 1) pstFile << "regulB" << endl;
	  if (NhPAR > 1) pstFile << "regulH" << endl;
	  if (NsPAR > 1) pstFile << "regulS" << endl;
	}
	else // preferred value
	{
	  if (NqtPAR > 0) pstFile << "regulQT" << endl;
	  if (NmPAR > 0) pstFile << "regulM" << endl;
	  if (NbPAR > 0) pstFile << "regulB" << endl;
	  if (NhPAR > 0) pstFile << "regulH" << endl;
	  if (NsPAR > 0) pstFile << "regulS" << endl;
	}
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
  double tsimMin = MDP_SFDM->getTsimMin();
  double tsimMax = MDP_SFDM->getTsimMax();
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
	pstFile << "MDP_SFDM /d" << endl; // derivative command line...
	pstFile << "Deriv.txt"; // EXTDERFLE
	pstFile << endl;
  }
  //============ Model Command Line Section ============
  pstFile << "* model command line" << endl;
  pstFile << "MDP_SFDM" << endl;
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
	int noPi = 1;
	if (NmPAR > 1)
	{
	  int idparam(1);
	  unsigned int k(0);
	  for (int i = 1; i < NmPAR; i++)
	  {
		while (k < pestUserChnlParams.size())
		{
		  if ((pestUserChnlParams[k][0] == idparam)&&(pestUserChnlParams[k][2] == 1)) break;
		  k++;
		}
		unsigned int kk = k+1;
		for (int j = 1; j < NmPAR-i+1; j++)
		{
		  while (kk < pestUserChnlParams.size())
		  {
			if ((pestUserChnlParams[kk][0] == idparam)&&(pestUserChnlParams[kk][2] == 1)) break;
			kk++;
		  }
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  if (pestUserChnlParams[k][5] == 1) pstFile << "log(m" << pestUserChnlParams[k][1] << ") - 1.0 * log(m" << pestUserChnlParams[kk][1] << ")";
		  else pstFile << "m" << pestUserChnlParams[k][1] << " - 1.0 * m" << pestUserChnlParams[kk][1];
		  pstFile << " = 0.0 1.0 regulM" << endl;
		  kk++;
		}
		k++;
	  }
	}
	if (NbPAR > 1)
	{
	  int idparam(2);
	  unsigned int k(0);
	  for (int i = 1; i < NbPAR; i++)
	  {
		while (k < pestUserChnlParams.size())
		{
		  if ((pestUserChnlParams[k][0] == idparam)&&(pestUserChnlParams[k][2] == 1)) break;
		  k++;
		}
		unsigned int kk = k+1;
		for (int j = 1; j < NbPAR-i+1; j++)
		{
		  while (kk < pestUserChnlParams.size())
		  {
			if ((pestUserChnlParams[kk][0] == idparam)&&(pestUserChnlParams[kk][2] == 1)) break;
			kk++;
		  }
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  if (pestUserChnlParams[k][5] == 1) pstFile << "log(b" << pestUserChnlParams[k][1] << ") - 1.0 * log(b" << pestUserChnlParams[kk][1] << ")";
		  else pstFile << "b" << pestUserChnlParams[k][1] << " - 1.0 * b" << pestUserChnlParams[kk][1];
		  pstFile << " = 0.0 1.0 regulB" << endl;
		  kk++;
		}
		k++;
	  }
	}
	if (NhPAR > 1)
	{
	  int idparam(3);
	  unsigned int k(0);
	  for (int i = 1; i < NhPAR; i++)
	  {
		while (k < pestUserChnlParams.size())
		{
		  if ((pestUserChnlParams[k][0] == idparam)&&(pestUserChnlParams[k][2] == 1)) break;
		  k++;
		}
		unsigned int kk = k+1;
		for (int j = 1; j < NhPAR-i+1; j++)
		{
		  while (kk < pestUserChnlParams.size())
		  {
			if ((pestUserChnlParams[kk][0] == idparam)&&(pestUserChnlParams[kk][2] == 1)) break;
			kk++;
		  }
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  if (pestUserChnlParams[k][5] == 1) pstFile << "log(h" << pestUserChnlParams[k][1] << ") - 1.0 * log(h" << pestUserChnlParams[kk][1] << ")";
		  else pstFile << "h" << pestUserChnlParams[k][1] << " - 1.0 * h" << pestUserChnlParams[kk][1];
		  pstFile << " = 0.0 1.0 regulH" << endl;
		  kk++;
		}
		k++;
	  }
	}
	if (NsPAR > 1)
	{
	  int idparam(4);
	  unsigned int k(0);
	  for (int i = 1; i < NsPAR; i++)
	  {
		while (k < pestUserChnlParams.size())
		{
		  if ((pestUserChnlParams[k][0] == idparam)&&(pestUserChnlParams[k][2] == 1)) break;
		  k++;
		}
		unsigned int kk = k+1;
		for (int j = 1; j < NsPAR-i+1; j++)
		{
		  while (kk < pestUserChnlParams.size())
		  {
			if ((pestUserChnlParams[kk][0] == idparam)&&(pestUserChnlParams[kk][2] == 1)) break;
			kk++;
		  }
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  if (pestUserChnlParams[k][5] == 1) pstFile << "log(s" << pestUserChnlParams[k][1] << ") - 1.0 * log(s" << pestUserChnlParams[kk][1] << ")";
		  else pstFile << "s" << pestUserChnlParams[k][1] << " - 1.0 * s" << pestUserChnlParams[kk][1];
		  pstFile << " = 0.0 1.0 regulS" << endl;
		  kk++;
		}
		k++;
	  }
	}
  }
  else if ((PESTMODE == "regularisation")&&(PestGeneral->getTikRegType() == 2)) // preferred value
  {
	int noPi = 1;
	if (NqtPAR == 1)
	{
	  int idparam(-1);
	  double qt_init = MDP_SFDM->getQT(); // initial user-specified value
	  bool logTrQT(false);
	  for (unsigned int i = 0; i < pestUserChnlParams.size(); i++)
	  {
		if ((pestUserChnlParams[i][0] == idparam)&&(pestUserChnlParams[i][5] == 1)) logTrQT = true;
	  }
	  pstFile << "pi" << noPi << "  1.0 * ";
	  noPi++;
	  if (logTrQT) pstFile << "log(qt) = " << log10(qt_init);
	  else pstFile << "qt = " << qt_init;
	  pstFile << " 1.0 regulQT" << endl;
	}
	if (NmPAR >= 1)
	{
	  int idparam(1);
	  for (unsigned int i = 0; i < pestUserChnlParams.size(); i++)
	  {
		if ((pestUserChnlParams[i][0] == idparam)&&(pestUserChnlParams[i][2] == 1))
		{
		  int idChnl = pestUserChnlParams[i][1];
		  double m_init = MDP_SFDM->getChnlParam(idChnl, idparam);
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  if (pestUserChnlParams[i][5] == 1) pstFile << "log(m" << idChnl << ") = " << log10(m_init);
		  else pstFile << "m" << idChnl << " = " << m_init;
		  pstFile << " 1.0 regulM" << endl;
		}
	  }
	}
	if (NbPAR >= 1)
	{
	  int idparam(2);
	  for (unsigned int i = 0; i < pestUserChnlParams.size(); i++)
	  {
		if ((pestUserChnlParams[i][0] == idparam)&&(pestUserChnlParams[i][2] == 1))
		{
		  int idChnl = pestUserChnlParams[i][1];
		  double b_init = MDP_SFDM->getChnlParam(idChnl, idparam);
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  if (pestUserChnlParams[i][5] == 1) pstFile << "log(b" << idChnl << ") = " << log10(b_init);
		  else pstFile << "b" << idChnl << " = " << b_init;
		  pstFile << " 1.0 regulB" << endl;
		}
	  }
	}
	if (NhPAR >= 1)
	{
	  int idparam(3);
	  for (unsigned int i = 0; i < pestUserChnlParams.size(); i++)
	  {
		if ((pestUserChnlParams[i][0] == idparam)&&(pestUserChnlParams[i][2] == 1))
		{
		  int idChnl = pestUserChnlParams[i][1];
		  double h_init = MDP_SFDM->getChnlParam(idChnl, idparam);
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  if (pestUserChnlParams[i][5] == 1) pstFile << "log(h" << idChnl << ") = " << log10(h_init);
		  else pstFile << "h" << idChnl << " = " << h_init;
		  pstFile << " 1.0 regulH" << endl;
		}
	  }
	}
	if (NsPAR >= 1)
	{
	  int idparam(4);
	  for (unsigned int i = 0; i < pestUserChnlParams.size(); i++)
	  {
		if ((pestUserChnlParams[i][0] == idparam)&&(pestUserChnlParams[i][2] == 1))
		{
		  int idChnl = pestUserChnlParams[i][1];
		  double s_init = MDP_SFDM->getChnlParam(idChnl, idparam);
		  pstFile << "pi" << noPi << "  1.0 * ";
		  noPi++;
		  if (pestUserChnlParams[i][5] == 1) pstFile << "log(s" << idChnl << ") = " << log10(s_init);
		  else pstFile << "s" << idChnl << " = " << s_init;
		  pstFile << " 1.0 regulS" << endl;
		}
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
//---------------------------------------------------------------------------


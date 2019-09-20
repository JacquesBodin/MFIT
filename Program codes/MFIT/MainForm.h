//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <VCLTee.TeCanvas.hpp>
#include <VCLTee.TeeEdiGrad.hpp>
#include <Data.DB.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.DBChart.hpp>
#include <VCLTee.TeeData.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.WinXCtrls.hpp>

#include <vector>

//---------------------------------------------------------------------------
class TMain_Form : public TForm
{
__published:	// Composants gérés par l'EDI
	TStatusBar *StatusBar;
	TToolBar *ToolBar1;
	TSpeedButton *SpeedButton1;
	TChart *TC_Chart;
	TPointSeries *Series1;
	TLineSeries *Series2;
	TSaveDialog *SaveDialog1;
	TOpenDialog *OpenDialog1;
	TProgressBar *ProgressBar1;
	TActivityIndicator *ActivityIndicator1;
	TSpeedButton *PestAbortSpeedButton;
	TMainMenu *MainMenu;
	TMenuItem *FileMenu;
	TMenuItem *FileNewMenu;
	TMenuItem *FileOpenMenu;
	TMenuItem *FileSaveAsMenu;
	TMenuItem *FileExitMenu;
	TMenuItem *ObservationsMenu;
	TMenuItem *TM_Menu;
	TMenuItem *WrkDirMenu;
	TMenuItem *MDMi_menu;
	TMenuItem *ADEiParamMenu;
	TMenuItem *MDMed_menu;
	TMenuItem *ADEniParamMenu;
	TMenuItem *MDP_SFDM_menu;
	TMenuItem *SFDMparamMenu;
	TMenuItem *N3;
	TMenuItem *BTCcalcMenu;
	TMenuItem *PestMenu;
	TMenuItem *PestOptionsMenu;
	TMenuItem *PestParamMenu;
	TMenuItem *PestUserChnls;
	TMenuItem *PestAutoChnls;
	TMenuItem *N2;
	TMenuItem *PestCreateDatasetsMenu;
	TMenuItem *RunPestChekMenu;
	TMenuItem *RunPestMenu;
	TMenuItem *PestAbortMenu;
	TSplitView *PhiN_SplitViewPannel;
	TChart *PhiN_Chart;
	TLineSeries *LineSeries1;
	TPointSeries *PointSeries1;
	TPanel *Panel1;
	TLabel *Label1;
	TEdit *n_Edit;
	TUpDown *nUpDown;
	TLabel *Label2;
	TLabel *Label3;
	TButton *PhiN_Cancel;
	TButton *PhiN_OK;
	TMenuItem *MDP_2RNE_menu;
	TMenuItem *TwoRNEparamMenu;
	TMenuItem *N4;
	TMenuItem *CalUncertMenu;
	TMenuItem *Help1;
	TMenuItem *AboutMFIT1;
	TMenuItem *N1;
	TMenuItem *N5;
	TMenuItem *Userguide1;
	void __fastcall FileExitMenuClick(TObject *Sender);
	void __fastcall WrkDirMenuClick(TObject *Sender);
	void __fastcall PestCreateDatasetsMenuClick(TObject *Sender);
	void __fastcall PestOptionsMenuClick(TObject *Sender);
	void __fastcall ObservationsMenuClick(TObject *Sender);
	void __fastcall TC_ChartClickAxis(TCustomChart *Sender, TChartAxis *Axis, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall BTCcalcMenuClick(TObject *Sender);
	void __fastcall ADEiParamMenuClick(TObject *Sender);
	void __fastcall RunPestChekMenuClick(TObject *Sender);
	void __fastcall RunPestMenuClick(TObject *Sender);
	void __fastcall FileSaveAsMenuClick(TObject *Sender);
	void __fastcall FileOpenMenuClick(TObject *Sender);
	void __fastcall PestUserChnlsClick(TObject *Sender);
	void __fastcall PestAutoChnlsClick(TObject *Sender);
	void __fastcall ADEniParamMenuClick(TObject *Sender);
	void __fastcall SFDMparamMenuClick(TObject *Sender);
	void __fastcall PestAbortMenuClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall PhiN_CancelClick(TObject *Sender);
	void __fastcall nUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall PhiN_OKClick(TObject *Sender);
	void __fastcall TwoRNEparamMenuClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FileNewMenuClick(TObject *Sender);
	void __fastcall CalUncertMenuClick(TObject *Sender);
	void __fastcall AboutMFIT1Click(TObject *Sender);
	void __fastcall Userguide1Click(TObject *Sender);
private:	// Déclarations utilisateur
	//------------------ Attributs privés ------------------------------------
	std::vector< std::vector<double> > m_modelBTC; // current simulated BTC
	std::vector< std::vector< std::vector<double> > > m_AutoChnlSolBTCs;
	bool m_UnsavedChanges;
	//------------------ Méthodes privées -----------------------------------
	bool __fastcall process_Model_Output();
	void __fastcall plot_modelBTC(std::vector< std::vector<double> > vect_BTC);
	double __fastcall phiCalc();
	void __fastcall init_AutoChnlSolBTCs(int nMax);
	void __fastcall set_AutoChnlSolBTC(int n, std::vector< std::vector<double> > vect_BTC); // set the n-channel solution BTC into m_AutoChnlSolBTCs
	std::vector< std::vector<double> > __fastcall get_AutoChnlSolBTC(int n) const; // extract the n-channel solution BTC
	//------------------------------------------------------------------------
public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ----------------------------------
	void __fastcall notifyChanges();
	//------------------------------------------------------------------------

	__fastcall TMain_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMain_Form *Main_Form;
//---------------------------------------------------------------------------
#endif

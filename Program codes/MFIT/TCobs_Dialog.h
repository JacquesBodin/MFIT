//---------------------------------------------------------------------------

#ifndef TCobs_DialogH
#define TCobs_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Data.DB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>

#include <vector>

//---------------------------------------------------------------------------
class TTCobs_Form : public TForm
{
__published:	// Composants gérés par l'EDI
	TButton *Cancel_Button;
	TButton *OK_Button;
	TStringGrid *TCWgrid;
	TButton *InsertRow_Button;
	TButton *Delete_Button;
	TButton *Apply_Button;
	TButton *AddRows_Button;
	TButton *SelectAll_Button;
	TButton *Copy_Button;
	TButton *Paste_Button;
	TButton *Import_Button;
	TOpenDialog *OpenDialog1;
	TButton *Filters_Button;
	TProgressBar *ProgressBar;
	void __fastcall InsertRow_ButtonClick(TObject *Sender);
	void __fastcall Delete_ButtonClick(TObject *Sender);
	void __fastcall Apply_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall OK_ButtonClick(TObject *Sender);
	void __fastcall AddRows_ButtonClick(TObject *Sender);
	void __fastcall TCWgridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TCWgridSetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SelectAll_ButtonClick(TObject *Sender);
	void __fastcall Copy_ButtonClick(TObject *Sender);
	void __fastcall Paste_ButtonClick(TObject *Sender);
	void __fastcall Import_ButtonClick(TObject *Sender);
	void __fastcall Filters_ButtonClick(TObject *Sender);

private:	// Déclarations utilisateur
	//------------------ Attributs privés ------------------------------------
	std::vector< std::vector<double> > m_vectUserObs;
	double m_tObsMin, m_tObsMax, m_cObsMax; // minimum and maximum observation times, maximum concentration value
	double m_tObsC05, m_tObsC95; // observation times corresponding to 5% and 95% of CobsMax, respectively
	//------------------ Méthodes privées -----------------------------------
	double __fastcall getCobsMax() const;
	void __fastcall setCobsMax(double val);
	void __fastcall setTobsMin(double val);
	void __fastcall setTobsMax(double val);
	void __fastcall setTobsC05(double val);
	void __fastcall setTobsC95(double val);
	//------------------------------------------------------------------------

public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ----------------------------------
	void __fastcall clearVectUserObs();
	void __fastcall addUserObs(std::vector<double> vTCW);
	int __fastcall getNoUserObs() const;
	void __fastcall checkUserObsMinMax();
	double __fastcall getTobsValue(unsigned int idUserObs);
	double __fastcall getCobsValue(unsigned int idUserObs);
	double __fastcall getWobsValue(unsigned int idUserObs);
	double __fastcall getTobsMin() const;
	double __fastcall getTobsMax() const;
	double __fastcall getTobsC05() const;
	double __fastcall getTobsC95() const;
	//------------------------------------------------------------------------

	__fastcall TTCobs_Form(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TTCobs_Form *TCobs_Form;
//---------------------------------------------------------------------------
#endif

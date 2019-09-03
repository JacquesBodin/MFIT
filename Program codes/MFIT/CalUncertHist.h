//---------------------------------------------------------------------------

#ifndef CalUncertHistH
#define CalUncertHistH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TPostCalHist_Form : public TForm
{
__published:	// Composants gérés par l'EDI
	TPanel *Panel1;
	TLabel *Label1;
	TComboBox *Param_CBox;
	TPanel *Panel2;
	TButton *OK_Button;
	TButton *Cancel_Button;
	TChart *Hist_Chart;
	TBarSeries *Series1;
	TButton *Export_Button;
	TSaveDialog *SaveDialog1;
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall Param_CBoxChange(TObject *Sender);
	void __fastcall Export_ButtonClick(TObject *Sender);
private:	// Déclarations utilisateur
	//------------------ Attributs privés ------------------------------------
	std::vector< std::vector<double> > m_vectRecalSets; // all recalibrated parameter sets (+ associated phi values)
	//------------------------------------------------------------------------
public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ----------------------------------
	void __fastcall clearVectRecalSets();
	void __fastcall addRecalSet(std::vector<double> vRecal);
	int __fastcall getNoRecalSets() const;
	double __fastcall getRecalParamValue(unsigned int idRecalSet, unsigned int idParam);
	double __fastcall getParMin(unsigned int idParam);
	double __fastcall getParMax(unsigned int idParam);
	//------------------------------------------------------------------------
	__fastcall TPostCalHist_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPostCalHist_Form *PostCalHist_Form;
//---------------------------------------------------------------------------
#endif

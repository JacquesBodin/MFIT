//---------------------------------------------------------------------------

#ifndef MultiFlow_Generic_DialogH
#define MultiFlow_Generic_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <vector>

//---------------------------------------------------------------------------
class TMultiFlow_Generic : public TForm
{
__published:	// Composants gérés par l'EDI
	TGroupBox *GroupBox1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *TsimMin_Edit;
	TEdit *TsimMax_Edit;
	TEdit *NoTimeSteps_Edit;
	TGroupBox *GroupBox2;
	TLabel *Label1;
	TUpDown *UpDown1;
	TEdit *n_Edit;
	TButton *Update_Button;
	TStringGrid *ChnlParamGrid;
	void __fastcall Update_ButtonClick(TObject *Sender);
protected:	// Déclarations utilisateur
	//------------------ Attributs privés ------------------------------------
	double m_TsimMin, m_TsimMax;
	int m_NoTimeSteps;
	std::vector< std::vector<double> > m_ModelParams; // vector of channels

public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ----------------------------------
	void __fastcall setTsimMin(double tsimmin);
	void __fastcall setTsimMax(double tsimmax);
	void __fastcall setNoTimeSteps(int notimesteps);
	double __fastcall getTsimMin() const;
	double __fastcall getTsimMax() const;
	int __fastcall getNoTimeSteps() const;
	int __fastcall getN() const; // number of channels
	int __fastcall getNoParamPerChnl() const;
	double __fastcall getChnlParam(int idChnl, int idParam) const;
	void __fastcall checkTsimMinMax();
	void __fastcall newPestInstructionFile();
	void __fastcall addChnl(std::vector<double> aChnlParams);
	//------------------------------------------------------------------------

	__fastcall TMultiFlow_Generic(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMultiFlow_Generic *MultiFlow_Generic;
//---------------------------------------------------------------------------
#endif

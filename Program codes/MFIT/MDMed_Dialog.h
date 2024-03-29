//---------------------------------------------------------------------------

#ifndef MDMed_DialogH
#define MDMed_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MultiFlow_Generic_Dialog.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TMDMed : public TMultiFlow_Generic
{
__published:	// Composants g�r�s par l'EDI
	TGroupBox *GroupBox3;
	TLabel *Label5;
	TEdit *c0_Edit;
	TButton *Import_Button;
	TOpenDialog *OpenDialog1;
	TButton *Ok_Button;
	TButton *Cancel_Button;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Import_ButtonClick(TObject *Sender);
	void __fastcall Ok_ButtonClick(TObject *Sender);
private:	// D�clarations utilisateur
	//------------------ Attributs priv�s ------------------------------------
	double m_c0;
	std::vector< std::vector<double> > m_AutoChnlSols; // solutions [n=1~nMax] (option "AutoChnl"): transport parameters for each channel of each solution
	//------------------ M�thodes priv�es ------------------------------------
	void __fastcall BackToSavedOptions();
	//------------------------------------------------------------------------
public:		// D�clarations utilisateur
	//------------------ M�thodes publiques ----------------------------------
    void __fastcall resetForm();
	void __fastcall setC0(double c0);
	double __fastcall getC0() const;
	void __fastcall initChnlsParamGrid();
	void __fastcall initAutoChnlSols(int nMax);
	void __fastcall setAutoChnlSol(int n); // set the n-channel solution into m_AutoChnlSols : used during the AutoChnl optimization process
	void __fastcall setAutoChnlSolParam(int n, int idParam, int idChnl, double paramValue); // set the specified parameter of the n-channel solution into m_AutoChnlSols : used when opening a MFIT file
	std::vector<double> __fastcall getAutoChnlSol(int n) const; // extract the n-channel solution parameters
	void __fastcall setParamGridFromSol(int n);
	//------------------------------------------------------------------------

	__fastcall TMDMed(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMDMed *MDMed;
//---------------------------------------------------------------------------
#endif

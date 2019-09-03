//---------------------------------------------------------------------------

#ifndef MDP_2RNE_DialogH
#define MDP_2RNE_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "MultiFlow_Generic_Dialog.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TMDP_2RNE : public TMultiFlow_Generic
{
__published:	// Composants gérés par l'EDI
	TGroupBox *GroupBox3;
	TLabel *Label5;
	TEdit *QT_Edit;
	TButton *Import_Button;
	TOpenDialog *OpenDialog1;
	TButton *Ok_Button;
	TButton *Cancel_Button;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Import_ButtonClick(TObject *Sender);
	void __fastcall Ok_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Déclarations utilisateur
	//------------------ Attributs privés ------------------------------------
	double m_QT;
	std::vector< std::vector<double> > m_AutoChnlSols; // solutions [n=1~nMax] (option "AutoChnl"): transport parameters for each channel of each solution
	//------------------ Méthodes privées ------------------------------------
	void __fastcall BackToSavedOptions();
	//------------------------------------------------------------------------
public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ------------------------------------
    void __fastcall resetForm();
	void __fastcall setQT(double qt);
	double __fastcall getQT() const;
	void __fastcall initChnlsParamGrid();
	void __fastcall initAutoChnlSols(int nMax);
	void __fastcall setAutoChnlSol(int n); // set the n-channel solution into m_AutoChnlSols : used during the AutoChnl optimization process
	void __fastcall setAutoChnlSolParam(int n, int idParam, int idChnl, double paramValue); // set the specified parameter of the n-channel solution into m_AutoChnlSols : used when opening a MFIT file
	std::vector<double> __fastcall getAutoChnlSol(int n) const; // extract the n-channel solution parameters
	void __fastcall setParamGridFromSol(int n);
	//------------------------------------------------------------------------

	__fastcall TMDP_2RNE(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMDP_2RNE *MDP_2RNE;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef PestMDMedUserChannels_DialogH
#define PestMDMedUserChannels_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TPestADEniUserChnls : public TForm
{
__published:	// Composants g�r�s par l'EDI
	TButton *OK_Button;
	TButton *Cancel_Button;
	TStringGrid *ParamGrid;
	TComboBox *PestParam_CBox;
	TComboBox *ChnlNo_CBox;
	TComboBox *UseParam_CBox;
	TButton *Import_Button;
	TOpenDialog *OpenDialog1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ParamGridTopLeftChanged(TObject *Sender);
	void __fastcall ParamGridClick(TObject *Sender);
	void __fastcall Import_ButtonClick(TObject *Sender);
	void __fastcall PestParam_CBoxChange(TObject *Sender);
	void __fastcall ChnlNo_CBoxChange(TObject *Sender);
	void __fastcall UseParam_CBoxChange(TObject *Sender);
	void __fastcall OK_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// D�clarations utilisateur
	//-------------------- Attributs priv�s ----------------------------------
	std::vector< std::vector<double> > m_PestParams;
	//-------------------- M�thodes priv�es ----------------------------------
	void __fastcall BackToSavedParams();
	//------------------------------------------------------------------------
public:		// D�clarations utilisateur
	//-------------------- M�thodes publiques --------------------------------
	void __fastcall resetPestParamGrid();
	void __fastcall resetChnlNo_CBox();
	void __fastcall setPestGridParam(int idPestParam, std::vector<double> TCEMM);
	std::vector< std::vector<double> > __fastcall getPestParams() const;
	void __fastcall setPestParams(std::vector< std::vector<double> > newPestParams);
	void __fastcall newPestTplFile();
	void __fastcall newPestControlFile();
    //------------------------------------------------------------------------
	__fastcall TPestADEniUserChnls(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPestADEniUserChnls *PestADEniUserChnls;
//---------------------------------------------------------------------------
#endif

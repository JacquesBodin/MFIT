//---------------------------------------------------------------------------

#ifndef Pest2RNEautoChannels_DialogH
#define Pest2RNEautoChannels_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TPest2RNEautoChnls : public TForm
{
__published:	// Composants g�r�s par l'EDI
	TStringGrid *ParamGrid;
	TComboBox *UseParam_CBox;
	TLabel *Label1;
	TEdit *Nmax_Edit;
	TUpDown *UpDown;
	TButton *OK_Button;
	TButton *Cancel_Button;
	TCheckBox *MICP_CheckBox;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ParamGridClick(TObject *Sender);
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
	std::vector< std::vector<double> > __fastcall getPestParams() const;
	void __fastcall clearPestParams();
	void __fastcall addParam(std::vector<double> vTEMM);
	void __fastcall newPestTplFile(int n); // n: number of flow channels
	void __fastcall newPestControlFile(int n, int idRemovedChnl);
	//------------------------------------------------------------------------

	__fastcall TPest2RNEautoChnls(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPest2RNEautoChnls *Pest2RNEautoChnls;
//---------------------------------------------------------------------------
#endif

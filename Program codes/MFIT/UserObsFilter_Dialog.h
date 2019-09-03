//---------------------------------------------------------------------------

#ifndef UserObsFilter_DialogH
#define UserObsFilter_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TUserObsFilter_Form : public TForm
{
__published:	// Composants gérés par l'EDI
	TGroupBox *GroupBox1;
	TButton *Ok_Button;
	TButton *Cancel_Button;
	TCheckBox *ObsFreqFilter_CheckBox;
	TEdit *ObsFreq_Edit;
	TLabel *Label1;
	TUpDown *ObsFreqEdit_UpDown;
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall Ok_ButtonClick(TObject *Sender);
	void __fastcall ObsFreqFilter_CheckBoxClick(TObject *Sender);

private:	// Déclarations utilisateur
public:		// Déclarations utilisateur
	__fastcall TUserObsFilter_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TUserObsFilter_Form *UserObsFilter_Form;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef PestGeneral_DialogH
#define PestGeneral_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPestGeneral : public TForm
{
__published:	// Composants gérés par l'EDI
	TButton *OK_Button;
	TButton *Cancel_Button;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TCheckBox *EXTDRV_CheckBox;
	TCheckBox *SVD_CheckBox;
	TCheckBox *TikReg_CheckBox;
	TLabel *TikRegType_Label;
	TComboBox *TikRegType_CBox;
	TLabel *PHIMLIM_Label;
	TEdit *PHIMLIM_Edit;
	TLabel *Label12;
	TEdit *NOPTMAX_Edit;
	TLabel *Label1;
	TEdit *nRandSampl_Edit;
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall OK_ButtonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TikReg_CheckBoxClick(TObject *Sender);

private:	// Déclarations utilisateur
	//---------------------- Attributs privés --------------------------------
	bool m_USER_APPROVAL_FLAG;
	bool m_EXTDRV_Option;
	int m_NOPTMAX;
	bool m_SVD_Option;
	bool m_TikReg_Option;
	int m_TikReg_Type;
	double m_PHIMLIM;
	int m_NrandSampl;
	//------------------ Méthodes privées ------------------------------------
	void __fastcall BackToSavedPestOptions();
	//------------------------------------------------------------------------

public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ----------------------------------
	bool __fastcall checkEXTDRVOption() const;
	int __fastcall getNOPTMAX() const;
	bool __fastcall checkSVDOption() const;
	bool __fastcall checkTikRegOption() const;
	int __fastcall getTikRegType() const;
	double __fastcall getPHIMLIM() const;
	int __fastcall getNrandSampl() const;
	bool __fastcall checkUserApprovalFlag() const;
	void __fastcall setEXTDRVOption(bool extDrvOption);
	void __fastcall setNOPTMAX(int noptmax);
	void __fastcall setSVDOption(bool svdOption);
	void __fastcall setTikRegOption(bool tikRegOption);
	void __fastcall setTikRegType(int tikRegType);
	void __fastcall setPHIMLIM(double phimlim);
	void __fastcall setNrandSampl(int nrandsampl);
	void __fastcall setUserApprovalFlag(bool userApprovalFlag);
	void __fastcall savePestOptions();
    void __fastcall runPest();
	//------------------------------------------------------------------------
	__fastcall TPestGeneral(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPestGeneral *PestGeneral;
//---------------------------------------------------------------------------
#endif

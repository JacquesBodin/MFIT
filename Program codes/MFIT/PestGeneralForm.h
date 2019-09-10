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
	TLabel *Label7;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TCheckBox *SVD_CheckBox;
	TCheckBox *EXTDRV_CheckBox;
	TTabSheet *TabSheet2;
	TLabel *PHIMLIM_Label;
	TLabel *TikRegType_Label;
	TEdit *PHIMLIM_Edit;
	TComboBox *TikRegType_CBox;
	TCheckBox *TikReg_CheckBox;
	TTabSheet *TabSheet3;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TEdit *NOPTMAX_Edit;
	TEdit *PHIREDSTP_Edit;
	TEdit *NPHISTP_Edit;
	TEdit *RELPARSTP_Edit;
	TEdit *NRELPAR_Edit;
	TEdit *NPHINORED_Edit;
	TTabSheet *TabSheet4;
	TEdit *nRandSampl_Edit;
	TLabel *Label1;
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
	double m_PHIREDSTP, m_RELPARSTP;
	int m_NPHISTP, m_NRELPAR, m_NPHINORED;
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
	double __fastcall getPHIREDSTP() const;
	double __fastcall getRELPARSTP() const;
	int __fastcall getNPHISTP() const;
	int __fastcall getNRELPAR() const;
	int __fastcall getNPHINORED() const;
	bool __fastcall checkSVDOption() const;
	bool __fastcall checkTikRegOption() const;
	int __fastcall getTikRegType() const;
	double __fastcall getPHIMLIM() const;
	int __fastcall getNrandSampl() const;
	bool __fastcall checkUserApprovalFlag() const;
	void __fastcall setEXTDRVOption(bool extDrvOption);
	void __fastcall setNOPTMAX(int noptmax);
	void __fastcall setPHIREDSTP(double phiredstp);
	void __fastcall setRELPARSTP(double relparstp);
	void __fastcall setNPHISTP(int nphistp);
	void __fastcall setNRELPAR(int nrelpar);
	void __fastcall setNPHINORED(int nphinored);
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

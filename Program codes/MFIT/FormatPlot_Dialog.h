//---------------------------------------------------------------------------

#ifndef FormatPlot_DialogH
#define FormatPlot_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormatPlot_Form : public TForm
{
__published:	// Composants gérés par l'EDI
	TButton *Ok_Button;
	TButton *Cancel_Button;
	TButton *Apply_Button;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TCheckBox *Xauto_CheckBox;
	TEdit *Xmin_Edit;
	TEdit *Xmax_Edit;
	TGroupBox *GroupBox2;
	TLabel *Label3;
	TLabel *Label4;
	TCheckBox *Yauto_CheckBox;
	TEdit *Ymin_Edit;
	TEdit *Ymax_Edit;
	void __fastcall Xauto_CheckBoxClick(TObject *Sender);
	void __fastcall Apply_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall Ok_ButtonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Yauto_CheckBoxClick(TObject *Sender);

private:	// Déclarations utilisateur
	//------------------ Attributs privés ------------------------------------
	bool m_BottomAxisAutoScale, m_LeftAxisAutoScale;
	//-------------------- Méthodes privées ----------------------------------
	void __fastcall BackToSavedParams();
	//------------------------------------------------------------------------
public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ----------------------------------
	void __fastcall checkChartAxisMinMax();
	//------------------------------------------------------------------------

	__fastcall TFormatPlot_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormatPlot_Form *FormatPlot_Form;
//---------------------------------------------------------------------------
#endif

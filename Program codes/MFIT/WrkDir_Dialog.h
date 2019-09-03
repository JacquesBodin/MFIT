//---------------------------------------------------------------------------

#ifndef WrkDir_DialogH
#define WrkDir_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class TWrkDir_Form : public TForm
{
__published:	// Composants gérés par l'EDI
	TLabel *Label1;
	TEdit *WrkDir_Edit;
	TButton *Browse_Button;
	TButton *OK_Button;
	TButton *Cancel_Button;
	void __fastcall Browse_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall OK_ButtonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Déclarations utilisateur
	//------------------ Attributs privés ------------------------------------
	AnsiString m_WrkDirPth;
	//------------------------------------------------------------------------
public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ----------------------------------
	AnsiString __fastcall getWrkDirPth() const;
	void __fastcall setWrkDirPth(AnsiString wrkdirpth);
	//------------------------------------------------------------------------
	__fastcall TWrkDir_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWrkDir_Form *WrkDir_Form;
//---------------------------------------------------------------------------
#endif

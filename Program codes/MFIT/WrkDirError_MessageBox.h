//---------------------------------------------------------------------------

#ifndef WrkDirError_MessageBoxH
#define WrkDirError_MessageBoxH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TWDerrorMessage : public TForm
{
__published:	// Composants gérés par l'EDI
	TButton *Ok_Button;
	TLabel *Label1;
	TButton *Cancel_Button;
	void __fastcall Ok_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
private:	// Déclarations utilisateur
public:		// Déclarations utilisateur
	__fastcall TWDerrorMessage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWDerrorMessage *WDerrorMessage;
//---------------------------------------------------------------------------
#endif

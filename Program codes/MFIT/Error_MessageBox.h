//---------------------------------------------------------------------------

#ifndef Error_MessageBoxH
#define Error_MessageBoxH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TErrorMessage : public TForm
{
__published:	// Composants gérés par l'EDI
	TButton *OkButton;
	TLabel *ErrorMessageLabel;
	void __fastcall OkButtonClick(TObject *Sender);
private:	// Déclarations utilisateur
public:		// Déclarations utilisateur
	//------------------ Méthodes publiques ---------------------------------
	void __fastcall showMessage(std::string message);
	//-----------------------------------------------------------------------
	__fastcall TErrorMessage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TErrorMessage *ErrorMessage;
//---------------------------------------------------------------------------
#endif

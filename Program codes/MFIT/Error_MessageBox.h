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
__published:	// Composants g�r�s par l'EDI
	TButton *OkButton;
	TLabel *ErrorMessageLabel;
	void __fastcall OkButtonClick(TObject *Sender);
private:	// D�clarations utilisateur
public:		// D�clarations utilisateur
	//------------------ M�thodes publiques ---------------------------------
	void __fastcall showMessage(std::string message);
	//-----------------------------------------------------------------------
	__fastcall TErrorMessage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TErrorMessage *ErrorMessage;
//---------------------------------------------------------------------------
#endif

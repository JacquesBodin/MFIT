//---------------------------------------------------------------------------

#ifndef FilterResult_DialogH
#define FilterResult_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFilterResult_Form : public TForm
{
__published:	// Composants gérés par l'EDI
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TButton *Button1;
	void __fastcall Button1Click(TObject *Sender);
private:	// Déclarations utilisateur
public:		// Déclarations utilisateur
	__fastcall TFilterResult_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFilterResult_Form *FilterResult_Form;
//---------------------------------------------------------------------------
#endif

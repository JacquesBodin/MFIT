//---------------------------------------------------------------------------

#ifndef PestcheckOutput_MessageBoxH
#define PestcheckOutput_MessageBoxH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TPestcheckOutputMessage : public TForm
{
__published:	// Composants gérés par l'EDI
	TMemo *Memo;
private:	// Déclarations utilisateur
public:		// Déclarations utilisateur
	__fastcall TPestcheckOutputMessage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPestcheckOutputMessage *PestcheckOutputMessage;
//---------------------------------------------------------------------------
#endif

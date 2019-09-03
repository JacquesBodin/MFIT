//---------------------------------------------------------------------------

#ifndef EndInversionUserChannels_DialogH
#define EndInversionUserChannels_DialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TEndInversionUserChnls_Form : public TForm
{
__published:	// Composants g�r�s par l'EDI
	TLabel *Label1;
	TLabel *Label2;
	TButton *Cancel_Button;
	TButton *OK_Button;
	void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall OK_ButtonClick(TObject *Sender);
private:	// D�clarations utilisateur
public:		// D�clarations utilisateur
	__fastcall TEndInversionUserChnls_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEndInversionUserChnls_Form *EndInversionUserChnls_Form;
//---------------------------------------------------------------------------
#endif

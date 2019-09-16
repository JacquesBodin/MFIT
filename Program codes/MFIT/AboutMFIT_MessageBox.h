//---------------------------------------------------------------------------

#ifndef AboutMFIT_MessageBoxH
#define AboutMFIT_MessageBoxH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------
class TAboutMFIT : public TForm
{
__published:	// Composants gérés par l'EDI
	TButton *OkButton;
	TImage *Image1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label5;
	TLinkLabel *LinkLabel1;
	TLinkLabel *LinkLabel2;
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link, TSysLinkType LinkType);
	void __fastcall LinkLabel2LinkClick(TObject *Sender, const UnicodeString Link, TSysLinkType LinkType);


private:	// Déclarations utilisateur
public:		// Déclarations utilisateur
	__fastcall TAboutMFIT(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutMFIT *AboutMFIT;
//---------------------------------------------------------------------------
#endif

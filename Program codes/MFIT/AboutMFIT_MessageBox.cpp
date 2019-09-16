//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AboutMFIT_MessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutMFIT *AboutMFIT;
//---------------------------------------------------------------------------
__fastcall TAboutMFIT::TAboutMFIT(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutMFIT::OkButtonClick(TObject *Sender)
{
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TAboutMFIT::LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
  ShellExecute(0, 0, L"https://github.com/JacquesBodin/MFIT", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TAboutMFIT::LinkLabel2LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
  ShellExecute(0, 0, L"https://spdx.org/licenses/CECILL-2.1.html#licenseText", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

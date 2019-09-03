//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "WrkDir_Dialog.h"
#include "WrkDirError_MessageBox.h"
#include "MainForm.h"
#include <boost/filesystem.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWrkDir_Form *WrkDir_Form;
//---------------------------------------------------------------------------
__fastcall TWrkDir_Form::TWrkDir_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TWrkDir_Form::FormCreate(TObject *Sender)
{
  m_WrkDirPth = "-999"; // initialization flag indicating that the working directory has not yet been defined
}
//---------------------------------------------------------------------------
void __fastcall TWrkDir_Form::setWrkDirPth(AnsiString wrkdirpth)
{
  if (boost::filesystem::is_directory(wrkdirpth.c_str())) // The working directory exists!
  {
	m_WrkDirPth = wrkdirpth;
	boost::filesystem::current_path(wrkdirpth.c_str());
	WrkDir_Edit->Text = wrkdirpth;
	Main_Form->ADEiParamMenu->Enabled = true;
	Main_Form->ADEniParamMenu->Enabled = true;
	Main_Form->SFDMparamMenu->Enabled = true;
	Main_Form->TwoRNEparamMenu->Enabled = true;
  }
  else // WD does not exist
  {
	WDerrorMessage->Show();
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TWrkDir_Form::getWrkDirPth() const
{
  return m_WrkDirPth;
}
//---------------------------------------------------------------------------
void __fastcall TWrkDir_Form::Browse_ButtonClick(TObject *Sender)
{
	// see...
	// http://www.cplusplus.com/forum/windows/111121/
	// http://www.verycomputer.com/5_d35dc407088ff455_1.htm
	// https://www.experts-exchange.com/articles/1600/Browse-for-Folder-Advanced-Options.html
	// http://study.marearts.com/2014/03/c-c-to-select-folder-only-and-get-path.html

	BROWSEINFO bi = {0};
	bi.lpszTitle = _T("Pick a Directory");
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_USENEWUI;
	LPITEMIDLIST pidl = SHBrowseForFolder (&bi);
	if (pidl != 0) {
		// get the name of the folder
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path)) {
			WrkDir_Edit->Text = path;
		}
		CoTaskMemFree(pidl);
	}
}
//---------------------------------------------------------------------------
void __fastcall TWrkDir_Form::OK_ButtonClick(TObject *Sender)
{
  Hide();
  setWrkDirPth(WrkDir_Edit->Text.c_str());
  Main_Form->notifyChanges();
}
//---------------------------------------------------------------------------
void __fastcall TWrkDir_Form::Cancel_ButtonClick(TObject *Sender)
{
  Hide();
  if (m_WrkDirPth != "-999") WrkDir_Edit->Text = m_WrkDirPth;
}
//---------------------------------------------------------------------------
void __fastcall TWrkDir_Form::FormClose(TObject *Sender, TCloseAction &Action)
{
  if (m_WrkDirPth != "-999") WrkDir_Edit->Text = m_WrkDirPth;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UserObsFilter_Dialog.h"
#include "TCobs_dialog.h"
#include "FilterResult_Dialog.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TUserObsFilter_Form *UserObsFilter_Form;
//---------------------------------------------------------------------------
__fastcall TUserObsFilter_Form::TUserObsFilter_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TUserObsFilter_Form::Cancel_ButtonClick(TObject *Sender)
{
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TUserObsFilter_Form::Ok_ButtonClick(TObject *Sender)
{
  Hide();
  if (ObsFreqFilter_CheckBox->Checked)
  {
	SendMessage(TCobs_Form->TCWgrid->Handle, WM_SETREDRAW, false, 0);
	try
	{
	  int nRowsToDel = ObsFreq_Edit->Text.ToInt() - 1;
	  int rowToKeepIndex = 1;
	  FilterResult_Form->Label3->Caption = TCobs_Form->TCWgrid->RowCount - 1;
	  TCobs_Form->Caption = "Observations - Filtering, please wait...";
	  do
	  {
		// (1) shift the contents of the trailing rows
		for (int row = rowToKeepIndex + 1;
			  row < TCobs_Form->TCWgrid->RowCount - 1; ++row)
		{
		  TCobs_Form->TCWgrid->Rows[row] =
		   TCobs_Form->TCWgrid->Rows[row + nRowsToDel];
		  TCobs_Form->TCWgrid->Cells[0][row]=row;
		}
		// (2) remove the last row(s)
		int nRowsBelowRowToKeep = TCobs_Form->TCWgrid->RowCount - 1 - rowToKeepIndex;
		if (nRowsBelowRowToKeep >= nRowsToDel)
		{
		  TCobs_Form->TCWgrid->RowCount -= nRowsToDel;
		}
		else
		{
		  TCobs_Form->TCWgrid->RowCount = rowToKeepIndex + 1;
		}
		rowToKeepIndex++;
		TCobs_Form->ProgressBar->Position = 100*rowToKeepIndex/(TCobs_Form->TCWgrid->RowCount-1);
	  } while (rowToKeepIndex < TCobs_Form->TCWgrid->RowCount - 1);
	  TCobs_Form->ProgressBar->Position = 100;
	  FilterResult_Form->Label4->Caption = TCobs_Form->TCWgrid->RowCount - 1;
	  FilterResult_Form->Show();
	}
	catch (...)
	{
	  SendMessage(TCobs_Form->TCWgrid->Handle, WM_SETREDRAW, true, 0);
	}
	SendMessage(TCobs_Form->TCWgrid->Handle, WM_SETREDRAW, true, 0);

	// update (repaint) the shifted rows
	RECT R =
	 TCobs_Form->TCWgrid->CellRect(0, TCobs_Form->TCWgrid->Row);
	InflateRect(&R, TCobs_Form->TCWgrid->Width,
	 TCobs_Form->TCWgrid->Height);
	InvalidateRect(TCobs_Form->TCWgrid->Handle,
	 &R, false);

    // notify changes
	Main_Form->notifyChanges();
  }
}
//---------------------------------------------------------------------------
void __fastcall TUserObsFilter_Form::ObsFreqFilter_CheckBoxClick(TObject *Sender)
{
  if (ObsFreqFilter_CheckBox->Checked)
  {
	ObsFreq_Edit->Enabled = true;
	ObsFreqEdit_UpDown->Enabled = true;
	Label1->Enabled = true;
  }
  else
  {
	ObsFreq_Edit->Enabled = false;
	ObsFreqEdit_UpDown->Enabled = false;
	Label1->Enabled = false;
  }
}
//---------------------------------------------------------------------------



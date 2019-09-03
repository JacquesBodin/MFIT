//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <fstream>
#include <string>
#include "TCobs_Dialog.h"
#include "UserObsFilter_Dialog.h"
#include "MainForm.h"
#include "Error_MessageBox.h"

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTCobs_Form *TCobs_Form;
//---------------------------------------------------------------------------
__fastcall TTCobs_Form::TTCobs_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
double __fastcall TTCobs_Form::getTobsMin() const
{
  return m_tObsMin;
}
//---------------------------------------------------------------------------
double __fastcall TTCobs_Form::getTobsMax() const
{
  return m_tObsMax;
}
//---------------------------------------------------------------------------
double __fastcall TTCobs_Form::getCobsMax() const
{
  return m_cObsMax;
}
//---------------------------------------------------------------------------
double __fastcall TTCobs_Form::getTobsC05() const
{
  return m_tObsC05;
}
//---------------------------------------------------------------------------
double __fastcall TTCobs_Form::getTobsC95() const
{
  return m_tObsC95;
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::setTobsMin(double val)
{
  m_tObsMin = val; // else default value = -999
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::setTobsMax(double val)
{
  m_tObsMax = val; // else default value = -999
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::setCobsMax(double val)
{
  m_cObsMax = val; // else default value = -999
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::setTobsC05(double val)
{
  m_tObsC05 = val; // else default value = -999
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::setTobsC95(double val)
{
  m_tObsC95 = val; // else default value = -999
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::FormCreate(TObject *Sender)
{
  setTobsMin(-999);
  setTobsMax(-999);
  setCobsMax(-999);
  setTobsC05(-999);
  setTobsC95(-999);
  TCWgrid->Cells[0][0]="Obs. No.";
  TCWgrid->Cells[1][0]="Time";
  TCWgrid->Cells[2][0]="Conc.";
  TCWgrid->Cells[3][0]="Weigth";
  TCWgrid->Cells[0][1]="1";
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::clearVectUserObs()
{
  m_vectUserObs.clear();
  setTobsMin(-999);
  setTobsMax(-999);
  setCobsMax(-999);
  setTobsC05(-999);
  setTobsC95(-999);
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::addUserObs(std::vector<double> vTCW)
{
  m_vectUserObs.push_back(vTCW);
}
//---------------------------------------------------------------------------
int __fastcall TTCobs_Form::getNoUserObs() const
{
  return m_vectUserObs.size();
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::checkUserObsMinMax()
{
  if (getNoUserObs()>0)
  {
	// --- TobsMin, TobsMax ----
	setTobsMin(m_vectUserObs[0][0]);
	setTobsMax(m_vectUserObs[m_vectUserObs.size()-1][0]);
	// ------- CobsMax --------
	double cObsMax = m_vectUserObs[0][1];
	for (unsigned int i = 1; i < m_vectUserObs.size(); i++)
	{
	  if (m_vectUserObs[i][1] > cObsMax) cObsMax = m_vectUserObs[i][1];
	}
	setCobsMax(cObsMax);
	// ------- TobsC05 --------
	bool flagTobsC05(false);
	unsigned int i = 0;
	do
	{
	  if ((m_vectUserObs[i][1] >= 0.05*cObsMax) && (i > 0.1*m_vectUserObs.size()))
	  {
		flagTobsC05 = true;
		setTobsC05(m_vectUserObs[i][0]);
	  }
	  i++;
	} while ((!flagTobsC05)&&(i<m_vectUserObs.size()));
	// ------- TobsC95 --------
	bool flagTobsC95(false);
	int j = m_vectUserObs.size()-1;
	do
	{
	  if ((m_vectUserObs[j][1] >= 0.05*cObsMax) && (j < 0.9*m_vectUserObs.size()))
	  {
		flagTobsC95 = true;
		setTobsC95(m_vectUserObs[j][0]);
	  }
	  j--;
	} while ((!flagTobsC95)&&(j>=0));
  }
}
//---------------------------------------------------------------------------
double __fastcall TTCobs_Form::getTobsValue(unsigned int idUserObs)
{
  if (idUserObs < m_vectUserObs.size()) return m_vectUserObs[idUserObs][0];
  else
  {
	ErrorMessage->showMessage("Error: unvalid idUserObs value");
	return -999;
  }
}
//---------------------------------------------------------------------------
double __fastcall TTCobs_Form::getCobsValue(unsigned int idUserObs)
{
  if (idUserObs < m_vectUserObs.size()) return m_vectUserObs[idUserObs][1];
  else
  {
	ErrorMessage->showMessage("Error: unvalid idUserObs value");
	return -999;
  }
}
//---------------------------------------------------------------------------
double __fastcall TTCobs_Form::getWobsValue(unsigned int idUserObs)
{
  if (idUserObs < m_vectUserObs.size()) return m_vectUserObs[idUserObs][2];
  else
  {
	ErrorMessage->showMessage("Error: unvalid idUserObs value");
	return -999;
  }
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::AddRows_ButtonClick(TObject *Sender)
{
  SendMessage(TCWgrid->Handle, WM_SETREDRAW, false, 0);
  try
  {
	TCWgrid->RowCount += 1;
	TCWgrid->Rows[TCWgrid->RowCount-1]->Clear();
	TCWgrid->Cells[0][TCWgrid->RowCount-1] = TCWgrid->RowCount-1;
  }
  catch (...)
  {
	SendMessage(TCWgrid->Handle, WM_SETREDRAW, true, 0);
  }
  SendMessage(TCWgrid->Handle, WM_SETREDRAW, true, 0);
  //-------- update (repaint) the shifted rows --------
  RECT R =
	TCWgrid->CellRect(0, TCWgrid->Row);
  InflateRect(&R, TCWgrid->Width,
	TCWgrid->Height);
  InvalidateRect(TCWgrid->Handle,
	&R, false);
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::InsertRow_ButtonClick(TObject *Sender)
{
  if (TCWgrid->Row > 0)
  {
	SendMessage(TCWgrid->Handle, WM_SETREDRAW, false, 0);
	try
	{
	  const int row_count = TCWgrid->RowCount;
	  //-------- (1) append a new row to the end --------
	  TCWgrid->RowCount = row_count +1;
	  //-------- (2) shift the contents of the trailing rows --------
	  for (int row = row_count; row > TCWgrid->Row; --row)
	  {
		TCWgrid->Rows[row] = TCWgrid->Rows[row - 1];
		TCWgrid->Cells[0][row] = row;
	  }
	  TCWgrid->Rows[TCWgrid->Row]->Clear();
	  TCWgrid->Cells[0][TCWgrid->Row] = TCWgrid->Row;
	}
	catch (...)
	{
	  SendMessage(TCWgrid->Handle, WM_SETREDRAW, true, 0);
	}
	SendMessage(TCWgrid->Handle, WM_SETREDRAW, true, 0);
	//-------- update (repaint) the shifted rows --------
	RECT R =
	  TCWgrid->CellRect(0, TCWgrid->Row);
	InflateRect(&R, TCWgrid->Width,
	  TCWgrid->Height);
	InvalidateRect(TCWgrid->Handle,
	  &R, false);
  }
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::SelectAll_ButtonClick(TObject *Sender)
{
  TGridRect SelectionAll;
  SelectionAll.Left = 1;
  SelectionAll.Top = 1;
  SelectionAll.Right = TCWgrid->ColCount-1;
  SelectionAll.Bottom = TCWgrid->RowCount-1;
  TCWgrid->Selection = SelectionAll;
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::Delete_ButtonClick(TObject *Sender)
{
  SendMessage(TCWgrid->Handle, WM_SETREDRAW, false, 0);
  try
  {
	const int row_count = TCWgrid->RowCount;
	unsigned int NSelectedRows = TCWgrid->Selection.Bottom - TCWgrid->Selection.Top + 1;
	//-------- (1) shift the contents of the trailing rows --------
	for (int row = TCWgrid->Selection.Top; row < row_count - 1; ++row)
	{
	  TCWgrid->Rows[row] = TCWgrid->Rows[row + NSelectedRows];
	  TCWgrid->Cells[0][row]=row;
	}
	//-------- (2) remove the last row(s) --------
	TCWgrid->RowCount = row_count - NSelectedRows;
	//-------- (3) if all rows deleted >> add an empty row --------
	if (TCWgrid->RowCount == 1)
	{
	  TCWgrid->RowCount = 2;
	  TCWgrid->Rows[1]->Clear();
	  TCWgrid->Cells[0][1] = "1";
	  TCWgrid->FixedRows = 1;
	  TCWgrid->Row = 1;
	}
  }
  catch (...)
  {
	SendMessage(TCWgrid->Handle, WM_SETREDRAW, true, 0);
  }
  SendMessage(TCWgrid->Handle, WM_SETREDRAW, true, 0);
  //-------- update (repaint) the shifted rows --------
  RECT R =
   TCWgrid->CellRect(0, TCWgrid->Row);
  InflateRect(&R, TCWgrid->Width,
   TCWgrid->Height);
  InvalidateRect(TCWgrid->Handle,
   &R, false);
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::TCWgridKeyDown(TObject *Sender,
		  WORD &Key, TShiftState Shift)
{
  TCWgrid->Options << goEditing;
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::TCWgridSetEditText(TObject *Sender,
		  int ACol, int ARow, const UnicodeString Value)
{
  TCWgrid->Options >> goEditing;
}
//---------------------------------------------------------------------------
String __fastcall CellsToText(TStringGrid& AGrid, const TGridRect& ACells)
{ // See: http://bcbjournal.org/articles/vol4/0011/Advanced_string_grid_techniques.htm
  // 1. compute the total size of the buffer required to hold the text of each cell plus the tab and CR/LF delimiters
  int text_len = 0;
  for (int row = ACells.Top; row <= ACells.Bottom; ++row)
  {
	for (int col = ACells.Left; col <= ACells.Right; ++col)
	{
	  text_len += AGrid.Cells[col][row].Length();
	  if (col < ACells.Right) ++text_len;
	}
	if (row < ACells.Bottom) text_len += 2;
  }
  // 2. fill the String with the text of each cell in a tab- and CR/LF-delimited format
  String text;
  text.SetLength(text_len);
  text = "";
  for (int row = ACells.Top; row <= ACells.Bottom; ++row)
  {
	for (int col = ACells.Left; col <= ACells.Right; ++col)
	{
	  text += AGrid.Cells[col][row];
	  if (col < ACells.Right) text += '\t';
	}
	if (row <= ACells.Bottom) text += "\r\n";
  }
  return text;
}
//---------------------------------------------------------------------------
void __fastcall CopyCellsToClipboard(TStringGrid& AGrid, const TGridRect& ACells)
{
  // 1. grab the formatted text
  String text( CellsToText(AGrid, ACells) );
  // 2. copy the text to the clipboard
  Clipboard()->AsText = text;
}
//---------------------------------------------------------------------------
void __fastcall TextToCells(TStringGrid& AGrid, const TGridCoord& AFirstCell, const String AText)
{
  TGridCoord cell = AFirstCell;
  // if the text has no tab-delimiters, copy it as a whole string
  if (!AText.Pos('\t'))
  {
	AGrid.Cells[cell.X][cell.Y] = AText;
	return;
  }
  // parse the text by scanning its contents, character by character,
  // looking for tab and CR delimiters
  int word_start = 1;
  const int text_len = AText.Length();
  for (int index = 1; index < text_len; ++index)
  {
	char current_char = AText[index];
	// if it's a new column
	if (current_char == '\t')
	{
	  AGrid.Cells[cell.X++][cell.Y] = AText.SubString(word_start, index - word_start);
	  // skip the tab character
	  word_start = index + 1;
	}
	// if it's a new row
	else if (current_char == '\r')
	{
	  AGrid.Cells[cell.X][cell.Y++] = AText.SubString(word_start, index - word_start);
	  cell.X = AFirstCell.X;
	  // skip the CR/LF characters
	  word_start = index + 2;
	}
  }
}
//---------------------------------------------------------------------------
void __fastcall PasteCellsFromClipboard(TStringGrid& AGrid, const TGridCoord& AFirstCell)
{
  // extract the text
  AnsiString text(Clipboard()->AsText);
  if (!text.IsEmpty())
  {
	// prevent grid updates
	AGrid.Rows[0]->BeginUpdate();
	try
	{
	  // fill the cells with the text
	  TextToCells(AGrid, AFirstCell, text);
	}
	catch (...)
	{
	   // restore grid updates
	   AGrid.Rows[0]->EndUpdate();
	   throw;
	}
	// restore grid updates
	AGrid.Rows[0]->EndUpdate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::Copy_ButtonClick(TObject *Sender)
{
  CopyCellsToClipboard(*TCWgrid, TCWgrid->Selection);
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::Paste_ButtonClick(TObject *Sender)
{
  PasteCellsFromClipboard(*TCWgrid, TCWgrid->Selection.TopLeft);
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::Import_ButtonClick(TObject *Sender)
{
  if (OpenDialog1->Execute())
  {
	 TStringList *StrList = new TStringList;
	 StrList->LoadFromFile(OpenDialog1->FileName.c_str());
	 TCWgrid->RowCount = StrList->Count + 1;
	 for(int i=0; i < StrList->Count; i++)
	 {
		TCWgrid->Rows[i+1]->CommaText = IntToStr(i+1) + "," + StrList->Strings[i];
	 }
	 delete StrList;
  }
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::Filters_ButtonClick(TObject *Sender)
{
  UserObsFilter_Form->Show();
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::Apply_ButtonClick(TObject *Sender)
{
  const int row_count = TCWgrid->RowCount;
  if (row_count > 1)
  {
	Main_Form->TC_Chart->Series[0]->Clear();
	for (int row = 1; row < row_count; ++row)
	{
	  Main_Form->TC_Chart->Series[0]->AddXY(TCWgrid->Cells[1][row].ToDouble(),
		TCWgrid->Cells[2][row].ToDouble());
	}
	Main_Form->TC_Chart->Visible = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::OK_ButtonClick(TObject *Sender)
{
  Main_Form->TC_Chart->Series[0]->Clear();
  clearVectUserObs();
  vector<double> vTCW (3,0); // Time, Concentration, Weight, initialized as [0,0,0]
  for (int row = 1; row < TCWgrid->RowCount; row++)
  {
	vTCW[0] = TCWgrid->Cells[1][row].ToDouble();
	vTCW[1] = TCWgrid->Cells[2][row].ToDouble();
	vTCW[2] = TCWgrid->Cells[3][row].ToDouble();
	addUserObs(vTCW);
	Main_Form->TC_Chart->Series[0]->AddXY(vTCW[0], vTCW[1]);
  }
  checkUserObsMinMax(); // Identification of TobsMin, TobsMax, Cobs_Max, TobsC05, TobsC95
  Hide();
  Main_Form->TC_Chart->Visible = true;
  Main_Form->notifyChanges();
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::Cancel_ButtonClick(TObject *Sender)
{
  if (m_vectUserObs.size()>0)
  {
	TCWgrid->RowCount = m_vectUserObs.size() + 1;
	for (int row = 1; row < TCWgrid->RowCount; row++)
	{
	  TCWgrid->Cells[0][row] = row;
	  TCWgrid->Cells[1][row] = m_vectUserObs[row-1][0];
	  TCWgrid->Cells[2][row] = m_vectUserObs[row-1][1];
	  TCWgrid->Cells[3][row] = m_vectUserObs[row-1][2];
	}
	Main_Form->TC_Chart->Series[0]->Clear();
	for (int row = 1; row < TCWgrid->RowCount; row++)
	{
	  Main_Form->TC_Chart->Series[0]->AddXY(TCWgrid->Cells[1][row].ToDouble(),
		TCWgrid->Cells[2][row].ToDouble());
	}
  }
  else // Vect_Observations is empty
  {
	TCWgrid->RowCount = 2;
	TCWgrid->Rows[1]->Clear();
	TCWgrid->Cells[0][1] = "1";
	Main_Form->TC_Chart->Series[0]->Clear();
  }
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TTCobs_Form::FormClose(TObject *Sender, TCloseAction &Action)
{
  if (m_vectUserObs.size()>0)
  {
	TCWgrid->RowCount = m_vectUserObs.size() + 1;
	for (int row = 1; row < TCWgrid->RowCount; row++)
	{
	  TCWgrid->Cells[0][row] = row;
	  TCWgrid->Cells[1][row] = m_vectUserObs[row-1][0];
	  TCWgrid->Cells[2][row] = m_vectUserObs[row-1][1];
	  TCWgrid->Cells[3][row] = m_vectUserObs[row-1][2];
	}
  }
  else // Vect_Observations is empty
  {
	TCWgrid->RowCount = 2;
	TCWgrid->Rows[1]->Clear();
	TCWgrid->Cells[0][1] = "1";
  }
}
//---------------------------------------------------------------------------

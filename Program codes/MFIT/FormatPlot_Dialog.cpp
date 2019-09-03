//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormatPlot_Dialog.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormatPlot_Form *FormatPlot_Form;
//---------------------------------------------------------------------------
__fastcall TFormatPlot_Form::TFormatPlot_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::FormCreate(TObject *Sender)
{
  m_BottomAxisAutoScale = true;
  m_LeftAxisAutoScale = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::checkChartAxisMinMax()
{
  Xmin_Edit->Text = Main_Form->TC_Chart->BottomAxis->Minimum;
  Xmax_Edit->Text = Main_Form->TC_Chart->BottomAxis->Maximum;
  Ymin_Edit->Text = Main_Form->TC_Chart->LeftAxis->Minimum;
  Ymax_Edit->Text = Main_Form->TC_Chart->LeftAxis->Maximum;
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::Xauto_CheckBoxClick(TObject *Sender)
{
  if (Xauto_CheckBox->Checked)
  {
	Main_Form->TC_Chart->BottomAxis->Automatic = true;
	Main_Form->TC_Chart->Repaint();
	Xmin_Edit->Text = Main_Form->TC_Chart->BottomAxis->Minimum;
	Xmax_Edit->Text = Main_Form->TC_Chart->BottomAxis->Maximum;
	Xmin_Edit->Enabled = false;
	Xmax_Edit->Enabled = false;
  }
  else
  {
	Main_Form->TC_Chart->BottomAxis->Automatic = false;
	Xmin_Edit->Enabled = true;
	Xmax_Edit->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::Yauto_CheckBoxClick(TObject *Sender)
{
  if (Yauto_CheckBox->Checked)
  {
	Main_Form->TC_Chart->LeftAxis->Automatic = true;
	Main_Form->TC_Chart->Repaint();
	Ymin_Edit->Text = Main_Form->TC_Chart->LeftAxis->Minimum;
	Ymax_Edit->Text = Main_Form->TC_Chart->LeftAxis->Maximum;
	Ymin_Edit->Enabled = false;
	Ymax_Edit->Enabled = false;
  }
  else
  {
	Main_Form->TC_Chart->LeftAxis->Automatic = false;
	Ymin_Edit->Enabled = true;
	Ymax_Edit->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::Apply_ButtonClick(TObject *Sender)
{
  if (!Xauto_CheckBox->Checked)
  {
	Main_Form->TC_Chart->BottomAxis->Minimum = Xmin_Edit->Text.ToDouble();
	Main_Form->TC_Chart->BottomAxis->Maximum = Xmax_Edit->Text.ToDouble();
  }
  if (!Yauto_CheckBox->Checked)
  {
	Main_Form->TC_Chart->LeftAxis->Minimum = Ymin_Edit->Text.ToDouble();
	Main_Form->TC_Chart->LeftAxis->Maximum = Ymax_Edit->Text.ToDouble();
  }
  if ((!Xauto_CheckBox->Checked)||(!Yauto_CheckBox->Checked))
  {
	Main_Form->TC_Chart->Repaint();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::Ok_ButtonClick(TObject *Sender)
{
  m_BottomAxisAutoScale = Xauto_CheckBox->Checked;
  if (!m_BottomAxisAutoScale)
  {
	Main_Form->TC_Chart->BottomAxis->Minimum = Xmin_Edit->Text.ToDouble();
	Main_Form->TC_Chart->BottomAxis->Maximum = Xmax_Edit->Text.ToDouble();
  }
  m_LeftAxisAutoScale = Yauto_CheckBox->Checked;
  if (!m_LeftAxisAutoScale)
  {
	Main_Form->TC_Chart->LeftAxis->Minimum = Ymin_Edit->Text.ToDouble();
	Main_Form->TC_Chart->LeftAxis->Maximum = Ymax_Edit->Text.ToDouble();
  }
  if ((!m_BottomAxisAutoScale)||(!m_LeftAxisAutoScale))
  {
	Main_Form->TC_Chart->Repaint();
  }
  Hide();
  Main_Form->notifyChanges();
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::BackToSavedParams()
{
  Xauto_CheckBox->Checked = m_BottomAxisAutoScale;
  if (Xauto_CheckBox->Checked)
  {
	Main_Form->TC_Chart->BottomAxis->Automatic = true;
	Xmin_Edit->Text = Main_Form->TC_Chart->BottomAxis->Minimum;
	Xmax_Edit->Text = Main_Form->TC_Chart->BottomAxis->Maximum;
	Xmin_Edit->Enabled = false;
	Xmax_Edit->Enabled = false;
  }
  else
  {
	Main_Form->TC_Chart->BottomAxis->Automatic = false;
	Xmin_Edit->Enabled = true;
	Xmax_Edit->Enabled = true;
  }
  Yauto_CheckBox->Checked = m_LeftAxisAutoScale;
  if (Yauto_CheckBox->Checked)
  {
	Main_Form->TC_Chart->LeftAxis->Automatic = true;
	Ymin_Edit->Text = Main_Form->TC_Chart->LeftAxis->Minimum;
	Ymax_Edit->Text = Main_Form->TC_Chart->LeftAxis->Maximum;
	Ymin_Edit->Enabled = false;
	Ymax_Edit->Enabled = false;
  }
  else
  {
	Main_Form->TC_Chart->LeftAxis->Automatic = false;
	Ymin_Edit->Enabled = true;
	Ymax_Edit->Enabled = true;
  }
  if ((Xauto_CheckBox->Checked)||(Yauto_CheckBox->Checked))
  {
	Main_Form->TC_Chart->Repaint();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::Cancel_ButtonClick(TObject *Sender)
{
  BackToSavedParams();
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFormatPlot_Form::FormClose(TObject *Sender, TCloseAction &Action)
{
  BackToSavedParams();
}
//---------------------------------------------------------------------------





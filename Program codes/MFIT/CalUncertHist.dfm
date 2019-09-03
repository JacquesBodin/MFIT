object PostCalHist_Form: TPostCalHist_Form
  Left = 0
  Top = 0
  Caption = 'Post calibration analysis'
  ClientHeight = 487
  ClientWidth = 463
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 463
    Height = 73
    Align = alTop
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 30
      Width = 128
      Height = 13
      Caption = 'Please select a parameter:'
    end
    object Param_CBox: TComboBox
      Left = 166
      Top = 27
      Width = 115
      Height = 21
      TabOrder = 0
      OnChange = Param_CBoxChange
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 414
    Width = 463
    Height = 73
    Align = alBottom
    TabOrder = 1
    object OK_Button: TButton
      Left = 30
      Top = 24
      Width = 75
      Height = 25
      Caption = 'OK'
      TabOrder = 0
    end
    object Cancel_Button: TButton
      Left = 111
      Top = 24
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = Cancel_ButtonClick
    end
    object Export_Button: TButton
      Left = 232
      Top = 24
      Width = 209
      Height = 25
      Caption = 'Export recalibrated parameter values'
      TabOrder = 2
      OnClick = Export_ButtonClick
    end
  end
  object Hist_Chart: TChart
    Left = 0
    Top = 73
    Width = 463
    Height = 341
    Legend.Visible = False
    Title.Text.Strings = (
      'TChart')
    Title.Visible = False
    Align = alClient
    TabOrder = 2
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TBarSeries
      Marks.Visible = False
      Marks.Arrow.Visible = False
      Marks.Callout.Arrow.Visible = False
      BarWidthPercent = 100
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Bar'
      YValues.Order = loNone
    end
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'Text files|*.txt;*.TXT'
    Left = 400
    Top = 24
  end
end

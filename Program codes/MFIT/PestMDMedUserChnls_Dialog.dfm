object PestADEniUserChnls: TPestADEniUserChnls
  Left = 457
  Top = 420
  BorderStyle = bsDialog
  Caption = 'PEST Parameters - User Specified Channels'
  ClientHeight = 296
  ClientWidth = 808
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object OK_Button: TButton
    Left = 308
    Top = 256
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 421
    Top = 256
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Cancel_ButtonClick
  end
  object ParamGrid: TStringGrid
    Left = 16
    Top = 16
    Width = 776
    Height = 222
    ColCount = 6
    DefaultColWidth = 125
    DefaultRowHeight = 21
    RowCount = 61
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goEditing, goTabs]
    ScrollBars = ssVertical
    TabOrder = 2
    OnClick = ParamGridClick
    OnTopLeftChanged = ParamGridTopLeftChanged
    ColWidths = (
      125
      125
      125
      125
      125
      125)
    RowHeights = (
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21
      21)
  end
  object PestParam_CBox: TComboBox
    Left = 548
    Top = 243
    Width = 102
    Height = 21
    TabOrder = 3
    Text = 'None'
    Visible = False
    OnChange = PestParam_CBoxChange
    Items.Strings = (
      'None'
      'Source Concentr.'
      'Flowrate contrib. ratio'
      'T0'
      'Pe'
      'Gamma Coeff.')
  end
  object ChnlNo_CBox: TComboBox
    Left = 548
    Top = 270
    Width = 102
    Height = 21
    TabOrder = 4
    Text = '1'
    Visible = False
    OnChange = ChnlNo_CBoxChange
    Items.Strings = (
      '1')
  end
  object UseParam_CBox: TComboBox
    Left = 656
    Top = 243
    Width = 102
    Height = 21
    TabOrder = 5
    Text = '0'
    Visible = False
    OnChange = UseParam_CBoxChange
    Items.Strings = (
      '0'
      '1')
  end
  object Import_Button: TButton
    Left = 16
    Top = 256
    Width = 75
    Height = 25
    Caption = 'Import...'
    TabOrder = 6
    OnClick = Import_ButtonClick
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Comma-delimited File|*.csv|Text File|*.txt'
    Left = 117
    Top = 248
  end
end

object PestADEniUserChnls: TPestADEniUserChnls
  Left = 457
  Top = 420
  BorderStyle = bsDialog
  Caption = 'PEST Parameters - User Specified Channels'
  ClientHeight = 296
  ClientWidth = 933
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
    Left = 363
    Top = 256
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 476
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
    Width = 902
    Height = 222
    ColCount = 7
    DefaultColWidth = 125
    DefaultRowHeight = 21
    RowCount = 61
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goEditing, goTabs]
    ScrollBars = ssVertical
    TabOrder = 2
    OnClick = ParamGridClick
    OnTopLeftChanged = ParamGridTopLeftChanged
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
    Left = 632
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
    Left = 632
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
  object ParTrans_CBox: TComboBox
    Left = 740
    Top = 270
    Width = 102
    Height = 21
    TabOrder = 5
    Text = 'None'
    Visible = False
    OnChange = ParTrans_CBoxChange
    Items.Strings = (
      'None'
      'Log')
  end
  object UseParam_CBox: TComboBox
    Left = 740
    Top = 243
    Width = 102
    Height = 21
    TabOrder = 6
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
    TabOrder = 7
    OnClick = Import_ButtonClick
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Comma-delimited File|*.csv|Text File|*.txt'
    Left = 117
    Top = 248
  end
end

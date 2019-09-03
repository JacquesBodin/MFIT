object Pest2RNEuserChnls: TPest2RNEuserChnls
  Left = 457
  Top = 420
  BorderStyle = bsDialog
  Caption = 'PEST Parameters'
  ClientHeight = 296
  ClientWidth = 797
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object ParamGrid: TStringGrid
    Left = 16
    Top = 16
    Width = 762
    Height = 222
    ColCount = 7
    DefaultColWidth = 105
    DefaultRowHeight = 21
    RowCount = 61
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goEditing, goTabs]
    ScrollBars = ssVertical
    TabOrder = 0
    OnClick = ParamGridClick
    OnTopLeftChanged = ParamGridTopLeftChanged
    ColWidths = (
      105
      105
      105
      105
      105
      105
      105)
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
  object Import_Button: TButton
    Left = 16
    Top = 256
    Width = 75
    Height = 25
    Caption = 'Import...'
    TabOrder = 1
    OnClick = Import_ButtonClick
  end
  object OK_Button: TButton
    Left = 299
    Top = 256
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 412
    Top = 256
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = Cancel_ButtonClick
  end
  object PestParam_CBox: TComboBox
    Left = 568
    Top = 240
    Width = 102
    Height = 21
    TabOrder = 4
    Text = 'None'
    Visible = False
    OnChange = PestParam_CBoxChange
    Items.Strings = (
      'None'
      'Total Flowrate'
      'Mass'
      'Channel Length'
      'T0'
      'Pe'
      'Psi Coeff.'
      'Omega Coeff.')
  end
  object ChnlNo_CBox: TComboBox
    Left = 568
    Top = 267
    Width = 102
    Height = 21
    TabOrder = 5
    Text = '1'
    Visible = False
    OnChange = ChnlNo_CBoxChange
    Items.Strings = (
      '1')
  end
  object UseParam_CBox: TComboBox
    Left = 676
    Top = 240
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
  object ParTrans_CBox: TComboBox
    Left = 676
    Top = 267
    Width = 102
    Height = 21
    TabOrder = 7
    Text = 'None'
    Visible = False
    OnChange = ParTrans_CBoxChange
    Items.Strings = (
      'None'
      'Log')
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Comma-delimited File|*.csv|Text File|*.txt'
    Left = 112
    Top = 248
  end
end

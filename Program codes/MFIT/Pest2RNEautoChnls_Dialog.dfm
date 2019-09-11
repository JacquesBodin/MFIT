object Pest2RNEautoChnls: TPest2RNEautoChnls
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'PEST Parameters - Automatic Multiple Channels'
  ClientHeight = 296
  ClientWidth = 666
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
  object Label1: TLabel
    Left = 24
    Top = 217
    Width = 146
    Height = 13
    Caption = 'Maximum number of flowpaths'
  end
  object ParamGrid: TStringGrid
    Left = 16
    Top = 16
    Width = 633
    Height = 180
    DefaultColWidth = 125
    DefaultRowHeight = 21
    RowCount = 8
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goEditing, goTabs]
    TabOrder = 0
    OnClick = ParamGridClick
    ColWidths = (
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
      21)
  end
  object UseParam_CBox: TComboBox
    Left = 154
    Top = 74
    Width = 102
    Height = 21
    TabOrder = 1
    Text = '0'
    Visible = False
    OnChange = UseParam_CBoxChange
    Items.Strings = (
      '0'
      '1')
  end
  object ParTrans_CBox: TComboBox
    Left = 408
    Top = 74
    Width = 102
    Height = 21
    TabOrder = 2
    Text = 'None'
    Visible = False
    OnChange = ParTrans_CBoxChange
    Items.Strings = (
      'None'
      'Log')
  end
  object Nmax_Edit: TEdit
    Left = 176
    Top = 214
    Width = 57
    Height = 21
    TabOrder = 3
    Text = '10'
  end
  object UpDown: TUpDown
    Left = 233
    Top = 214
    Width = 16
    Height = 21
    Associate = Nmax_Edit
    Min = 1
    Position = 10
    TabOrder = 4
  end
  object OK_Button: TButton
    Left = 225
    Top = 254
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 5
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 365
    Top = 254
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 6
    OnClick = Cancel_ButtonClick
  end
  object MICP_CheckBox: TCheckBox
    Left = 279
    Top = 216
    Width = 257
    Height = 17
    Caption = 'Enable Multiple Initial Condition Preconditioning'
    Checked = True
    State = cbChecked
    TabOrder = 7
  end
end

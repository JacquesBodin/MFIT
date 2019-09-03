inherited MDP_SFDM: TMDP_SFDM
  Caption = 'Multi-flow transport model parameters - MDP_SFDM'
  ClientWidth = 602
  OnClose = FormClose
  OnCreate = FormCreate
  ExplicitWidth = 608
  PixelsPerInch = 96
  TextHeight = 13
  inherited GroupBox1: TGroupBox
    Width = 441
    ExplicitWidth = 441
  end
  inherited GroupBox2: TGroupBox
    Width = 576
    ExplicitWidth = 576
  end
  inherited ChnlParamGrid: TStringGrid
    Left = 29
    Width = 533
    ColCount = 5
    ExplicitLeft = 29
    ExplicitWidth = 533
    ColWidths = (
      105
      105
      105
      105
      105)
  end
  object GroupBox3: TGroupBox
    Left = 455
    Top = 8
    Width = 129
    Height = 65
    Caption = ' Total flow rate '
    TabOrder = 3
    object Label5: TLabel
      Left = 16
      Top = 29
      Width = 18
      Height = 13
      Caption = 'QT:'
    end
    object QT_Edit: TEdit
      Left = 37
      Top = 26
      Width = 57
      Height = 21
      TabOrder = 0
      Text = '0'
    end
  end
  object Import_Button: TButton
    Left = 16
    Top = 328
    Width = 75
    Height = 25
    Caption = 'Import...'
    TabOrder = 4
    OnClick = Import_ButtonClick
  end
  object Ok_Button: TButton
    Left = 277
    Top = 328
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 5
    OnClick = Ok_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 358
    Top = 328
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 6
    OnClick = Cancel_ButtonClick
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Text file|*.txt'
    Left = 104
    Top = 328
  end
end

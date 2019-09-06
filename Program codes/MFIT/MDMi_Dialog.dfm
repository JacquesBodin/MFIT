inherited MDMi: TMDMi
  Caption = 
    'Multi-Flow transport model parameters - MDMi (ADE instantaneous ' +
    'injection)'
  ClientWidth = 602
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
    Width = 535
    ColCount = 4
    DefaultColWidth = 132
    ExplicitLeft = 29
    ExplicitWidth = 535
    ColWidths = (
      132
      132
      132
      132)
  end
  object Import_Button: TButton
    Left = 16
    Top = 328
    Width = 75
    Height = 25
    Caption = 'Import...'
    TabOrder = 3
    OnClick = Import_ButtonClick
  end
  object Ok_Button: TButton
    Left = 277
    Top = 328
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 4
    OnClick = Ok_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 358
    Top = 328
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 5
    OnClick = Cancel_ButtonClick
  end
  object GroupBox3: TGroupBox
    Left = 455
    Top = 8
    Width = 129
    Height = 65
    Caption = ' Total flow rate '
    TabOrder = 6
    object Label5: TLabel
      Left = 20
      Top = 29
      Width = 12
      Height = 13
      Caption = 'Q:'
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
  object OpenDialog1: TOpenDialog
    Filter = 'Text file|*.txt'
    Left = 104
    Top = 328
  end
end

inherited MDMed: TMDMed
  Caption = 
    'Multi-flow transport model parameters - MDMed (ADE non-instantan' +
    'eous injection)'
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  inherited GroupBox1: TGroupBox
    Width = 441
    ExplicitWidth = 441
  end
  inherited ChnlParamGrid: TStringGrid
    Left = 16
    Width = 743
    ColCount = 5
    DefaultColWidth = 147
    ExplicitLeft = 16
    ExplicitWidth = 743
    ColWidths = (
      147
      147
      147
      147
      147)
  end
  object GroupBox3: TGroupBox
    Left = 464
    Top = 8
    Width = 305
    Height = 65
    Caption = ' Source concentration '
    TabOrder = 3
    object Label5: TLabel
      Left = 24
      Top = 29
      Width = 15
      Height = 13
      Caption = 'c0:'
    end
    object c0_Edit: TEdit
      Left = 45
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
    Left = 312
    Top = 328
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 5
    OnClick = Ok_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 393
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

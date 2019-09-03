object MultiFlow_Generic: TMultiFlow_Generic
  Left = 517
  Top = 439
  BorderStyle = bsDialog
  Caption = 'Multi-Flow transport model parameters'
  ClientHeight = 361
  ClientWidth = 777
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 761
    Height = 65
    Caption = ' Simulation time range '
    TabOrder = 1
    object Label2: TLabel
      Left = 24
      Top = 29
      Width = 26
      Height = 13
      Caption = 'Tmin:'
    end
    object Label3: TLabel
      Left = 152
      Top = 29
      Width = 30
      Height = 13
      Caption = 'Tmax:'
    end
    object Label4: TLabel
      Left = 280
      Top = 29
      Width = 76
      Height = 13
      Caption = 'No. Time Steps:'
    end
    object TsimMin_Edit: TEdit
      Left = 56
      Top = 26
      Width = 57
      Height = 21
      TabOrder = 0
      Text = '0'
    end
    object TsimMax_Edit: TEdit
      Left = 188
      Top = 26
      Width = 57
      Height = 21
      TabOrder = 1
      Text = '0'
    end
    object NoTimeSteps_Edit: TEdit
      Left = 362
      Top = 26
      Width = 57
      Height = 21
      TabOrder = 2
      Text = '500'
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 79
    Width = 761
    Height = 234
    Caption = ' Transport Channels '
    TabOrder = 2
    object Label1: TLabel
      Left = 24
      Top = 27
      Width = 101
      Height = 13
      Caption = 'Number of Channels:'
    end
    object UpDown1: TUpDown
      Left = 173
      Top = 24
      Width = 16
      Height = 21
      Associate = n_Edit
      Min = 1
      Position = 1
      TabOrder = 0
    end
    object n_Edit: TEdit
      Left = 132
      Top = 24
      Width = 41
      Height = 21
      TabOrder = 1
      Text = '1'
    end
    object Update_Button: TButton
      Left = 193
      Top = 22
      Width = 75
      Height = 25
      Caption = 'Update'
      TabOrder = 2
      OnClick = Update_ButtonClick
    end
  end
  object ChnlParamGrid: TStringGrid
    Left = 8
    Top = 137
    Width = 745
    Height = 161
    ColCount = 7
    DefaultColWidth = 105
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goEditing, goTabs]
    TabOrder = 0
    ColWidths = (
      105
      105
      105
      105
      105
      105
      105)
    RowHeights = (
      24
      24)
  end
end

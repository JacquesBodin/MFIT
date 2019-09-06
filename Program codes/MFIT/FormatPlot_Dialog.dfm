object FormatPlot_Form: TFormatPlot_Form
  Left = 467
  Top = 321
  BorderStyle = bsDialog
  Caption = 'Format Plot'
  ClientHeight = 201
  ClientWidth = 340
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
  object Ok_Button: TButton
    Left = 49
    Top = 161
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = Ok_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 130
    Top = 161
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Cancel_ButtonClick
  end
  object Apply_Button: TButton
    Left = 211
    Top = 161
    Width = 75
    Height = 25
    Caption = 'Apply'
    TabOrder = 2
    OnClick = Apply_ButtonClick
  end
  object GroupBox1: TGroupBox
    Left = 16
    Top = 16
    Width = 305
    Height = 57
    Caption = ' X-Axis Range '
    TabOrder = 3
    object Label1: TLabel
      Left = 88
      Top = 25
      Width = 20
      Height = 13
      Caption = 'Min:'
    end
    object Label2: TLabel
      Left = 200
      Top = 25
      Width = 24
      Height = 13
      Caption = 'Max:'
    end
    object Xauto_CheckBox: TCheckBox
      Left = 23
      Top = 23
      Width = 46
      Height = 17
      Caption = 'Auto'
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnClick = Xauto_CheckBoxClick
    end
    object Xmin_Edit: TEdit
      Left = 113
      Top = 21
      Width = 57
      Height = 21
      Enabled = False
      TabOrder = 1
    end
    object Xmax_Edit: TEdit
      Left = 230
      Top = 21
      Width = 57
      Height = 21
      Enabled = False
      TabOrder = 2
    end
  end
  object GroupBox2: TGroupBox
    Left = 16
    Top = 88
    Width = 305
    Height = 57
    Caption = 'Y-Axis Range '
    TabOrder = 4
    object Label3: TLabel
      Left = 88
      Top = 25
      Width = 20
      Height = 13
      Caption = 'Min:'
    end
    object Label4: TLabel
      Left = 200
      Top = 25
      Width = 24
      Height = 13
      Caption = 'Max:'
    end
    object Yauto_CheckBox: TCheckBox
      Left = 23
      Top = 23
      Width = 46
      Height = 17
      Caption = 'Auto'
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnClick = Yauto_CheckBoxClick
    end
    object Ymin_Edit: TEdit
      Left = 113
      Top = 21
      Width = 57
      Height = 21
      Enabled = False
      TabOrder = 1
    end
    object Ymax_Edit: TEdit
      Left = 230
      Top = 21
      Width = 57
      Height = 21
      Enabled = False
      TabOrder = 2
    end
  end
end

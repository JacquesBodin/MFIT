object UserObsFilter_Form: TUserObsFilter_Form
  Left = 617
  Top = 507
  BorderStyle = bsDialog
  Caption = 'Observation Filter'
  ClientHeight = 113
  ClientWidth = 302
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
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 281
    Height = 65
    TabOrder = 0
    object Label1: TLabel
      Left = 192
      Top = 26
      Width = 62
      Height = 13
      Caption = 'observations'
      Enabled = False
    end
    object ObsFreqFilter_CheckBox: TCheckBox
      Left = 24
      Top = 24
      Width = 89
      Height = 17
      Caption = 'Retain every'
      TabOrder = 0
      OnClick = ObsFreqFilter_CheckBoxClick
    end
    object ObsFreq_Edit: TEdit
      Left = 114
      Top = 22
      Width = 55
      Height = 21
      Enabled = False
      TabOrder = 1
      Text = '10'
    end
    object ObsFreqEdit_UpDown: TUpDown
      Left = 169
      Top = 22
      Width = 16
      Height = 21
      Associate = ObsFreq_Edit
      Enabled = False
      Min = 1
      Position = 10
      TabOrder = 2
    end
  end
  object Ok_Button: TButton
    Left = 16
    Top = 80
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = Ok_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 104
    Top = 80
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Cancel_ButtonClick
  end
end

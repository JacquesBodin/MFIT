object PestGeneral: TPestGeneral
  Left = 467
  Top = 372
  BorderStyle = bsDialog
  Caption = 'Pest Options'
  ClientHeight = 376
  ClientWidth = 428
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
    Left = 262
    Top = 340
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 345
    Top = 340
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Cancel_ButtonClick
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 262
    Width = 412
    Height = 65
    Caption = ' Post calibration uncertainty analysis '
    TabOrder = 2
    object Label1: TLabel
      Left = 31
      Top = 28
      Width = 141
      Height = 13
      Caption = 'Number of random value sets'
    end
    object nRandSampl_Edit: TEdit
      Left = 186
      Top = 25
      Width = 57
      Height = 21
      TabOrder = 0
      Text = '50'
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 191
    Width = 412
    Height = 65
    Caption = ' Run Termination '
    TabOrder = 3
    object Label12: TLabel
      Left = 31
      Top = 28
      Width = 215
      Height = 13
      Caption = 'Maximum Optimization Iterations (NOPTMAX)'
    end
    object NOPTMAX_Edit: TEdit
      Left = 260
      Top = 25
      Width = 57
      Height = 21
      TabOrder = 0
      Text = '200'
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 8
    Width = 412
    Height = 177
    Caption = ' Basic Options '
    TabOrder = 4
    object TikRegType_Label: TLabel
      Left = 32
      Top = 111
      Width = 95
      Height = 13
      Caption = 'Regularization Type'
      Enabled = False
    end
    object PHIMLIM_Label: TLabel
      Left = 32
      Top = 140
      Width = 280
      Height = 13
      Caption = 'Upper Limit of Measurement Objective Function (PHIMLIM)'
      Enabled = False
    end
    object EXTDRV_CheckBox: TCheckBox
      Left = 16
      Top = 26
      Width = 208
      Height = 17
      Caption = 'Use External Analytical Derivatives'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object SVD_CheckBox: TCheckBox
      Left = 16
      Top = 54
      Width = 254
      Height = 17
      Caption = 'Use Singular Value Decomposition (SVD)'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object TikReg_CheckBox: TCheckBox
      Left = 16
      Top = 82
      Width = 166
      Height = 17
      Caption = 'Use Tikhonov Regularization'
      TabOrder = 2
      OnClick = TikReg_CheckBoxClick
    end
    object TikRegType_CBox: TComboBox
      Left = 143
      Top = 108
      Width = 145
      Height = 21
      Enabled = False
      ItemIndex = 0
      TabOrder = 3
      Text = 'Preferred Homogeneity'
      Items.Strings = (
        'Preferred Homogeneity'
        'Preferred Value')
    end
    object PHIMLIM_Edit: TEdit
      Left = 329
      Top = 137
      Width = 57
      Height = 21
      Enabled = False
      TabOrder = 4
      Text = '1.0E-10'
    end
  end
end

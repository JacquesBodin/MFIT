object PestGeneral: TPestGeneral
  Left = 467
  Top = 372
  BorderStyle = bsDialog
  Caption = 'Pest Options'
  ClientHeight = 258
  ClientWidth = 485
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
  object Label7: TLabel
    Left = 11
    Top = 132
    Width = 148
    Height = 13
    Caption = 'Number of Lambdas (NUMLAM)'
  end
  object OK_Button: TButton
    Left = 274
    Top = 225
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 355
    Top = 225
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Cancel_ButtonClick
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 485
    Height = 217
    ActivePage = TabSheet1
    Align = alTop
    TabOrder = 2
    object TabSheet1: TTabSheet
      Caption = 'Basic Options'
      object Label2: TLabel
        Left = 16
        Top = 18
        Width = 187
        Height = 13
        Caption = 'Lambda Adjustment Factor (RLAMFAC)'
      end
      object RLAMFAC_Edit: TEdit
        Left = 240
        Top = 15
        Width = 57
        Height = 21
        TabOrder = 0
        Text = '2.0'
      end
      object BOUNDSCALE_CheckBox: TCheckBox
        Left = 16
        Top = 103
        Width = 254
        Height = 17
        Caption = 'Use Bound scaling of parameters'
        Checked = True
        State = cbChecked
        TabOrder = 1
      end
      object SVD_CheckBox: TCheckBox
        Left = 16
        Top = 73
        Width = 254
        Height = 17
        Caption = 'Use Singular Value Decomposition (SVD)'
        Checked = True
        State = cbChecked
        TabOrder = 2
        OnClick = SVD_CheckBoxClick
      end
      object EXTDRV_CheckBox: TCheckBox
        Left = 16
        Top = 45
        Width = 208
        Height = 17
        Caption = 'Use External Analytical Derivatives'
        Checked = True
        State = cbChecked
        TabOrder = 3
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Tikhonov Regularization'
      ImageIndex = 1
      object PHIMLIM_Label: TLabel
        Left = 16
        Top = 72
        Width = 280
        Height = 13
        Caption = 'Upper Limit of Measurement Objective Function (PHIMLIM)'
        Enabled = False
      end
      object PHIMACCEPT_Label: TLabel
        Left = 16
        Top = 99
        Width = 289
        Height = 13
        Caption = 'Acceptable Measurement Objective Function (PHIMACCEPT)'
        Enabled = False
      end
      object IREGADJ_Label: TLabel
        Left = 16
        Top = 126
        Width = 251
        Height = 13
        Caption = 'Intergroup Regularization (IREGADJ) Option (1 to 5)'
        Enabled = False
      end
      object TikRegType_Label: TLabel
        Left = 16
        Top = 45
        Width = 95
        Height = 13
        Caption = 'Regularization Type'
        Enabled = False
      end
      object PHIMLIM_Edit: TEdit
        Left = 333
        Top = 69
        Width = 57
        Height = 21
        Enabled = False
        TabOrder = 0
        Text = '1.0E-10'
      end
      object PHIMACCEPT_Edit: TEdit
        Left = 333
        Top = 96
        Width = 57
        Height = 21
        Enabled = False
        TabOrder = 1
        Text = '1.05E-10'
      end
      object IREGADJ_Edit: TEdit
        Left = 333
        Top = 123
        Width = 57
        Height = 21
        Enabled = False
        TabOrder = 2
        Text = '1'
      end
      object TikRegType_CBox: TComboBox
        Left = 133
        Top = 42
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
      object TikReg_CheckBox: TCheckBox
        Left = 16
        Top = 16
        Width = 166
        Height = 17
        Caption = 'Use Tikhonov Regularization'
        TabOrder = 4
        OnClick = TikReg_CheckBoxClick
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Run Termination'
      ImageIndex = 2
      object Label12: TLabel
        Left = 11
        Top = 18
        Width = 215
        Height = 13
        Caption = 'Maximum Optimization Iterations (NOPTMAX)'
      end
      object Label13: TLabel
        Left = 11
        Top = 45
        Width = 186
        Height = 13
        Caption = 'Objective Func. Criterion (PHIREDSTP)'
      end
      object Label14: TLabel
        Left = 11
        Top = 72
        Width = 185
        Height = 13
        Caption = 'Max. PHIREDSTP Iterations (NPHISTP)'
      end
      object Label15: TLabel
        Left = 11
        Top = 126
        Width = 176
        Height = 13
        Caption = 'Max. Rel. Par. Change (RELPARSTP)'
      end
      object Label16: TLabel
        Left = 11
        Top = 153
        Width = 189
        Height = 13
        Caption = 'Max. RELPARSTP Iterations (NRELPAR)'
      end
      object Label17: TLabel
        Left = 11
        Top = 99
        Width = 175
        Height = 13
        Caption = 'Max. Failed Iterations (NPHINORED)'
      end
      object NOPTMAX_Edit: TEdit
        Left = 240
        Top = 15
        Width = 57
        Height = 21
        TabOrder = 0
        Text = '200'
      end
      object PHIREDSTP_Edit: TEdit
        Left = 240
        Top = 42
        Width = 57
        Height = 21
        TabOrder = 1
        Text = '1.0E-4'
      end
      object NPHISTP_Edit: TEdit
        Left = 240
        Top = 69
        Width = 57
        Height = 21
        TabOrder = 2
        Text = '5'
      end
      object RELPARSTP_Edit: TEdit
        Left = 240
        Top = 123
        Width = 57
        Height = 21
        TabOrder = 3
        Text = '1.0E-4'
      end
      object NRELPAR_Edit: TEdit
        Left = 240
        Top = 150
        Width = 57
        Height = 21
        TabOrder = 4
        Text = '5'
      end
      object NPHINORED_Edit: TEdit
        Left = 240
        Top = 96
        Width = 57
        Height = 21
        TabOrder = 5
        Text = '4'
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Post calibration uncertainty analysis'
      ImageIndex = 3
      object Label1: TLabel
        Left = 16
        Top = 18
        Width = 141
        Height = 13
        Caption = 'Number of random value sets'
      end
      object nRandSampl_Edit: TEdit
        Left = 177
        Top = 15
        Width = 57
        Height = 21
        TabOrder = 0
        Text = '50'
      end
    end
  end
end

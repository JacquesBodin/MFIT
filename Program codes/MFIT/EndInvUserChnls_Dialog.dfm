object EndInversionUserChnls_Form: TEndInversionUserChnls_Form
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Inversion results'
  ClientHeight = 148
  ClientWidth = 413
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
  object Label1: TLabel
    Left = 24
    Top = 24
    Width = 355
    Height = 13
    Caption = 
      'Inversion is complete: Would you like to import the estimated pa' +
      'rameters?'
    Color = clBlue
    ParentColor = False
  end
  object Label2: TLabel
    Left = 24
    Top = 56
    Width = 355
    Height = 13
    Caption = 
      '(caution: all the initial parameters will be replaced by the new' +
      ' parameters)'
  end
  object Cancel_Button: TButton
    Left = 240
    Top = 100
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 0
    OnClick = Cancel_ButtonClick
  end
  object OK_Button: TButton
    Left = 99
    Top = 100
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = OK_ButtonClick
  end
end

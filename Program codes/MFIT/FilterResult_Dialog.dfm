object FilterResult_Form: TFilterResult_Form
  Left = 686
  Top = 480
  BorderStyle = bsDialog
  Caption = 'Filtering Results'
  ClientHeight = 136
  ClientWidth = 243
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
  object Label1: TLabel
    Left = 24
    Top = 24
    Width = 154
    Height = 13
    Caption = 'Initial number of observations ='
  end
  object Label2: TLabel
    Left = 24
    Top = 56
    Width = 169
    Height = 13
    Caption = 'Number of observations retained ='
  end
  object Label3: TLabel
    Left = 182
    Top = 24
    Width = 3
    Height = 13
  end
  object Label4: TLabel
    Left = 198
    Top = 56
    Width = 3
    Height = 13
  end
  object Button1: TButton
    Left = 147
    Top = 96
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button1Click
  end
end

object WDerrorMessage: TWDerrorMessage
  Left = 533
  Top = 423
  BorderStyle = bsDialog
  Caption = 'Working directory error'
  ClientHeight = 97
  ClientWidth = 310
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
    Width = 258
    Height = 13
    Caption = 'The working directory does not exist, find a new one?'
  end
  object Ok_Button: TButton
    Left = 64
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Ok'
    TabOrder = 0
    OnClick = Ok_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 175
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Cancel_ButtonClick
  end
end

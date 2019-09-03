object ErrorMessage: TErrorMessage
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Error'
  ClientHeight = 110
  ClientWidth = 339
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
  object ErrorMessageLabel: TLabel
    Left = 24
    Top = 24
    Width = 94
    Height = 13
    Caption = 'Error message here'
  end
  object OkButton: TButton
    Left = 128
    Top = 64
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = OkButtonClick
  end
end

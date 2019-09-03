object WrkDir_Form: TWrkDir_Form
  Left = 500
  Top = 355
  BorderStyle = bsDialog
  Caption = 'Working Directory'
  ClientHeight = 103
  ClientWidth = 442
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
  object Label1: TLabel
    Left = 88
    Top = 24
    Width = 26
    Height = 13
    Caption = 'Path:'
  end
  object WrkDir_Edit: TEdit
    Left = 120
    Top = 21
    Width = 201
    Height = 21
    TabOrder = 0
  end
  object Browse_Button: TButton
    Left = 336
    Top = 19
    Width = 75
    Height = 25
    Caption = 'Browse'
    TabOrder = 1
    OnClick = Browse_ButtonClick
  end
  object OK_Button: TButton
    Left = 128
    Top = 63
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 2
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 230
    Top = 63
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = Cancel_ButtonClick
  end
end

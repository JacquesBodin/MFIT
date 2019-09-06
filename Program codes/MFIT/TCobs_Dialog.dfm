object TCobs_Form: TTCobs_Form
  Left = 467
  Top = 355
  BorderStyle = bsDialog
  Caption = 'Observations'
  ClientHeight = 368
  ClientWidth = 456
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
  object Cancel_Button: TButton
    Left = 367
    Top = 315
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Cancel_ButtonClick
  end
  object OK_Button: TButton
    Left = 286
    Top = 314
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = OK_ButtonClick
  end
  object TCWgrid: TStringGrid
    Left = 8
    Top = 8
    Width = 265
    Height = 331
    ColCount = 4
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goTabs]
    TabOrder = 0
    OnKeyDown = TCWgridKeyDown
    OnSetEditText = TCWgridSetEditText
    ColWidths = (
      64
      64
      64
      64)
    RowHeights = (
      24
      24)
  end
  object InsertRow_Button: TButton
    Left = 285
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Insert Row'
    TabOrder = 3
    OnClick = InsertRow_ButtonClick
  end
  object Delete_Button: TButton
    Left = 366
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Delete'
    TabOrder = 4
    OnClick = Delete_ButtonClick
  end
  object Apply_Button: TButton
    Left = 286
    Top = 187
    Width = 75
    Height = 25
    Caption = 'Apply'
    TabOrder = 5
    OnClick = Apply_ButtonClick
  end
  object AddRows_Button: TButton
    Left = 285
    Top = 39
    Width = 75
    Height = 25
    Caption = 'Add Rows'
    TabOrder = 6
    OnClick = AddRows_ButtonClick
  end
  object SelectAll_Button: TButton
    Left = 285
    Top = 70
    Width = 75
    Height = 25
    Caption = 'Select All'
    TabOrder = 7
    OnClick = SelectAll_ButtonClick
  end
  object Copy_Button: TButton
    Left = 366
    Top = 39
    Width = 75
    Height = 25
    Caption = 'Copy'
    TabOrder = 8
    OnClick = Copy_ButtonClick
  end
  object Paste_Button: TButton
    Left = 366
    Top = 70
    Width = 75
    Height = 25
    Caption = 'Paste'
    TabOrder = 9
    OnClick = Paste_ButtonClick
  end
  object Import_Button: TButton
    Left = 286
    Top = 109
    Width = 75
    Height = 25
    Caption = 'Import'
    TabOrder = 10
    OnClick = Import_ButtonClick
  end
  object Filters_Button: TButton
    Left = 286
    Top = 148
    Width = 75
    Height = 25
    Caption = 'Filter'
    TabOrder = 11
    OnClick = Filters_ButtonClick
  end
  object ProgressBar: TProgressBar
    Left = 0
    Top = 351
    Width = 456
    Height = 17
    Align = alBottom
    TabOrder = 12
  end
  object OpenDialog1: TOpenDialog
    Filter = 'Comma-delimited File|*.csv|Text File|*.txt'
    Left = 392
    Top = 128
  end
end

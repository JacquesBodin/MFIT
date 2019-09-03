[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{59451906-A3BD-420E-8DA4-F0DBB9571B7D}
AppName=MFIT
AppVersion=1.0.0
;AppVerName=MFIT 1.0.0
AppPublisher=Jacques Bodin, University of Poitiers, France
;AppPublisherURL=
;AppSupportURL=
;AppUpdatesURL=
DefaultDirName={pf}\MFIT
DefaultGroupName=MFIT
AllowNoIcons=yes
AlwaysRestart=yes
; Asks for a computer restart after the programm installation
OutputDir=C:\Users\JBodin\Documents\Recherche\MFIT\Setup
OutputBaseFilename=MFIT_setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\JBodin\Documents\Recherche\MFIT\Program codes\MFIT\Win32\Release\MFIT.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\JBodin\Documents\Recherche\MFIT\Program codes\MDMi\Release\MDMi.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\JBodin\Documents\Recherche\MFIT\Program codes\MDMed\Release\MDMed.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\JBodin\Documents\Recherche\MFIT\Program codes\MDP_SFDM\Release\MDP_SFDM.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\JBodin\Documents\Recherche\MFIT\Program codes\MDP_2RNE\Release\MDP_2RNE.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\JBodin\Documents\Recherche\MFIT\Program codes\MDP_2RNE\Release\gsl.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\JBodin\Documents\Recherche\MFIT\Program codes\MDP_2RNE\Release\gslcblas.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\JBodin\Documents\Recherche\MFIT\Pest\*"; DestDir: "{app}\Pest"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.16.27012\vc_redist.x86.exe"; Check: not IsWin64; DestDir: "{tmp}"; Flags: deleteafterinstall
Source: "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.16.27012\vc_redist.x64.exe"; Check: IsWin64; DestDir: "{tmp}"; Flags: deleteafterinstall

[Run]
Filename: "{tmp}\vc_redist.x86.exe"; Parameters: "/install /passive /norestart"; Check: (not VCRedistInstalled) and (not IsWin64)
Filename: "{tmp}\vc_redist.x64.exe"; Parameters: "/install /passive /norestart"; Check: (not VCRedistInstalled) and IsWin64
; Installation of the Microsoft Visual C++ 2017 v14.16.27012.00 redistributable package if this version (or a more recent) has not been found
; See options at https://www.advancedinstaller.com/forums/viewtopic.php?t=34277

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}"; Check: NeedsAddPath(ExpandConstant('{app}'))
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}\Pest"; Check: NeedsAddPath(ExpandConstant('{app}\Pest'))
; Setup of the environment variables
; Prior test (NeedsAddPath) checks if the variables already exist (if true: do nothing)
            
[Icons]
Name: "{group}\MFIT"; Filename: "{app}\MFIT.exe"
Name: "{group}\{cm:UninstallProgram,MFIT}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\MFIT"; Filename: "{app}\MFIT.exe"; Tasks: desktopicon

[Code]

function VCRedistInstalled: Boolean;
 // Returns True if Microsoft Visual C++ 2017 v14.16.27012.00 (or later) redistributable is already installed, otherwise False.
 var
  major: Cardinal;
  minor: Cardinal;
  bld: Cardinal;
  rbld: Cardinal;
  key: String;
 begin
  Result := False;
  if IsWin64 then
  begin
    key := 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64';
  end
  else begin
    key := 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86';
  end;
  if RegQueryDWordValue(HKEY_LOCAL_MACHINE, key, 'Major', major) then begin
    if RegQueryDWordValue(HKEY_LOCAL_MACHINE, key, 'Minor', minor) then begin
      if RegQueryDWordValue(HKEY_LOCAL_MACHINE, key, 'Bld', bld) then begin
        if RegQueryDWordValue(HKEY_LOCAL_MACHINE, key, 'RBld', rbld) then begin
            // Version info was found. Return true if later or equal to our 14.16.27012.00 redistributable
            Result := (major >= 14) and (minor >= 16) and (bld >= 27012) and (rbld >= 0)
        end;
      end;
    end;
  end;
 end;

function NeedsAddPath(Param: string): boolean;
//Checks if the environment variables have to be set (maybe already existing if not the first installation)
 var
  CurrentPaths: string;
begin
  if not RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', CurrentPaths) then
  begin
    Result := True;
    exit;
  end;
  Result := Pos(';' + Param + ';', ';' + CurrentPaths + ';') = 0;
  // Look for the path with leading and trailing semicolon
  // Pos() returns 0 if not found
end;

procedure RemovePath(Param: string);
// Removes the environment variables if the program is uninstalled (see below)
var
  CurrentPaths: string;
  P: Integer;
begin
  if RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', CurrentPaths) then
  begin
    P := Pos(';' + Uppercase(Param) + ';', ';' + Uppercase(CurrentPaths) + ';');
    if P > 0 then
    begin
      if P > 1 then P := P - 1;
      Delete(CurrentPaths, P, Length(Param) + 1);
      RegWriteStringValue(HKEY_LOCAL_MACHINE, 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 'Path', CurrentPaths)
    end
  end
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
// Automatically run during uninstallation
begin
  if CurUninstallStep = usUninstall then
  begin
    RemovePath(ExpandConstant('{app}'));
    RemovePath(ExpandConstant('{app}\Pest'));
  end;
end;

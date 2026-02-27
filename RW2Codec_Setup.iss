; ======================================================================
; Panasonic RW2 WIC Codec - Inno Setup Script
; ======================================================================

[Setup]
; AppId uniquely identifies this application.
AppId={{8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}_Installer}
AppName=Panasonic RW2 WIC Codec
AppVersion=1.0.0
AppPublisher=RW2-WIC-Codec Project
AppPublisherURL=https://github.com/magnum-qin/RW2-WIC-Codec
AppSupportURL=https://github.com/magnum-qin/RW2-WIC-Codec

; Enable modern wizard style (requires Inno Setup 6+)
WizardStyle=modern

; UI Personalization & Modernization
WizardSizePercent=100
WizardResizable=yes
DisableWelcomePage=no
DisableDirPage=yes
DisableProgramGroupPage=yes
DisableReadyPage=yes
DisableFinishedPage=no

; Installation directories
DefaultDirName={autopf}\RW2Codec
DefaultGroupName=Panasonic RW2 WIC Codec

; Output settings
OutputDir=.\build
OutputBaseFilename=RW2Codec_Setup_v1.0.0
Compression=lzma2/ultra64
SolidCompression=yes

; Required for a 64-bit WIC codec
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

; Require admin privileges to register COM
PrivilegesRequired=admin
PrivilegesRequiredOverridesAllowed=dialog

; Visual tweaks
ShowLanguageDialog=auto

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; Dependencies must be extracted BEFORE the codec registers, so regsvr32 can find them.
; Use restartreplace/uninsrestartdelete so if explorer is locking them, Windows replaces them on reboot gracefully.
Source: "scripts\lcms2-2.dll"; DestDir: "{app}"; Flags: ignoreversion 64bit restartreplace uninsrestartdelete
Source: "scripts\raw.dll"; DestDir: "{app}"; Flags: ignoreversion 64bit restartreplace uninsrestartdelete
Source: "scripts\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion 64bit restartreplace uninsrestartdelete

; IMPORTANT: The 'regserver' flag automatically runs regsvr32 during install and regsvr32 /u during uninstall.
; '64bit' flag ensures it is registered in the 64-bit registry.
Source: "build\Release\RW2Codec.dll"; DestDir: "{app}"; Flags: ignoreversion regserver 64bit restartreplace uninsrestartdelete


[Icons]
; Creates an uninstall shortcut in the Start Menu
Name: "{group}\{cm:UninstallProgram,Panasonic RW2 WIC Codec}"; Filename: "{uninstallexe}"

[Code]
// Use SHChangeNotify to refresh icons safely without killing explorer
const
  SHCNE_ASSOCCHANGED = $08000000;
  SHCNF_IDLIST       = $0000;

procedure SHChangeNotify(wEventId: Integer; uFlags: Cardinal; dwItem1, dwItem2: Integer);
  external 'SHChangeNotify@Shell32.dll stdcall';

// Called after installation is finished
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
  end;
end;

// Called after uninstallation is finished
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usPostUninstall then
  begin
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);
  end;
end;

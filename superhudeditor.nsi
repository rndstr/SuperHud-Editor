;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "SuperHud Editor"
  OutFile "superhudeditor-0.3.0-win32.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\superhudeditor"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\superhudeditor" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
;  !define MUI_ICON "images\cpma-trans.ico"

;!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install-nsis.ico"
;!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall-nsis.ico"
 
; MUI Settings / Header
;!define MUI_HEADERIMAGE
;!define MUI_HEADERIMAGE_RIGHT
;!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r-nsis.bmp"
;!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall-r-nsis.bmp"
 
; MUI Settings / Wizard
;!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-nsis.bmp"
;!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall-nsis.bmp"


;--------------------------------
;Pages

  !define MUI_PAGE_CUSTOMFUNCTION_PRE WelcomePageSetupLinkPre
  !define MUI_PAGE_CUSTOMFUNCTION_SHOW WelcomePageSetupLinkShow

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;uninstall.log
!define UninstLog "uninstall.log"
Var UninstLog
;filemissing
LangString UninstLogMissing ${LANG_ENGLISH} "${UninstLog} not found!$\r$\nUninstallation cannot proceed, remove the directory manually."

!macro AddItem Path
  FileWrite $UninstLog "${Path}$\r$\n"
!macroend
!define AddItem "!insertmacro AddItem"

; File macro
!macro File FilePath FileName
 IfFileExists "$OUTDIR\${FileName}" +2
  FileWrite $UninstLog "$OUTDIR\${FileName}$\r$\n"
 File "${FilePath}${FileName}"
!macroend
!define File "!insertmacro File"

; FileOname macro
!macro FileOname FilePath FileName TargetFilename
 IfFileExists "$OUTDIR\${FileName}" +2
  FileWrite $UninstLog "$OUTDIR\${TargetFileName}$\r$\n"
 File /oname=${TargetFilename} "${FilePath}${FileName}"
!macroend
!define FileOname "!insertmacro FileOname"
 
; Copy files macro
!macro CopyFiles SourcePath DestPath
 IfFileExists "${DestPath}" +2
  FileWrite $UninstLog "${DestPath}$\r$\n"
 CopyFiles "${SourcePath}" "${DestPath}"
!macroend
!define CopyFiles "!insertmacro CopyFiles"
 
; Rename macro
!macro Rename SourcePath DestPath
 IfFileExists "${DestPath}" +2
  FileWrite $UninstLog "${DestPath}$\r$\n"
 Rename "${SourcePath}" "${DestPath}"
!macroend
!define Rename "!insertmacro Rename"
 
; CreateDirectory macro
!macro CreateDirectory Path
 CreateDirectory "${Path}"
 FileWrite $UninstLog "${Path}$\r$\n"
!macroend
!define CreateDirectory "!insertmacro CreateDirectory"

; CreateShortcut macro
!macro CreateShortcut FilePath FileTarget
 IfFileExists "${FilePath}" +2
  FileWrite $UninstLog "${FilePath}$\r$\n"
 CreateShortcut "${FilePath}" "${FileTarget}"
!macroend
!define CreateShortcut "!insertmacro CreateShortcut"

; CreateShortcutIcon macro
!macro CreateShortcutIcon FilePath FileTarget Params IconFile IconNo
 IfFileExists "${FilePath}" +2
  FileWrite $UninstLog "${FilePath}$\r$\n"
 CreateShortcut "${FilePath}" "${FileTarget}" "${Params}" "${IconFile}" $IconNo
!macroend
!define CreateShortcutIcon "!insertmacro CreateShortcutIcon"
 
; SetOutPath macro
!macro SetOutPath Path
 SetOutPath "${Path}"
 FileWrite $UninstLog "${Path}$\r$\n"
!macroend
!define SetOutPath "!insertmacro SetOutPath"
 
Section -openlogfile
 CreateDirectory "$INSTDIR"
 IfFileExists "$INSTDIR\${UninstLog}" +3
  FileOpen $UninstLog "$INSTDIR\${UninstLog}" w
 Goto +4
  SetFileAttributes "$INSTDIR\${UninstLog}" NORMAL
  FileOpen $UninstLog "$INSTDIR\${UninstLog}" a
  FileSeek $UninstLog 0 END
SectionEnd


;--------------------------------
;Installer Sections

Section "Core files" SecCORE
  SectionIn RO

  ${SetOutPath} "$INSTDIR"
  
  ;root
  ${File} "" "superhudeditor.exe"
  ${File} "" "webupdater.exe"
  ${FileOname} "" "ChangeLog" "ChangeLog.txt"
  ${FileOname} "" "README" "README.txt"

  ${SetOutPath} "$INSTDIR\data"
  ${File} "data\" "tips.txt"
  ${File} "data\" "pak-0.3.0.pke"

  ${SetOutPath} "$INSTDIR\data\webupdater"
  ${File} "data\webupdater\" "local.xml"
  ${File} "data\webupdater\" "webupdatedlg.xrc"

  ${SetOutPath} "$INSTDIR\Microsoft.VC90.CRT"
  ${File} "Microsoft.VC90.CRT\" "*.dll"
  ${File} "Microsoft.VC90.CRT\" "*.manifest"

  ;Store installation folder
  WriteRegStr HKCU "Software\superhudeditor" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"
    
SectionEnd


Section "Startmenu entry" SecSTARTMENU
  ${CreateDirectory} "$SMPROGRAMS\SuperHud Editor"
  ${CreateShortcut} "$SMPROGRAMS\SuperHud Editor\SuperHud Editor.lnk" "$INSTDIR\superhudeditor.exe"
  ${CreateShortcut} "$SMPROGRAMS\SuperHud Editor\uninstall.lnk" "$INSTDIR\uninstall.exe"
  ${CreateShortcut} "$SMPROGRAMS\SuperHud Editor\WebUpdater.lnk" "$INSTDIR\webupdater.exe"
SectionEnd

Function WelcomePageSetupLinkPre
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "Numfields" "4" ; increase counter
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Bottom" "122" ; limit size of the upper label
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Type" "Link"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Text" "http://plrf.org/superhudeditor"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Left" "120"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Right" "315"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Top" "123"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "Bottom" "132"
FunctionEnd

Function WelcomePageSetupLinkShow
  ; Thanks to pengyou
  ; Fix colors of added link control
  ; See http://forums.winamp.com/showthread.php?s=&threadid=205674
  Push $0
 
  GetDlgItem $0 $MUI_HWND 1203
  SetCtlColors $0 "0000FF" "FFFFFF"
  ; underline font
  CreateFont $1 "$(^Font)" "$(^FontSize)" "400" /UNDERLINE 
  SendMessage $0 ${WM_SETFONT} $1 1 
  Pop $0
 
FunctionEnd
 



;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecCORE ${LANG_ENGLISH} "Application with necessary files. (required)"
  LangString DESC_SecSTARTMENU ${LANG_ENGLISH} "Create a startmenu shortcut."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCORE} $(DESC_SecCORE)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecSTARTMENU} $(DESC_SecSTARTMENU)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section


Section -closelogfile
 FileClose $UninstLog
 SetFileAttributes "$INSTDIR\${UninstLog}" READONLY|SYSTEM|HIDDEN
SectionEnd
 
Section Uninstall
 
 ; Can't uninstall if uninstall log is missing!
 IfFileExists "$INSTDIR\${UninstLog}" +3
  MessageBox MB_OK|MB_ICONSTOP "$(UninstLogMissing)"
   Abort
 
 Push $R0
 Push $R1
 Push $R2
 SetFileAttributes "$INSTDIR\${UninstLog}" NORMAL
 FileOpen $UninstLog "$INSTDIR\${UninstLog}" r
 StrCpy $R1 0
 
 GetLineCount:
  ClearErrors
   FileRead $UninstLog $R0
   IntOp $R1 $R1 + 1
   IfErrors 0 GetLineCount
 
 LoopRead:
  FileSeek $UninstLog 0 SET
  StrCpy $R2 0
  FindLine:
   FileRead $UninstLog $R0
   IntOp $R2 $R2 + 1
   StrCmp $R1 $R2 0 FindLine
 
   StrCpy $R0 $R0 -2
   IfFileExists "$R0\*.*" 0 +3
    RMDir $R0  #is dir
   Goto +3
   IfFileExists $R0 0 +2
    Delete $R0 #is file
 
  IntOp $R1 $R1 - 1
  StrCmp $R1 0 LoopDone
  Goto LoopRead
 LoopDone:
 FileClose $UninstLog
 Delete "$INSTDIR\${UninstLog}"
 RMDir "$INSTDIR"
 Pop $R2
 Pop $R1
 Pop $R0
 
 Delete "$INSTDIR\uninstall.exe"
 RmDir "$INSTDIR"
 ; $LOCALAPPDATA

 DeleteRegKey /ifempty HKCU "Software\superhudeditor"
SectionEnd



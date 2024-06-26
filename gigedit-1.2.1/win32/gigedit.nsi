; gigedit Windows installer

; The name of the installer
Name "gigedit 1.2.1"

; The file to write
OutFile "gigedit_1_2_1_setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\gigedit

LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"

; Probably the best compression ratio
SetCompressor lzma

;--------------------------------
;Version Information

  VIProductVersion "1.2.1.0"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "gigedit"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "a Gigasampler file editor"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "The LinuxSampler Project"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" ""
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "� 2006,2007 Andreas Persson"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "gigedit 1.2.1 installer"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "1.2.1"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; Check for the presence of gtkmm, and if false, ask the user whether to
; download and install gtkmm now from the internet.
Function .onInit
  Var /GLOBAL gtkmmSetupFile

  ; This is just a lazy check for the presence of gtkmm, we should better use:
  ; System::Call function (NSI internal function) to actually call an arbitrary
  ; gtkmm function (/method) from a gtkmm DLL to make it certain
  ReadRegStr $0 HKCU "Software\gtkmm\2.4" "Installer Language"
  IfErrors +2 0
  goto NoAbort
  MessageBox MB_YESNO "gtkmm not found. Install it now (internet connection needed)?" IDYES InstallGtkmm
    MessageBox MB_YESNO "gigedit won't work without gtkmm. Continue anyway?" IDYES NoAbort
      Abort ;  causes installer to quit
  InstallGtkmm:
    ClearErrors
	StrCpy $gtkmmSetupFile $TEMP\gtkmm-win32-runtime-2.10.11-1.exe
    NSISdl::download "http://ftp.gnome.org/pub/gnome/binaries/win32/gtkmm/2.10/gtkmm-win32-runtime-2.10.11-1.exe" $gtkmmSetupFile
    IfErrors 0 +2
	Goto InstallGtkmmFailed
	ExecWait $gtkmmSetupFile
    Delete $gtkmmSetupFile ; we don't need it anymore
	IfErrors 0 +2
	Goto InstallGtkmmFailed
	Goto NoAbort
  InstallGtkmmFailed:
	MessageBox MB_YESNO "Could not download gtkmm. gigedit won't work without gtkmm. Continue anyway?" IDYES NoAbort
      Abort ;  causes installer to quit
  NoAbort:
FunctionEnd

;--------------------------------

; The stuff to install
Section "gigedit 1.2.1"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Put files there
  File gigedit.exe
  File libgigedit.dll
  File libgig.dll
  File libsndfile-1.dll
  File ..\COPYING
  File ..\README
  File ..\NEWS
  File ..\ChangeLog
  File ..\AUTHORS

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gigedit" "DisplayName" "gigedit 1.2.1"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gigedit" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gigedit" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gigedit" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd ; end the section

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\gigedit"
  CreateShortCut "$SMPROGRAMS\gigedit\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\gigedit\gigedit 1.2.1.lnk" "$INSTDIR\gigedit.exe" "" "$INSTDIR\gigedit.exe" 0

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\gigedit"

  ; Remove files and uninstaller
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\gigedit.exe
  Delete $INSTDIR\libgigedit.dll
  Delete $INSTDIR\libgig.dll
  Delete $INSTDIR\libsndfile-1.dll
  Delete $INSTDIR\COPYING
  Delete $INSTDIR\README
  Delete $INSTDIR\NEWS
  Delete $INSTDIR\ChangeLog
  Delete $INSTDIR\AUTHORS

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\gigedit\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\gigedit"
  RMDir "$INSTDIR"

SectionEnd

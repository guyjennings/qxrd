# Modern UI example script
!include MUI2.nsh

Name "QXRD"

OutFile "qxrd-setup-0.3.10.exe"
InstallDir "$PROGRAMFILES\qxrd\qxrd-0.3.10"
InstallDirRegKey HKLM "Software\qxrd\qxrd-0.3.10" "install_dir"
RequestExecutionLevel admin

Var StartMenuFolder

!define MUI_ABORTWARNING
!define MUI_ICON "images\qxrd-icon.ico"
!define MUI_UNICON "images\qxrd-icon.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
!insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\qxrd\qxrd-0.3.10"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Section "Extract qxrd"
  SetOutPath $INSTDIR
  File release\qxrd.exe
  File release\*.dll

  WriteRegStr HKLM "Software\qxrd\qxrd-0.3.10" "install_dir" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder\QXRD-0.3.10"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD-0.3.10\QXRD 0.3.10.lnk" "$INSTDIR\qxrd.exe"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD-0.3.10\Uninstall QXRD 0.3.10.lnk" "$INSTDIR\uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section "Uninstall"

  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\qxrd.exe"
  RMDir  "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD-0.3.10\Uninstall QXRD 0.3.10.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD-0.3.10\QXRD 0.3.10.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder\QXRD-0.3.10"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKLM "Software\qxrd\qxrd-0.3.10"
  DeleteRegKey /ifempty HKLM "Software\qxrd"

SectionEnd

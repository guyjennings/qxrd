# Modern UI example script
!include MUI2.nsh

Name "QXRD 0.3.10"

OutFile "qxrd-setup-0.3.10.exe"
InstallDir "$PROGRAMFILES\qxrd-0.3.10"
InstallDirRegKey HKCU "Software\qxrd-0.3.10" ""
RequestExecutionLevel user

Var StartMenuFolder

!define MUI_ABORTWARNING
!define MUI_ICON "images\qxrd-icon.ico"
!define MUI_UNICON "images\qxrd-icon.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
!insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\qxrd-0.3.10"
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

  WriteRegStr HKCU "Sofware\qxrd-0.3.10" "" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD 0.3.10.lnk" "$INSTDIR\qxrd.exe"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\Uninstall QXRD 0.3.10.lnk" "$INSTDIR\uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section "Uninstall"

  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\qxrd.exe"
  RMDir  "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall QXRD 0.3.10.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD 0.3.10.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\qxrd-0.3.10"

SectionEnd

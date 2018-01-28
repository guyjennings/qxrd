!ifndef VERSION
!define VERSION 0.4.x
!endif

!ifndef PREFIX
!define PREFIX ""
!endif

!ifndef PREFIXSTR
!define PREFIXSTR ""
!endif

!include MUI2.nsh

Name "QXRD"

OutFile "${APPDIR}\qxrd-setup-${VERSION}${PREFIX}.exe"
!ifdef WIN64
InstallDir "$PROGRAMFILES64\qxrd\qxrd-${VERSION}${PREFIX}"
!else
InstallDir "$PROGRAMFILES\qxrd\qxrd-${VERSION}${PREFIX}"
!endif
;InstallDirRegKey HKLM "Software\qxrd\qxrd-${VERSION}${PREFIX}" "install_dir"
RequestExecutionLevel admin

Var StartMenuFolder

!define MUI_ABORTWARNING
!define MUI_ICON "libraries\qxrdlib\images\qxrd-icon.ico"
!define MUI_UNICON "libraries\qxrdlib\images\qxrd-icon.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
!insertmacro MUI_PAGE_DIRECTORY

;!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
;!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\qxrd\qxrd${PREFIX}-${VERSION}"
;!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Section "Extract qxrd"
  SetShellVarContext all
  SetOutPath "$INSTDIR"
  File "${APPDIR}\qxrd.exe"
  File "${APPDIR}\qxrdviewer.exe"
  File "${APPDIR}\vcredist*.exe"
  File "${APPDIR}\*.dll"

  SetOutPath "$INSTDIR\plugins"

  File "${APPDIR}\plugins\*.dll"

  SetOutPath "$INSTDIR\platforms"

  File "${APPDIR}\platforms\*.dll"

  SetOutPath "$INSTDIR"

;  WriteRegStr HKLM "Software\qxrd\qxrd-${VERSION}${PREFIX}" "install_dir" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\ QXRD ${VERSION} ${PREFIXSTR}.lnk" "$INSTDIR\qxrd.exe"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\ QXRDVIEWER ${VERSION} ${PREFIXSTR}.lnk" "$INSTDIR\qxrdviewer.exe"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\Fresh Start.lnk" "$INSTDIR\qxrd.exe" "-fresh"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QXRD ${VERSION} ${PREFIXSTR}" \
                 "DisplayName" "QXRD ${VERSION} ${PREFIXSTR} -- Data Acquisition for Perkin-Elmer XRD Detectors"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QXRD ${VERSION} ${PREFIXSTR}" \
                 "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
  ExecWait "$INSTDIR\vcredist*.exe /install /quiet /norestart"
SectionEnd

Section "Uninstall"
  SetShellVarContext all
  Delete "$INSTDIR\platforms\*.*"
  RMDir  "$INSTDIR\platforms"
  Delete "$INSTDIR\plugins\*.*"
  RMDir  "$INSTDIR\plugins"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\qxrd.exe"
  Delete "$INSTDIR\qxrdviewer.exe"
  Delete "$INSTDIR\vcredist*.exe"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\Uninstall.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\ QXRD ${VERSION} ${PREFIXSTR}.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\ QXRDVIEWER ${VERSION} ${PREFIXSTR}.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\Fresh Start.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

;  DeleteRegKey /ifempty HKLM "Software\qxrd\qxrd${PREFIX}-${VERSION}"
;  DeleteRegKey /ifempty HKLM "Software\qxrd"
  Delete "$INSTDIR\uninstall.exe"
  RMDir  "$INSTDIR"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QXRD ${VERSION} ${PREFIXSTR}"
SectionEnd

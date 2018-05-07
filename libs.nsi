!include MUI2.nsh

Name "QCEPLIBS"

OutFile "${APPDIR}\setup-qceplibs-${VERSION}${PREFIX}.exe"
!ifdef WIN64
InstallDir "$PROGRAMFILES64\qxrd\qxrd-${LIBVERSION}${PREFIX}"
!else
InstallDir "$PROGRAMFILES\qxrd\qxrd-${LIBVERSION}${PREFIX}"
!endif
;InstallDirRegKey HKLM "Software\qxrd\qxrd-${VERSION}${PREFIX}" "install_dir"
RequestExecutionLevel admin

Var StartMenuFolder

!define MUI_ABORTWARNING
!define MUI_ICON   "apps\qxrd-app\qxrd-icon.ico"
!define MUI_UNICON "apps\qxrd-app\qxrd-icon.ico"

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

Section "Extract libs"
  SetShellVarContext all
  SetOutPath "$INSTDIR"
  File "${APPDIR}\vcredist*.exe"
  File "${APPDIR}\*.dll"

  SetOutPath "$INSTDIR\plugins"

  File "${APPDIR}\plugins\*.dll"

  SetOutPath "$INSTDIR\platforms"

  File "${APPDIR}\platforms\*.dll"

  SetOutPath "$INSTDIR"

;  WriteRegStr HKLM "Software\qxrd\qxrd-${VERSION}${PREFIX}" "install_dir" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall-qceplibs.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\Uninstall.lnk" "$INSTDIR\uninstall-qceplibs.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QCEPLIBS ${VERSION} ${PREFIXSTR}" \
                 "DisplayName" "QCEPLIBS ${VERSION} ${PREFIXSTR} -- Libraries for CEP Software"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QCEPLIBS ${VERSION} ${PREFIXSTR}" \
                 "UninstallString" "$\"$INSTDIR\uninstall-qceplibs.exe$\""
  ExecWait "$INSTDIR\vcredist*.exe /install /quiet /norestart"
SectionEnd

Section "Uninstall"
  SetShellVarContext all
  Delete "$INSTDIR\platforms\*.*"
  RMDir  "$INSTDIR\platforms"
  Delete "$INSTDIR\plugins\*.*"
  RMDir  "$INSTDIR\plugins"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\vcredist*.exe"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

;  DeleteRegKey /ifempty HKLM "Software\qxrd\qxrd${PREFIX}-${VERSION}"
;  DeleteRegKey /ifempty HKLM "Software\qxrd"
  Delete "$INSTDIR\uninstall-qceplibs.exe"
  RMDir  "$INSTDIR"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QCEPLIBS ${VERSION} ${PREFIXSTR}"
SectionEnd

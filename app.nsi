!include MUI2.nsh

Name "${TARGETUC}"

OutFile "${APPDIR}\setup-${TARGET}-${VERSION}${PREFIX}.exe"
!ifdef WIN64
InstallDir "$PROGRAMFILES64\qxrd\qxrd-${LIBVERSION}${PREFIX}"
!else
InstallDir "$PROGRAMFILES\qxrd\qxrd-${LIBVERSION}${PREFIX}"
!endif
;InstallDirRegKey HKLM "Software\qxrd\${TARGET}-${VERSION}${PREFIX}" "install_dir"
RequestExecutionLevel admin

Var StartMenuFolder

!define MUI_ABORTWARNING
!define MUI_ICON   "apps\${TARGET}-app\${target}-icon.ico"
!define MUI_UNICON "apps\${TARGET}-app\${target}-icon.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
!insertmacro MUI_PAGE_DIRECTORY

;!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
;!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\qxrd\${TARGET}${PREFIX}-${VERSION}"
;!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Section "Extract"
  SetShellVarContext all
  SetOutPath "$INSTDIR"
  File "${APPDIR}\${TARGET}.exe"

;  WriteRegStr HKLM "Software\qxrd\${TARGET}-${VERSION}${PREFIX}" "install_dir" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall-${TARGET}.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\ ${TARGETUC} ${VERSION} ${PREFIXSTR}.lnk" "$INSTDIR\${TARGET}.exe"
  CreateShortCut  "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\Uninstall ${TARGETUC}.lnk" "$INSTDIR\uninstall-${TARGET}.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TARGETUC} ${VERSION} ${PREFIXSTR}" \
                 "DisplayName" "${TARGETUC} ${VERSION} ${PREFIXSTR} -- ${DESCRIPTION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TARGETUC} ${VERSION} ${PREFIXSTR}" \
                 "UninstallString" "$\"$INSTDIR\uninstall-${TARGET}.exe$\""
SectionEnd

Section "Uninstall"
  SetShellVarContext all
  Delete "$INSTDIR\${TARGET}.exe"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\Uninstall ${TARGETUC}.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\QXRD ${VERSION} ${PREFIXSTR}\ ${TARGETUC} ${VERSION} ${PREFIXSTR}.lnk"

;  DeleteRegKey /ifempty HKLM "Software\qxrd\qxrd${PREFIX}-${VERSION}"
;  DeleteRegKey /ifempty HKLM "Software\qxrd"
  Delete "$INSTDIR\uninstall-${TARGET}.exe"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QXRD ${VERSION} ${PREFIXSTR}"
SectionEnd

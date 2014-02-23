!include "MUI2.nsh"

# Name of the installer
Name "WoW Model Viewer"

# set the name of the installer
outFile "..\..\..\bin\WMV_Installer.exe"

# force installed app to be run as admin
RequestExecutionLevel admin

# custom header fo all pages
# setup header banner
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "..\NSIS_header.bmp"

# License page
!insertmacro MUI_PAGE_LICENSE "..\..\License.rtf"

!insertmacro MUI_PAGE_DIRECTORY
#!define MUI_PAGE_HEADER_TEXT "Installation directory choice"
# set desktop as install directory
InstallDir $PROGRAMFILES32\WoWModelViewer

!insertmacro MUI_PAGE_INSTFILES 

!insertmacro MUI_LANGUAGE "English"
# default section start
Section "Install"
 
# define output path
setOutPath $INSTDIR
 
# specify file to go in output path
!define wmvroot "..\..\..\"
File "${wmvroot}\bin\wowmodelviewer.exe"
File "${wmvroot}\bin\UpdateManager.exe"
File "${wmvroot}\bin\ridable.csv"
File "${wmvroot}\bin_support\MinGW\jpeg62.dll"
File "${wmvroot}\bin_support\MinGW\libgcc_s_dw2-1.dll"
File "${wmvroot}\bin_support\MinGW\libstdc++-6.dll"

CreateDirectory $INSTDIR\plugins
SetOutPath $INSTDIR\plugins
File "${wmvroot}\bin\plugins\*"

CreateDirectory $INSTDIR\mo
SetOutPath $INSTDIR\mo
File "${wmvroot}\src\po\*.mo"

CreateDirectory $INSTDIR\

CreateDirectory $INSTDIR\userSettings

# create shortcuts
setOutPath $INSTDIR
CreateShortCut "$DESKTOP\WoW Model Viewer.lnk" "$INSTDIR\wowmodelviewer.exe" ""
 
# create start-menu items
CreateDirectory "$SMPROGRAMS\WoW Model Viewer"
CreateShortCut "$SMPROGRAMS\WoW Model Viewer\Uninstall.lnk" "$INSTDIR\uninstaller.exe" "" "$INSTDIR\uninstaller.exe" 0
CreateShortCut "$SMPROGRAMS\WoW Model Viewer\WoW Model Viewer.lnk" "$INSTDIR\wowmodelviewer.exe" "" "$INSTDIR\wowmodelviewer.exe" 0

WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\layers" "$INSTDIR\wowmodelviewer.exe" "RUNASADMIN"
WriteRegStr HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\layers" "$INSTDIR\wowmodelviewer.exe" "RUNASADMIN"

# define uninstaller name
writeUninstaller $INSTDIR\uninstaller.exe
 
#-------
# default section end
sectionEnd
 
# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
section "Uninstall"
 
# Always delete uninstaller first
delete $INSTDIR\uninstaller.exe
 
# now delete installed file
RMDir /r "$INSTDIR\*.*"    
 
# Remove the installation directory
RMDir "$INSTDIR"
 
# Delete Shortcuts
Delete "$DESKTOP\WoW Model Viewer.lnk"
Delete "$SMPROGRAMS\WoW Model Viewer\*.*"
RMDir "$SMPROGRAMS\WoW Model Viewer"

# cleanup reg
DeleteRegKey HKLM "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\layers"
DeleteRegKey HKCU "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\layers"
 
sectionEnd
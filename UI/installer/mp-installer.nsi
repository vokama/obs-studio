; Script generated with the Venis Install Wizard

Unicode true

; Define your application name
!define APPNAME "VK Games"

!ifndef APPVERSION
!define APPVERSION "1.0"
!define SHORTVERSION "1.0"
!endif

!define APPNAMEANDVERSION "VK Games ${SHORTVERSION}"
!define FULL
; !define REALSENSE_PLUGIN

; Additional script dependencies
!include WinVer.nsh
!include x64.nsh

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES32\vk-games"
InstallDirRegKey HKLM "Software\${APPNAME}" ""
!ifdef FULL
OutFile "VK-Games-${SHORTVERSION}-Full-Installer.exe"
!else
OutFile "VK-Games-${SHORTVERSION}-Small-Installer.exe"
!endif

; Use compression
SetCompressor /SOLID LZMA

; Need Admin
RequestExecutionLevel admin

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_TEXT "Запустить VK Games ${SHORTVERSION}"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchOBS"

!define MUI_PAGE_CUSTOMFUNCTION_LEAVE PreReqCheck

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "data\obs-studio\license\gplv2.txt"
!insertmacro MUI_PAGE_DIRECTORY
!ifdef FULL
	!insertmacro MUI_PAGE_COMPONENTS
!endif
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

;!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_COMPONENTS
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_RESERVEFILE_LANGDLL

Function PreReqCheck
	; Abort on XP or lower
	${If} ${AtMostWinXP}
		MessageBox MB_OK|MB_ICONSTOP "В связи с активным использованием возможностей DirectX 10, ${APPNAME} нуждается в Windows Vista SP2 или новее и не может быть установлена на данной версии Windows."
		Quit
	${EndIf}

	; Vista specific checks
	${If} ${IsWinVista}
		; Check Vista SP2
		${If} ${AtMostServicePack} 1
			MessageBox MB_YESNO|MB_ICONEXCLAMATION "Для использования ${APPNAME} под Vista, необходим пакет обновления 2 (SP2). Желаете скачать его?" IDYES sptrue IDNO spfalse
			sptrue:
				ExecShell "open" "https://support.microsoft.com/ru-ru/help/13858/windows-vista-service-pack-2-install"
			spfalse:
			Quit
		${EndIf}

		; Check Vista Platform Update
		nsexec::exectostack "$SYSDIR\wbem\wmic.exe qfe where HotFixID='KB971512' get HotFixID /Format:list"
		pop $0
		pop $0
		strcpy $1 $0 17 6
		strcmps $1 "HotFixID=KB971512" gotPatch
			MessageBox MB_YESNO|MB_ICONEXCLAMATION "Для использования ${APPNAME} необходимо обновление Windows Vista. Желаете скачать его?" IDYES putrue IDNO pufalse
			putrue:
				${If} ${RunningX64}
					; 64 bit
					ExecShell "open" "http://www.microsoft.com/ru-ru/download/details.aspx?id=4390"
				${Else}
					; 32 bit
					ExecShell "open" "http://www.microsoft.com/ru-ru/download/details.aspx?id=3274"
				${EndIf}
			pufalse:
			Quit
		gotPatch:
	${EndIf}

	ClearErrors
	GetDLLVersion "MSVCR120.DLL" $R0 $R1
	IfErrors vs2013Missing vs2013OK
	vs2013Missing:
		MessageBox MB_YESNO|MB_ICONEXCLAMATION "В вашей системе недостаёт компонентов, необходимых для работы ${APPNAME}. Пожалуйста, обратите внимание на то, что необходимо установить как vcredist_x64, так и vcredist_x86. Желаете скачать их?" IDYES vs2013true IDNO vs2013false
		vs2013true:
			ExecShell "open" "http://www.microsoft.com/ru-ru/download/details.aspx?id=40784"
		vs2013false:
		Quit
	vs2013OK:
	ClearErrors

	; DirectX Version Check
	ClearErrors
	GetDLLVersion "D3DCompiler_33.dll" $R0 $R1
	IfErrors dxMissing33 dxOK
	dxMissing33:
	ClearErrors
	GetDLLVersion "D3DCompiler_34.dll" $R0 $R1
	IfErrors dxMissing34 dxOK
	dxMissing34:
	ClearErrors
	GetDLLVersion "D3DCompiler_35.dll" $R0 $R1
	IfErrors dxMissing35 dxOK
	dxMissing35:
	ClearErrors
	GetDLLVersion "D3DCompiler_36.dll" $R0 $R1
	IfErrors dxMissing36 dxOK
	dxMissing36:
	ClearErrors
	GetDLLVersion "D3DCompiler_37.dll" $R0 $R1
	IfErrors dxMissing37 dxOK
	dxMissing37:
	ClearErrors
	GetDLLVersion "D3DCompiler_38.dll" $R0 $R1
	IfErrors dxMissing38 dxOK
	dxMissing38:
	ClearErrors
	GetDLLVersion "D3DCompiler_39.dll" $R0 $R1
	IfErrors dxMissing39 dxOK
	dxMissing39:
	ClearErrors
	GetDLLVersion "D3DCompiler_40.dll" $R0 $R1
	IfErrors dxMissing40 dxOK
	dxMissing40:
	ClearErrors
	GetDLLVersion "D3DCompiler_41.dll" $R0 $R1
	IfErrors dxMissing41 dxOK
	dxMissing41:
	ClearErrors
	GetDLLVersion "D3DCompiler_42.dll" $R0 $R1
	IfErrors dxMissing42 dxOK
	dxMissing42:
	ClearErrors
	GetDLLVersion "D3DCompiler_43.dll" $R0 $R1
	IfErrors dxMissing43 dxOK
	dxMissing43:
	ClearErrors
	GetDLLVersion "D3DCompiler_47.dll" $R0 $R1
	IfErrors dxMissing47 dxOK
	dxMissing47:
	MessageBox MB_YESNO|MB_ICONEXCLAMATION "В вашей системе недостаёт компонентов DirectX, необходимых для работы ${APPNAME}. Желаете скачать их?" IDYES dxtrue IDNO dxfalse
	dxtrue:
		ExecShell "open" "https://obsproject.com/go/dxwebsetup"
	dxfalse:
	Quit
	dxOK:
	ClearErrors

	; Check previous instance

	OBSInstallerUtils::IsProcessRunning "obs32.exe"
	IntCmp $R0 1 0 notRunning1
		MessageBox MB_OK|MB_ICONEXCLAMATION "${APPNAME} уже работает. Пожалуйста, закройте его, прежде чем установить новую версию." /SD IDOK
		Quit
	notRunning1:

	${if} ${RunningX64}
		OBSInstallerUtils::IsProcessRunning "obs64.exe"
		IntCmp $R0 1 0 notRunning2
			MessageBox MB_OK|MB_ICONEXCLAMATION "${APPNAME} уже работает. Пожалуйста,закройте его, прежде чем установить новую версию." /SD IDOK
			Quit
		notRunning2:
	${endif}

	OBSInstallerUtils::AddInUseFileCheck "$INSTDIR\data\obs-plugins\win-capture\graphics-hook32.dll"
	OBSInstallerUtils::AddInUseFileCheck "$INSTDIR\data\obs-plugins\win-capture\graphics-hook64.dll"
	OBSInstallerUtils::GetAppNameForInUseFiles
	StrCmp $R0 "" gameCaptureNotRunning
		MessageBox MB_OK|MB_ICONEXCLAMATION "Захват игры всё ещё используется следующими приложениями:$\r$\n$\r$\n$R0$\r$\nПожалуйста, закройте эти приложения, прежде чем устанавливать новую версию VK Games." /SD IDOK
		Quit
	gameCaptureNotRunning:
FunctionEnd

Function filesInUse
	MessageBox MB_OK|MB_ICONEXCLAMATION "Некоторые файлы не могли быть установлены. Если вы устанавливаете VK Games впервые, пожалуйста, отключите любые антивирусы или другое защитное программное обеспечение и попробуйте снова. Если вы переустанавливаете или обновляете VK Games, закройте любые приложения, которые могут быть с ним связаны, или перезагрузите компьютер и попробуйте снова."  /SD IDOK
FunctionEnd

Function LaunchOBS
	${if} ${RunningX64}
		Exec '"$WINDIR\explorer.exe" "$SMPROGRAMS\VK Games\VK Games (64bit).lnk"'
	${else}
		Exec '"$WINDIR\explorer.exe" "$SMPROGRAMS\VK Games\VK Games (32bit).lnk"'
	${endif}
FunctionEnd

Var outputErrors

Section "OBS Studio" SecCore

	; Set Section properties
	SectionIn RO
	SetOverwrite on
	AllowSkipFiles off

	SetShellVarContext all

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR"
	OBSInstallerUtils::KillProcess "obs-plugins\32bit\cef-bootstrap.exe"
	OBSInstallerUtils::KillProcess "obs-plugins\64bit\cef-bootstrap.exe"
	File /r "data"
	SetOutPath "$INSTDIR\bin"
	File /r "bin\32bit"
	SetOutPath "$INSTDIR\obs-plugins"
	File /r "obs-plugins\32bit"

	${if} ${RunningX64}
		SetOutPath "$INSTDIR\bin"
		File /r "bin\64bit"
		SetOutPath "$INSTDIR\obs-plugins"
		File /r "obs-plugins\64bit"
	${endif}

	ClearErrors

	IfErrors 0 +2
		StrCpy $outputErrors "yes"

	WriteUninstaller "$INSTDIR\uninstall.exe"

	; Delete Old "Multiplatform" Shortcuts
	Delete "$DESKTOP\OBS Multiplatform.lnk"
	Delete "$SMPROGRAMS\OBS Multiplatform\OBS Multiplatform (32bit).lnk"
	Delete "$SMPROGRAMS\OBS Multiplatform\Uninstall.lnk"
	${if} ${RunningX64}
		Delete "$SMPROGRAMS\OBS Multiplatform\OBS Multiplatform (64bit).lnk"
	${endif}

	${if} ${RunningX64}
		SetOutPath "$INSTDIR\bin\64bit"
		CreateShortCut "$DESKTOP\VK Games.lnk" "$INSTDIR\bin\64bit\obs64.exe"
	${else}
		SetOutPath "$INSTDIR\bin\32bit"
		CreateShortCut "$DESKTOP\VK Games.lnk" "$INSTDIR\bin\32bit\obs32.exe"
	${endif}
	SetOutPath "$INSTDIR\bin\32bit"
	CreateDirectory "$SMPROGRAMS\VK Games"
	CreateShortCut "$SMPROGRAMS\VK Games\VK Games (32bit).lnk" "$INSTDIR\bin\32bit\obs32.exe"
	CreateShortCut "$SMPROGRAMS\VK Games\Uninstall.lnk" "$INSTDIR\uninstall.exe"

	${if} ${RunningX64}
		SetOutPath "$INSTDIR\bin\64bit"
		CreateShortCut "$SMPROGRAMS\VK Games\VK Games (64bit).lnk" "$INSTDIR\bin\64bit\obs64.exe"
	${endif}

	SetOutPath "$INSTDIR\bin\32bit"

	StrCmp $outputErrors "yes" 0 +2
		Call filesInUse
SectionEnd

!ifdef FULL
SectionGroup /e "Plugins" SecPlugins
	Section "Browser plugin" SecPlugins_Browser
		; Set Section properties
		SetOverwrite on
		AllowSkipFiles off
		SetShellVarContext all

		SetOutPath "$INSTDIR\obs-plugins"
		OBSInstallerUtils::KillProcess "32bit\cef-bootstrap.exe"
		File /r "obs-browser\obs-plugins\32bit"

		${if} ${RunningX64}
			OBSInstallerUtils::KillProcess "64bit\cef-bootstrap.exe"
			File /r "obs-browser\obs-plugins\64bit"
		${endif}

		SetOutPath "$INSTDIR\bin\32bit"
	SectionEnd

	!ifdef REALSENSE_PLUGIN
	Section /o "Realsense plugin" SecPlugins_Realsense
		SetOverwrite on
		AllowSkipFiles off
		SetShellVarContext all

		SetOutPath "$INSTDIR\obs-plugins"
		File /r "realsense\32bit"

		${if} ${RunningX64}
			File /r "realsense\64bit"
		${endif}

		SetOutPath "$INSTDIR\data\obs-plugins"
		File /r "realsense\data\obs-plugins\win-ivcam"

		ExecWait '"$INSTDIR\data\obs-plugins\win-ivcam\seg_service.exe" /UnregServer'
		ExecWait '"$INSTDIR\data\obs-plugins\win-ivcam\seg_service.exe" /RegServer'

		ReadRegStr $0 HKLM "Software\Intel\RSSDK\Dispatch" "Core"
		${if} ${Errors}
			ReadRegStr $0 HKLM "Software\Intel\RSSDK\v10\Dispatch" "Core"
		${endif}

		${if} ${Errors}
			InitPluginsDir
			SetOutPath "$PLUGINSDIR\realsense"

			File "intel_rs_sdk_runtime_websetup_10.0.26.0396.exe"
			ExecWait '"$PLUGINSDIR\realsense\intel_rs_sdk_runtime_websetup_10.0.26.0396.exe" --finstall=personify --fnone=all'
		${endif}

		SetOutPath "$INSTDIR\bin\32bit"
	SectionEnd
	!endif
SectionGroupEnd
!endif

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "ProductID" "d16d2409-3151-4331-a9b1-dfd8cf3f0d9c"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$INSTDIR\bin\32bit\obs32.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "OBS Project"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "HelpLink" "https://obsproject.com"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" "${APPVERSION}"

SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SecCore} "Core OBS Studio files"
	!ifdef FULL
		!insertmacro MUI_DESCRIPTION_TEXT ${SecPlugins} "Optional Plugins"
		!insertmacro MUI_DESCRIPTION_TEXT ${SecPlugins_Browser} "Browser plugin (a source you can add to your scenes that displays web pages)"
		!ifdef REALSENSE_PLUGIN
			!insertmacro MUI_DESCRIPTION_TEXT ${SecPlugins_Realsense} "Plugin for Realsense cameras"
		!endif
	!endif
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section "un.obs-studio Program Files" UninstallSection1

	SectionIn RO

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	Delete "$DESKTOP\VK Games.lnk"
	Delete "$SMPROGRAMS\VK Games\VK Games (32bit).lnk"
	Delete "$SMPROGRAMS\VK Games\Uninstall.lnk"
	${if} ${RunningX64}
		Delete "$SMPROGRAMS\VK Games\VK Games (64bit).lnk"
	${endif}

	IfFileExists "$INSTDIR\data\obs-plugins\win-ivcam\seg_service.exe" UnregisterSegService SkipUnreg
	UnregisterSegService:
	ExecWait '"$INSTDIR\data\obs-plugins\win-ivcam\seg_service.exe" /UnregServer'
	SkipUnreg:

	; Clean up OBS Studio
	RMDir /r "$INSTDIR\bin"
	RMDir /r "$INSTDIR\data"
	RMDir /r "$INSTDIR\obs-plugins"
	RMDir "$INSTDIR"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\VK Games"
	RMDir "$INSTDIR\VK Games"
SectionEnd

Section "un.User Settings" UninstallSection2
	RMDir /R "$APPDATA\vk-games"
SectionEnd

!insertmacro MUI_UNFUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${UninstallSection1} "Удалить все программные файлы VK Games."
	!insertmacro MUI_DESCRIPTION_TEXT ${UninstallSection2} "Удалить все настройки, плагины, сцены и источники, профили, лог-файлы и другие данные приложения."
!insertmacro MUI_UNFUNCTION_DESCRIPTION_END

; Version information
VIProductVersion "${APPVERSION}.0.0"
VIAddVersionKey /LANG=${LANG_RUSSIAN} "ProductName" "VK Games"
VIAddVersionKey /LANG=${LANG_RUSSIAN} "CompanyName" "vk.com"
VIAddVersionKey /LANG=${LANG_RUSSIAN} "LegalCopyright" "(c) 2012-2016"
; FileDescription is what shows in the UAC elevation prompt when signed
VIAddVersionKey /LANG=${LANG_RUSSIAN} "FileDescription" "VK Games"
VIAddVersionKey /LANG=${LANG_RUSSIAN} "FileVersion" "1.0"

; eof

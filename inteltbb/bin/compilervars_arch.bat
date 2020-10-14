@echo off
:: ============================================================================
:: Copyright 1985-2019 Intel Corporation All Rights Reserved.
::
:: The source code,  information and material ("Material")  contained herein is
:: owned by Intel Corporation or its suppliers or licensors,  and title to such
:: Material remains with Intel Corporation or its suppliers  or licensors.  The
:: Material contains  proprietary  information  of Intel  or its  suppliers and
:: licensors.  The Material is protected by worldwide copyright laws and treaty
:: provisions.  No part  of the  Material   may be  used,  copied,  reproduced,
:: modified,  published,   uploaded,   posted,   transmitted,   distributed  or
:: disclosed in any way without Intel's prior  express written  permission.  No
:: license under any patent,  copyright  or other intellectual  property rights
:: in the Material is granted to or conferred upon you,  either  expressly,  by
:: implication,  inducement,  estoppel  or otherwise.  Any  license  under such
:: intellectual  property  rights must  be  express and  approved  by  Intel in
:: writing.
::
:: Unless otherwise  agreed by  Intel in writing,  you may not  remove or alter
:: this notice or  any other notice  embedded in Materials by  Intel or Intel's
:: suppliers or licensors in any way.
:: ============================================================================

:: Cache some environment variables.
set ROOT=%~dp0
call :GetFullPath "%ROOT%\.." ROOT
set SCRIPT_NAME=%~nx0

:: Set the default arguments
set TARGET_ARCH=
set TARGET_VS=
set TARGET_VS_ARCH=
set TARGET_PLATFORM=
set QUIET=
set ARCH_PATH=
set NDK_ARCH=
set WIN_TITLE_VS=
set WIN_TITLE_ARCH=

:ParseArgs
:: Parse the incoming arguments
if /i "%1"==""        goto Checkargs
if /i "%1"=="ia32"         (set TARGET_ARCH=ia32)          & (set TARGET_VS_ARCH=x86)       & (set ARCH_PATH=ia32)    & shift & goto ParseArgs
if /i "%1"=="intel64"      (set TARGET_ARCH=intel64)       & (set TARGET_VS_ARCH=amd64)     & (set ARCH_PATH=intel64) & shift & goto ParseArgs
if /i "%1"=="vs2013"       (set TARGET_VS=vs2013) & (set WIN_TITLE_VS=Visual Studio 2013) & shift & goto ParseArgs
if /i "%1"=="vs2015"       (set TARGET_VS=vs2015) & (set WIN_TITLE_VS=Visual Studio 2015) & shift & goto ParseArgs
if /i "%1"=="vs2017"       (set TARGET_VS=vs2017) & (set WIN_TITLE_VS=Visual Studio 2017) & shift & goto ParseArgs
if /i "%1"=="vs2019"       (set TARGET_VS=vs2019) & (set WIN_TITLE_VS=Visual Studio 2019) & shift & goto ParseArgs
if /i "%1"=="quiet"        (set QUIET=quiet)      & shift & goto ParseArgs
if /i "%1"=="linux"        (set TARGET_PLATFORM=linux)      & shift & goto ParseArgs
if /i "%1"=="android"      (set TARGET_PLATFORM=android)    & shift & goto ParseArgs
goto Error

:Checkargs
:: target architecture is mandatory
if /i "%TARGET_ARCH%"=="" goto Syntax
goto Build

:Build
:: set variables
set PRODUCT_NAME=Intel Compiler 19.1
set PRODUCT_NAME_FULL=Intel(R) Compiler 19.1 (package 166)
set NDK_DIR=""

:: set calculated variables
if /i "%TARGET_ARCH%"=="ia32"         set WIN_TITLE_ARCH=IA-32
if /i "%TARGET_ARCH%"=="intel64"      set WIN_TITLE_ARCH=Intel(R) 64
set WIN_TITLE=%PRODUCT_NAME% %WIN_TITLE_ARCH% %WIN_TITLE_VS%
title %WIN_TITLE%

if defined TARGET_PLATFORM goto LinuxTarget

::detect installed VS
set MSVS_VAR_SCRIPT=

::default, set the latest VS in global environment
if /i "%TARGET_VS%"=="" (
    ::vs2019
    if defined VS2019INSTALLDIR (
        if exist "%VS2019INSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" (
            goto SetVS2019
        )
    )

    ::vs2017
    if defined VS2017INSTALLDIR (
        if exist "%VS2017INSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" (
            goto SetVS2017
        )
    )

    ::vs2015
    if defined VS140COMNTOOLS (
        if exist "%VS140COMNTOOLS%..\IDE\devenv.exe" (
            if exist "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" (
                goto SetVS2015
            )
        )
        if exist "%VS140COMNTOOLS%..\IDE\WDExpress.exe" (
            if exist "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" (
                if /i "%TARGET_ARCH%"=="intel64" (
                    set TARGET_VS_ARCH=x86_amd64
                )
                goto SetVS2015
            )
        )
    )
    ::vs2013
    if defined VS120COMNTOOLS (
        if exist "%VS120COMNTOOLS%..\IDE\devenv.exe" (
            if exist "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" (
                goto SetVS2013
            )
        )
        if exist "%VS120COMNTOOLS%..\IDE\WDExpress.exe" (
            if exist "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" (
                if /i "%TARGET_ARCH%"=="intel64" (
                    set TARGET_VS_ARCH=x86_amd64
                )
                goto SetVS2013
            )
        )
    )
    ::vsshell
    if defined VSSHELL2015INSTALLDIR (
        if exist "%ROOT%\bin\vsshell2015vars_arch.bat" (
            set MSVS_VAR_SCRIPT="%ROOT%\bin\vsshell2015vars_arch.bat"
            goto Calls
        )
    )
    if defined VSSHELL2013INSTALLDIR (
        if exist "%ROOT%\bin\vsshell2013vars_arch.bat" (
            set MSVS_VAR_SCRIPT="%ROOT%\bin\vsshell2013vars_arch.bat"
            goto Calls
        )
    )
    ::ia32 special
    if /i "%TARGET_ARCH%"=="ia32" (
        if defined VS140COMNTOOLS (
            if exist "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" (
                goto SetVS2015
            )
        )
        if defined VS120COMNTOOLS (
            if exist "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" (
                goto SetVS2013
            )
        )
    )
    call :no_vs 2013, 2015, 2017 or 2019
    goto End
)

::VS2019
if /i "%TARGET_VS%"=="vs2019" (
    if defined VS2019INSTALLDIR (
        if exist "%VS2019INSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" (
        goto SetVS2019
        )
    )
    call :no_vs 2019
    goto End
)

::VS2017
if /i "%TARGET_VS%"=="vs2017" (
    if defined VS2017INSTALLDIR (
        if exist "%VS2017INSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" (
        goto SetVS2017
        )
    )
    call :no_vs 2017
    goto End
)

::VS2015
if /i "%TARGET_VS%"=="vs2015" (
    if defined VS140COMNTOOLS (
        if exist "%VS140COMNTOOLS%..\IDE\devenv.exe" (
            if exist "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" (
                goto SetVS2015
            )
        )
        if exist "%VS140COMNTOOLS%..\IDE\WDExpress.exe" (
            if exist "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" (
                if /i "%TARGET_ARCH%"=="intel64" (
                    set TARGET_VS_ARCH=x86_amd64
                )
                goto SetVS2015
            )
        )
    )

    if defined VSSHELL2015INSTALLDIR (
        if exist "%ROOT%\bin\vsshell2015vars_arch.bat" (
            set MSVS_VAR_SCRIPT="%ROOT%\bin\vsshell2015vars_arch.bat"
            goto Calls
        )
    )
    ::ia32 special
    if /i "%TARGET_ARCH%"=="ia32" (
        if defined VS140COMNTOOLS (
            if exist "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" (
                goto SetVS2015
            )
        )
    )
    call :no_vs 2015
    goto End
)

::VS2013
if /i "%TARGET_VS%"=="vs2013" (
    if defined VS120COMNTOOLS (
        if exist "%VS120COMNTOOLS%..\IDE\devenv.exe" (
            if exist "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" (
                goto SetVS2013
            )
        )
        if exist "%VS120COMNTOOLS%..\IDE\WDExpress.exe" (
            if exist "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" (
                if /i "%TARGET_ARCH%"=="intel64" set TARGET_VS_ARCH=x86_amd64
                goto SetVS2013
            )
        )
    )

    if defined VSSHELL2013INSTALLDIR (
        if exist "%ROOT%\bin\vsshell2013vars_arch.bat" (
            set MSVS_VAR_SCRIPT="%ROOT%\bin\vsshell2013vars_arch.bat"
            goto Calls
        )
    )

    if /i "%TARGET_ARCH%"=="ia32" (
        if defined VS120COMNTOOLS (
            if exist "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" (
                goto SetVS2013
            )
        )
    )
    call :no_vs 2013
    goto End
)
:SetVS2019
set TARGET_VS=vs2019
set MSVS_VAR_SCRIPT="%VS2019INSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat"
goto Calls

:SetVS2017
set TARGET_VS=vs2017
set MSVS_VAR_SCRIPT="%VS2017INSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat"
goto Calls

:SetVS2015
set "__MS_VC_INSTALL_PATH=%VS140COMNTOOLS%..\..\VC"
set MSVS_VAR_SCRIPT="%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
goto Calls

:SetVS2013
set "__MS_VC_INSTALL_PATH=%VS120COMNTOOLS%..\..\VC"
set MSVS_VAR_SCRIPT="%VS120COMNTOOLS%..\..\VC\vcvarsall.bat"
goto Calls

:LinuxTarget
::android
if /i "%TARGET_ARCH%"=="ia32"    set NDK_ARCH=x86
if /i "%TARGET_ARCH%"=="intel64" set NDK_ARCH=x86_64
:: set Android variables
FOR %%A IN (9 12 13 14 15 16 17 18 19 21 22 23 24 26) DO (
    if exist "%NDK_DIR:~1,-1%\platforms\android-%%A\arch-%NDK_ARCH%" (
        set ANDROID_API=%%A
        set "ANDROID_SYSROOT=%NDK_DIR:~1,-1%\platforms\android-%%A\arch-%NDK_ARCH%"
        goto EndAndroidAPI
    )
)
:EndAndroidAPI

if exist "%NDK_DIR:~1,-1%\toolchains\%NDK_ARCH%-4.9\prebuilt\windows-x86_64" (
    set "ANDROID_GNU_X86_TOOLCHAIN=%NDK_DIR:~1,-1%\toolchains\%NDK_ARCH%-4.9\prebuilt\windows-x86_64"
)
if defined ANDROID_GNU_X86_TOOLCHAIN (
    set "PATH=%ANDROID_GNU_X86_TOOLCHAIN%\bin;%NDK_DIR:~1,-1%\toolchains\llvm\prebuilt\windows-x86_64\bin;%PATH%"
)

:SetCFG
if /i "%TARGET_PLATFORM%"=="android" (
    set "ICCCFG=%ROOT%\bin\intel64\icc_and.cfg"
    set "ICPCCFG=%ROOT%\bin\intel64\icpc_and.cfg"
    set "ICXCFG=%ROOT%\bin\intel64\icx_and.cfg"
    set "ICPXCFG=%ROOT%\bin\intel64\icpx_and.cfg"
    set "XIARCFG=%ROOT%\bin\intel64\xiar_and.cfg"
    set "XILDCFG=%ROOT%\bin\intel64\xild_and.cfg"
) else (
    set ICCCFG=
    set ICPCCFG=
    set ICXCFG=
    set ICPXCFG=
    set XIARCFG=
    set XILDCFG=
    set INTEL_USE_ENVIRONMENT=
)

:Calls
:: print product info
if /i "%QUIET%"=="quiet" goto quietmode
    echo Copyright (C) 1985-2019 Intel Corporation. All rights reserved.
:quietmode
echo %PRODUCT_NAME_FULL%

:: call visual studio VARs script
echo.
if not defined MSVS_VAR_SCRIPT goto :set_bin_end

if "%TARGET_VS%"=="vs2019" (
    if "%VSCMD_START_DIR%"=="" (
        if EXIST "%USERPROFILE%\Source" (
        set "VSCMD_START_DIR=%CD%"
        )
    )
)

if "%TARGET_VS%"=="vs2017" (
    if "%VSCMD_START_DIR%"=="" (
        if EXIST "%USERPROFILE%\Source" (
        set "VSCMD_START_DIR=%CD%"
        )
    )
)

@call %MSVS_VAR_SCRIPT% %TARGET_VS_ARCH%

call :GetFullPath %MSVS_VAR_SCRIPT%\.. MSVS_VAR_SCRIPT_DIR
if /i "%TARGET_ARCH%"=="ia32" (
    if "%TARGET_VS%"=="vs2019" (
        if defined VCToolsInstallDir (
            if exist "%VCToolsInstallDir%\bin\HostX64\x64" (
                set "PATH=%PATH%;%VCToolsInstallDir%\bin\HostX64\x64"
                goto set_dll_end
            )
        )
    )

    if "%TARGET_VS%"=="vs2017" (
        if defined VCToolsInstallDir (
            if exist "%VCToolsInstallDir%\bin\HostX64\x64" (
                set "PATH=%PATH%;%VCToolsInstallDir%\bin\HostX64\x64"
                goto set_dll_end
            )
        )
    )

    if exist "%MSVS_VAR_SCRIPT_DIR%\bin\amd64" (
        set "PATH=%PATH%;%MSVS_VAR_SCRIPT_DIR%\bin\amd64"
        goto set_dll_end
    )

    if not "%ROOT%\bin" EQU "%MSVS_VAR_SCRIPT_DIR%" (
        echo "Warning: no path to 64-bits dll for reading and writing the pdb (for example, mspdb140.dll)"
    )
)
:set_dll_end

if /i "%TARGET_VS%"=="vs2019" (
    if defined VCToolsInstallDir (
        set "__MS_VC_INSTALL_PATH=%VCToolsInstallDir%"
    )
)

if /i "%TARGET_VS%"=="vs2017" (
    if defined VCToolsInstallDir (
        set "__MS_VC_INSTALL_PATH=%VCToolsInstallDir%"
    )
)

if exist "%ROOT%\..\linux\compiler\lib\intel64_lin" (
    if defined LD_LIBRARY_PATH (
        set "LD_LIBRARY_PATH=%ROOT%\..\linux\compiler\lib\intel64_lin;%LD_LIBRARY_PATH%"
    ) else (
        set "LD_LIBRARY_PATH=%ROOT%\..\linux\compiler\lib\intel64_lin"
    )
)
:set_bin_end

set "PATH=%ROOT%\bin\intel64;%ROOT%\bin;%ROOT%\redist\%ARCH_PATH%\compiler;%ROOT%\redist\%ARCH_PATH%_win\compiler;%PATH%"
if /i "%TARGET_ARCH%"=="ia32" (
    set "PATH=%PATH%;%ROOT%\redist\%ARCH_PATH%_win\mpirt"
    set "INTEL_TARGET_ARCH_IA32=ia32"
) else (
    if defined INTEL_TARGET_ARCH_IA32 (set INTEL_TARGET_ARCH_IA32=)
)

set LIB=%ROOT%\compiler\lib;%ROOT%\compiler\lib\%ARCH_PATH%_win;%LIB%
set MIC_LD_LIBRARY_PATH=%ROOT%\compiler\lib\mic;%MIC_LD_LIBRARY_PATH%
set MIC_LIBRARY_PATH=%ROOT%\compiler\lib\mic;%MIC_LIBRARY_PATH%
set INCLUDE=%ROOT%\compiler\include;%ROOT%\compiler\include\%ARCH_PATH%;%INCLUDE%


:: end
goto End

:Error
echo Invalid command line argument: %1
echo.
exit /B 1

:Syntax
echo Syntax:
echo  %SCRIPT_NAME% ^<arch^> ^<platform^>
echo.
echo    ^<arch^> must be is one of the following
echo        ia32         : Set up for IA-32 target
echo        intel64      : Set up for Intel(R) 64 target
echo    If specified, ^<vs^> must be one of the following
echo        vs2013      : Set to use Microsoft Visual Studio 2013
echo        vs2015      : Set to use Microsoft Visual Studio 2015
echo        vs2017      : Set to use Microsoft Visual Studio 2017
echo        vs2019      : Set to use Microsoft Visual Studio 2019
echo        none        : Set to not use Microsoft Visual Studio
echo    If ^<vs^> is not specified, the version of Visual Studio detected at install
echo    time is used.
echo    ^<platform^> must be of the following.
echo        linux          : Set to Linux target.
echo        android        : Set to Android target.
echo.
exit /B 1


:: current direcoty
:GetFullPath
SET %2=%~f1
GOTO :EOF

:no_vs
echo.
echo ERROR: Visual Studio %* is not found in the system.
exit /B 1

:End
exit /B 0

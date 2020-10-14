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
set BIN_ROOT=%~dp0
set SCRIPT_NAME=%~nx0

:: Set the default arguments
set TARGET_VS=
set TARGET_ARCH=
set C_TARGET_ARCH=
set LP64_ILP64=
set QUIET=

:ParseArgs
:: Parse the incoming arguments
if /i "%1"==""        goto CheckArgs
if /i "%1"=="ia32"         (set TARGET_ARCH=ia32)         & (set C_TARGET_ARCH=ia32)    & shift & goto ParseArgs
if /i "%1"=="intel64"      (set TARGET_ARCH=intel64)      & (set C_TARGET_ARCH=intel64) & shift & goto ParseArgs
if /i "%1"=="vs2013"       (set TARGET_VS=vs2013)                                       & shift & goto ParseArgs
if /i "%1"=="vs2013shell"  (set TARGET_VS=vs2013)                                       & shift & goto ParseArgs
if /i "%1"=="vs2015"       (set TARGET_VS=vs2015)                                       & shift & goto ParseArgs
if /i "%1"=="vs2015shell"  (set TARGET_VS=vs2015)                                       & shift & goto ParseArgs
if /i "%1"=="vs2017"       (set TARGET_VS=vs2017)                                       & shift & goto ParseArgs
if /i "%1"=="vs2019"       (set TARGET_VS=vs2019)                                       & shift & goto ParseArgs
if /i "%1"=="lp64"         (set LP64_ILP64=lp64)                                        & shift & goto ParseArgs
if /i "%1"=="ilp64"        (set LP64_ILP64=ilp64)                                       & shift & goto ParseArgs
if /i "%1"=="quiet"        (set QUIET=quiet)                                            & shift & goto ParseArgs
if /i "%1"=="-arch"        shift & goto ParseArgs
if /i "%1"=="-platform"    shift & goto ParseArgsPlatform
goto Syntax

:ParseArgsPlatform
if /i "%1"==""        goto Syntax
if /i "%1"=="android"      (set TARGET_PLATFORM=android) & shift & goto ParseArgs
if /i "%1"=="linux"        (set TARGET_PLATFORM=linux)   & shift & goto ParseArgs
goto ParseArgs

:CheckArgs
:: target architecture is mandatory
if /i "%TARGET_ARCH%"=="" goto Syntax
:: set correct defaults
if /i "%TARGET_PLATFORM%"==""     (call :GetFileName "%BIN_ROOT%\.." ROOT_ROOT_NAME)
if /i "%ROOT_ROOT_NAME%"=="linux" (set TARGET_PLATFORM=linux)
goto Build

:Build
if exist "%BIN_ROOT%..\daal\bin\daalvars.bat" @call "%BIN_ROOT%..\daal\bin\daalvars.bat" %C_TARGET_ARCH% %TARGET_VS%
if exist "%BIN_ROOT%..\tbb\bin\tbbvars.bat" @call "%BIN_ROOT%..\tbb\bin\tbbvars.bat" %C_TARGET_ARCH% %TARGET_VS%
if exist "%BIN_ROOT%..\pstl\bin\pstlvars.bat" @call "%BIN_ROOT%..\pstl\bin\pstlvars.bat" %C_TARGET_ARCH% %TARGET_VS%
if exist "%BIN_ROOT%..\mkl\bin\mklvars.bat" @call "%BIN_ROOT%..\mkl\bin\mklvars.bat" %C_TARGET_ARCH% %TARGET_VS% %LP64_ILP64%
if exist "%BIN_ROOT%..\ipp\bin\ippvars.bat" @call "%BIN_ROOT%..\ipp\bin\ippvars.bat" %C_TARGET_ARCH% %TARGET_PLATFORM% %TARGET_VS%
if exist "%BIN_ROOT%..\ippcp\bin\ippcpvars.bat" @call "%BIN_ROOT%..\ippcp\bin\ippcpvars.bat" %C_TARGET_ARCH% %TARGET_PLATFORM% %TARGET_VS%
if exist "%FNL_DIR%\%C_TARGET_ARCH%\bin\fnlsetup.bat" @call "%FNL_DIR%\%C_TARGET_ARCH%\bin\fnlsetup.bat"
if exist "%BIN_ROOT%..\..\..\debugger_2020\bin\debuggervars.bat" @call "%BIN_ROOT%..\..\..\debugger_2020\bin\debuggervars.bat" %C_TARGET_ARCH%
if exist "%BIN_ROOT%..\mpi\%C_TARGET_ARCH%\bin\mpivars.bat" @call "%BIN_ROOT%..\mpi\%C_TARGET_ARCH%\bin\mpivars.bat" %QUIET%
if exist "%BIN_ROOT%compilervars_arch.bat" @call "%BIN_ROOT%compilervars_arch.bat" %TARGET_ARCH% %TARGET_VS% %TARGET_PLATFORM% %QUIET%

exit /B 0

:Syntax
echo Syntax:
echo  %SCRIPT_NAME% [-arch] ^<arch^> [vs] [-platform ^<platform^>]
echo.
echo    ^<arch^> must be is one of the following
echo        ia32         : Set up for IA-32 target
echo        intel64      : Set up for Intel(R) 64 target
echo    If specified, ^<vs^> must be one of the following
echo        vs2013      : Set to use Microsoft Visual Studio* 2013
echo        vs2013shell : Set to use Microsoft Visual Studio* Shell 2013
echo        vs2015      : Set to use Microsoft Visual Studio* 2015
echo        vs2015shell : Set to use Microsoft Visual Studio* Shell 2015
echo        vs2017      : Set to use Microsoft Visual Studio* 2017
echo        vs2019      : Set to use Microsoft Visual Studio* 2019
echo    If ^<vs^> is not specified, the version of Visual Studio* detected at install
echo    time is used.
echo    ^<platform^> must be of the following.
echo        linux          : Set to Linux* target.
echo        android        : Set to Android* target.
echo.
exit /B 1

:GetFileName
SET %2=%~n1
GOTO :EOF

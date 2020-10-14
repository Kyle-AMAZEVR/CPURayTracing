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

:: Set the default arguments
set TARGET_ARCH=

:ParseArgs
:: Parse the incoming arguments
if /i "%1"==""        goto Build
if /i "%1"=="ia32_intel64" (set TARGET_ARCH=intel64) & shift & goto ParseArgs
if /i "%1"=="intel64"      (set TARGET_ARCH=intel64) & shift & goto ParseArgs
shift & goto ParseArgs

:Build

if defined INSPECTOR_2019_DIR (
    if exist "%INSPECTOR_2019_DIR%\inspxe-vars.bat" @call "%INSPECTOR_2019_DIR%\inspxe-vars.bat" quiet
)
if defined VTUNE_AMPLIFIER_2019_DIR (
    if exist "%VTUNE_AMPLIFIER_2019_DIR%\amplxe-vars.bat" @call "%VTUNE_AMPLIFIER_2019_DIR%\amplxe-vars.bat" quiet
)
if defined ADVISOR_2019_DIR (
    if exist "%ADVISOR_2019_DIR%\advixe-vars.bat" @call "%ADVISOR_2019_DIR%\advixe-vars.bat" quiet
)

if /i "%TARGET_ARCH%"=="intel64" (
:: ITAC
    if exist "%BIN_ROOT%..\..\..\itac_2019\bin\itacvars.bat" (
        @call "%BIN_ROOT%..\..\..\itac_2019\bin\itacvars.bat"
    ) else (
        if exist "%BIN_ROOT%..\..\..\..\itac_2019\bin\itacvars.bat" @call "%BIN_ROOT%..\..\..\..\itac_2019\bin\itacvars.bat"
    )
)

if exist "%BIN_ROOT%compilervars.bat" @call "%BIN_ROOT%compilervars.bat" %*

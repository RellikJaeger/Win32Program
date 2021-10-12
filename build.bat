@echo off
:: IMPORTANT
set build_tool="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
set build_dir=build\
set source_file=main
set default_source=false
set param=%1
rem call :Stringlength paramLength %param%
set fileName=%param:~0,-4%
set fileExt=%param:~-4%

if [%param%] == [] (
	if exist "%source_file%.cpp" (
		set default_source=true
	) else (
		goto FileNotFoundError
	)
) else (
	set default_source=false
)

if not defined DevEnvDir (
	call %build_tool%
	if %errorlevel% neq 0 (
		echo.
		echo   Please check your Visual Studio C++ Workload Build Tools under this directory path:
		echo   %build_tool%
		echo.
		echo   If you can't find it, you need to install Visual Studio C++ Build Tools
		echo   and change the build_tool variable in build.bat file!
		echo.
	)
)
:: END IMPORTANT

call clean.bat %build_dir%
if not exist %build_dir% (md %build_dir%>nul)
pushd %build_dir%

:: IMPORTANT
set build_flags=/EHsc /WX /D UNICODE /D _UNICODE
:: END IMPORTANT

echo.
echo   +------------------------+
echo   ^|^> BUILDING THE PROGRAM ^<^|
echo   +------------------------+
echo.

:: IMPORTANT
if %default_source% == false (
	cl %build_flags% ..\%1
) else (
	cl %build_flags% ..\%source_file%.cpp
)
:: END IMPORTANT

if %errorlevel% neq 0 (goto Fail) else (goto Success)

:FileNotFoundError
if /i [%0] equ ["%~dpnx0"] (
msg * ERROR: Target source file not found. (default: main.cpp^)^

Please specify a target source file to build.^

Example: build hello.cpp
) else (
	echo.
	echo ERROR: Target source file not found. (default: main.cpp)
	echo        Please specify a target source file to build.
	echo        Example: build hello.cpp
)
exit /b 1

:Success
del /f /q *.obj
pushd ..
if %default_source% == false (
call run.bat %build_dir%\%fileName%.exe
) else (
call run.bat %build_dir%\%source_file%.exe
)
popd
popd
echo.
echo   +-------------+
echo   ^|^> SUCCEEDED ^<^|
echo   +-------------+
exit /b 0

:Fail
echo.
echo ERROR: Failed to build the program.
popd
echo.
echo   +----------+
echo   ^|^> FAILED ^<^|
echo   +----------+
exit /b 1

:Stringlength <resultVar> <stringVar>
(   
    setlocal EnableDelayedExpansion
    set "s=!%~2!#"
    set "len=0"
    for %%P in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
        if "!s:~%%P,1!" NEQ "" ( 
            set /a "len+=%%P"
            set "s=!s:~%%P!"
        )
    )
)
( 
    endlocal
    set "%~1=%len%"
    exit /b
)

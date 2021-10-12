@echo off
:: IMPORTANT
set build_tool="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
set output_dir=build
set source_file="..\main.cpp"
set output_program=main.exe

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

set build_dir=build
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
cl %build_flags% %source_file% %*
:: END IMPORTANT

if %errorlevel% neq 0 (goto Fail)

:Success
pushd ..
call run.bat %build_dir%\%output_program%
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

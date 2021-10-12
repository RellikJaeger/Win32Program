@echo off
set param=%1

for /f "delims=" %%i in ("%param%") do (
set fileDrive=%%~di
set filePath=%%~pi
set fileName=%%~ni
set fileExt=%%~xi
)

echo.
echo ---
echo ^>["%fileDrive%%filePath%%fileName%%fileExt%"]
echo  - Drive: "%fileDrive%"
echo  - Path: "%filePath%"
echo  - Filename: "%fileName%"
echo  - Extension: "%fileExt%"
echo ---

if [%param%] equ [] (
	if exist "build\main.exe" (
		start "" /b "build\main.exe"
		exit /b 0
	) else goto NeedToBuildError
) else (
	start "" /b %1>nul
	exit /b 0
)

:NeedToBuildError
if /i [%0] equ ["%~dpnx0"] (
	msg * Please build the program first.
) else (
	echo.
	echo   Please build the program first.
)
exit /b 1

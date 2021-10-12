@echo off
if [%1] equ [] (
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

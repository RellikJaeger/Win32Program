@echo off
if [%1] equ [] (
	if exist "build\" (
		rd /s /q "build\"
		exit /b 0
	) else goto NothingToClean
) else (
	if exist "%1" (
		rd /s /q "%1"
		exit /b 0
	) else goto NothingToClean
)

:NothingToClean
if /i [%0] equ ["%~dpnx0"] (
	msg * Nothing found to clean.
) else (
	echo.
	echo   Nothing found to clean.
)
exit /b 1

@echo off
if [%1] equ [] (
	if exist "build\" (
		rd /s /q "build\"
		exit /b 0
	) else goto BuildDirNotFoundError
) else (
	if exist [%1] (
		rd /s /q %1
		exit /b 0
	) else goto DirNotFoundError
)

:BuildDirNotFoundError
if /i [%0] equ ["%~dpnx0"] (
	msg * \"build\" directory not found to clean.
) else (
	echo.
	echo   "build" directory not found to clean.
)
exit /b 1

:DirNotFoundError
exit /b 1

@echo off
setlocal

:: Check if the correct number of arguments is passed
if "%~3"=="" (
    echo Usage: %0 [DEF file path] [Intermediate directory] [Output DLL path]
    exit /b 1
)

:: Assign arguments to variables for better readability
set "DefFilePath=%~1"
set "IntermediateDir=%~2"
set "OutputDllPath=%~3"

:: Ensure the intermediate directory exists
if not exist "%IntermediateDir%" mkdir "%IntermediateDir%"

:: Generate the .lib file from the .def file
lib /MACHINE:X64 /DEF:"%DefFilePath%" /OUT:"%IntermediateDir%\temp.lib"

:: Use the .exp file generated with the .lib file to create the DLL
link /MACHINE:X64 /DLL /NOENTRY /OUT:"%OutputDllPath%" "%IntermediateDir%\temp.exp"

echo DLL successfully created at %OutputDllPath%
endlocal

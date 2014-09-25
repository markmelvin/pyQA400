@echo OFF
REM Build script for the pyQA400 Cython wrapper

set PYTHONDIR=I:\Python33
set QAAPPDIR="C:\Program Files (x86)\QuantAsylum\QA400"

IF "%1"=="clean" (
    rmdir /S /Q build
    del /Q *.pyd
    del /Q *.cpp
    del /Q *.dll
    del /Q *.h
    del /Q *.lib
    del /Q %PYTHONDIR%\QAAnalyzer.exe
    del /Q %PYTHONDIR%\QAConnectionManager.dll
) ELSE (
    @echo Hint: You may need to run set_env.bat once to set up the environment for Visual Studio

    REM TODO - Make this go away
    @echo Copying dependencies...
    copy "..\cpp\Debug\QA400API.dll" .
    copy "..\cpp\QA400API\QA400API.h" .
    copy "..\cpp\Debug\QA400API.lib" .

    REM We need these to be alongside python.exe for the
    REM C# managed assembly resolution to work (unless we
    REM want to get into the mess of installing into the GAC
    REM TODO - Make this go away too
    copy %QAAPPDIR%\QAAnalyzer.exe %PYTHONDIR%
    copy ..\dependencies\QAConnectionManager.dll %PYTHONDIR%

    %PYTHONDIR%\python setup.py build_ext --inplace
)
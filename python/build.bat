@echo OFF
REM Build script for the pyQA400 Cython wrapper

set PYTHONDIR=I:\Programs\Python\Python35-32

IF "%1"=="clean" (
    rmdir /S /Q build
    del /Q *.pyd
    del /Q *.cpp
    del /Q *.dll
    del /Q *.h
    del /Q *.lib
) ELSE (
    @echo Hint: You may need to run set_env.bat once to set up the environment for Visual Studio

    REM TODO - Make this go away
    @echo Copying dependencies...
    copy "..\cpp\Debug\QA400API.dll" .
    copy "..\cpp\QA400API\QA400API.h" .
    copy "..\cpp\Debug\QA400API.lib" .

    %PYTHONDIR%\python setup.py build_ext --inplace
)
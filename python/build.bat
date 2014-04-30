@echo OFF
REM Build script for the pyQA Cython wrapper

IF "%1"=="clean" (
    rmdir /S /Q build
    del /Q *.pyd
    del /Q *.cpp
    del /Q *.dll
    del /Q *.lib
) ELSE (
    @echo Hint: You may need to run set_env.bat once to set up the environment for Visual Studio

    REM TODO - Make this go away
    @echo Copying dependencies...
    copy "..\cpp\Debug\QA400API.dll" .
    copy "..\cpp\Debug\QA400API.lib" .

    C:\python27\python setup.py build_ext --inplace
)
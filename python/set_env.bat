@ECHO OFF
REM Where my Visual Studio is installed
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"

REM So distutils will work. The environment variable set here is
REM the "fallback" version and matches whatever version was used to
REM build Python. For Python 3.3, this is VS100COMNTOOLS
REM set VS100COMNTOOLS=%VS120COMNTOOLS%

REM This is for Python 2.7
set VS90COMNTOOLS=%VS120COMNTOOLS%

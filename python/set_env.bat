@ECHO OFF
REM Where my Visual Studio is installed
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

REM --------------------------------------------------------------------------
REM This no longer seems to be required on Windows 10 with Python 3.5
REM --------------------------------------------------------------------------
REM So distutils will work. The environment variable set here is
REM the "fallback" version and matches whatever version was used to
REM build Python. For Python 3.5, this is VS100COMNTOOLS
REM set VS100COMNTOOLS=%VS140COMNTOOLS%
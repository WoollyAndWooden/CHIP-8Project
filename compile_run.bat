@ECHO OFF
mingw32-make > result.txt
FINDSTR /m "failed" result.txt
IF %ERRORLEVEL%==1 (
    START ./bin/main.exe
)
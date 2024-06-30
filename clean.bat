echo clean directories:
del c:\obj\xlib\*.o
del c:\obj\xlib\widget\*.o
del c:\obj\xapps\*.o
del c:\obj\dms\*.o
del c:\obj\dms\png\*.o
del c:\obj\dms\jpg\*.o
del c:\obj\dms\jpg\*.o
del c:\obj\dms\mp3\*.o
del c:\obj\dms\mp3\decoder\*.o
del c:\obj\vfile\*.o
del c:\bin\xapps\*.app
del c:\bin\xlib\*.dl

echo Aura M6 builder august 2023 build :4000 
if exist c:\dj.bat goto cpath
if exist c:\auragui\dj.bat goto full
goto error

:full
echo DJGPP was detected in the ROOT of C:\AURAGUI

set path=c:\AURAGUI\DJGPP\BIN;%path%;c:\AURAGUI\djgpp\watt32\src;c:\AURAGUI\djgpp\watt32\bin;c:\AURAGUI\djgpp\watt32\util;c:\AURAGUI\djgpp\watt32\lib;c:\AURAGUI\djgpp\watt32\inc;c:\include\pdcurses\
set DJGPP=c:\AURAGUI\DJGPP\DJGPP.env
set WATT_ROOT=c:\AURAGUI\DJGPP\watt32
set PDCURSES_SRCDIR=c:\include\pdcurses\
make
goto end

:cpath
echo DJGPP was detected in the ROOT of C:\
set path=c:\DJGPP\BIN;%path%;c:\djgpp\watt32\src;c:\djgpp\watt32\bin;c:\djgpp\watt32\util;c:\djgpp\watt32\lib;c:\djgpp\watt32\inc;c:\include\pdcurses\
set DJGPP=c:\DJGPP\DJGPP.env
set WATT_ROOT=c:\DJGPP\watt32
set PDCURSES_SRCDIR=c:\include\pdcurses\
make > 
goto end

:error
if exist c:\bin\main.exe goto end
if exist c:\auragui\bin\main.exe goto end
echo well youre fucked lol.
pause

:end
if exist c:\bin\main.exe echo Aura has been Built. 
if exist c:\auragui\bin\main.exe echo Aura has been Built. 

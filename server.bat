@echo off
echo DJGPP was detected in the ROOT of C:\
set path=c:\DJGPP\BIN;c:\djgpp\watt32\src;c:\djgpp\watt32\bin;c:\djgpp\watt32\util;c:\djgpp\watt32\lib;c:\djgpp\watt32\inc;c:\include\pdcurses\;c:\
set DJGPP=c:\DJGPP\DJGPP.env
set WATT_ROOT=c:\DJGPP\watt32
set PDCURSES_SRCDIR=c:\include\pdcurses\
echo building dos watt32 server
gcc dserver.c -o dserver.exe -IC:\DJGPP\watt32\inc -LC:\DJGPP\watt32\lib -lwatt

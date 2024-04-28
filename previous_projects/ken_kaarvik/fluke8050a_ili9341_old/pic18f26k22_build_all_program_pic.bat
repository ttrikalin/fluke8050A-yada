@echo off
echo PIC18 ....

@echo.
echo Compiling PIC Program (Hi-Tech)................................for PIC18F46K22
@echo.

xc8 --CHIP=18F46K22 fluke8050a_ili9341_pic18f46k22.c

if errorlevel	1 pause

@echo.
echo Programming PIC................................................for PIC18F46K22
@echo.

pk2cmd -pPIC18F46K22 -ffluke8050a_ili9341_pic18f46k22.hex -M -J -R

if errorlevel	1 pause
pause
:end
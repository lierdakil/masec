@echo off
SET called=%0
SET in=%1
SET out=%in:.red=.txt%
SET homedir=%called:parse.bat=%
cd %homedir%
asec_parser.exe %in%
asec_parser.exe %in% > %out%
echo Saved to %out%
pause
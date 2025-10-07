@echo off
cd /d "%~dp0build\Release"
echo Starting CORE game...
CORE.exe > output.txt 2>&1
type output.txt
pause


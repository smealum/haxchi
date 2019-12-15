@echo off

cd installer
mkdir data 2>/nul
cd ../dsrom
echo.
echo Building DS ROMs
echo.
make clean
make Windows_NT=1 CB=1

cd ../installer
echo.
echo Building WiiU Installer
echo.
make clean
make CB=1

echo.
pause

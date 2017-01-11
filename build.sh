#!/bin/sh

cd installer
mkdir data
cd ../dsrom
echo Building DS ROMs
make clean
make

cd ../installer
echo Building WiiU Installer
make clean
make


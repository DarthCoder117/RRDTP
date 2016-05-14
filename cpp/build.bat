REM Make sure to run this from the Visual Studio developer command line.
mkdir projects
cd projects
cmake ../ -G "NMake Makefiles"
nmake
cd ../

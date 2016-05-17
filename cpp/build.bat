mkdir projects\x86
cd projects\x86
cmake ../../ -G "Visual Studio 14"
cmake --build . --config Release
cd ..\..\

mkdir projects\x64
cd projects\x64
cmake ../../ -G "Visual Studio 14 2015 Win64"
cmake --build . --config Release
cd ..\..\
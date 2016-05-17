cd ..\cpp
call build.bat
cd ..\java
rmdir /s /q build
mkdir build
mkdir build\win32-x86
mkdir build\win32-x86_64
copy "..\cpp\lib\MSVC\x86\Release\rrdtp_module.dll" "build\win32-x86\rrdtp_module.dll"
copy "..\cpp\lib\MSVC\x64\Release\rrdtp_module.dll" "build\win32-x86_64\rrdtp_module.dll"
javac -cp "3rd-party\*;." -d "build" src\edu\cwu\rrdtp\*.java
cd build
jar xf ../3rd-party/jna-4.2.2.jar
rmdir /s /q META-INF
jar cvf rrdtp.jar ./*
cd ..\..\
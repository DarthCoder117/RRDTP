REM ../cpp/build.bat
rmdir /s /q build
mkdir build
mkdir build\win32-x86
copy "..\cpp\lib\Release\rrdtp_module.dll" "build\win32-x86\rrdtp_module.dll"
javac -cp "3rd-party\*;." -d "build" src\edu\cwu\rrdtp\*.java
cd build
jar xf ../3rd-party/jna-4.2.2.jar
rmdir /s /q META-INF
jar cvf rrdtp.jar ./*
cd ../
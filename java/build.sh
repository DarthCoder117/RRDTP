cd ../cpp
sh ../cpp/build.sh
cd ../java
rm -rf ./build
mkdir ./build
mkdir ./build/linux
cp ../cpp/lib/GNU/librrdtp_module.so ./build/linux/rrdtp_module.so
javac -cp ./3rd-party/*:. -d ./build ./src/edu/cwu/rrdtp/*.java
cd build
jar xf ../3rd-party/jna-4.2.2.jar
rm -rf ./META-INF
jar cvf rrdtp.jar ./*
cd ../../

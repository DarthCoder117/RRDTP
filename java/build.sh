mkdir ./build
javac -cp ./3rd-party/jna-4.2.2.jar:. -d ./build ./src/edu/cwu/rrdtp/*.java
jar cvf ./build/rrdtp.jar ./3rd-party/jna-4.2.2.jar ./build/edu/cwu/rrdtp/*.class

# RRDTP (Realtime Robotics Data Transmission Protocol)
RRDTP is a realtime data transmission protocol intended for use in monitoring sensor data in robotics applications. This library can be compiled for Windows, Linux, and Mac OSX, systems. Arduino support is planned as well.
## Building
Use CMake to build the library and tests. Run ``cmake -G`` and specify the platform to generate project files for.
``cmake -G Visual Studio 14`` for Visual Studio 2015, ``cmake -G Unix Makefiles`` for Linux, etc. 
After the project files are generated, then just build normally in your preferred environment.

# RRDTP
RRDTP (Realtime Robotics Data Transmission Protocol) is a realtime data transmission protocol intended for use in monitoring sensor data in robotics applications. The RRDTP protocol is designed to be very minimal to facilitate easy implementation on resource-limited systems.  

RRDTP and the associated Realtime Robotics Dashboard project, were developed at Central Washington University by the RRDTP team.
* Tanner Mickelson - lead developer/library implementation (mickelsont@cwu.edu)
* David Rigdby - netcode/library implementation
* Michael Githinji - library implementation/dashboard
* Brennan Radich - dashboard UI/library integration
* Tyler Swan - dashboard UI/library integration

## Building
RRDTP has been tested on Windows, Linux, and Mac OSX. Arduino support is planned but not yet implemented.
Use CMake to build the library and tests. Run ``cmake -G`` and specify the platform to generate project files for.
``cmake -G Visual Studio 14`` for Visual Studio 2015, ``cmake -G Unix Makefiles`` for Linux, etc. 
After the project files are generated, then just build normally in your preferred environment.
## License
The MIT License (MIT)

Copyright (c) 2016 Tanner Mickelson & The RRDTP Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

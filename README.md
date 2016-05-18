# RRDTP
RRDTP (Realtime Robotics Data Transmission Protocol) is a realtime data transmission protocol intended for use in monitoring sensor data in robotics applications. The RRDTP protocol is designed to be very minimal to facilitate easy implementation on resource-limited systems.  

RRDTP and the associated Realtime Robotics Dashboard project, were developed at Central Washington University by the RRDTP team.
* Tanner Mickelson - lead developer/library implementation (mickelsont@cwu.edu)
* David Rigdby - netcode/library implementation
* Michael Githinji - library implementation/dashboard
* Brennan Radich - dashboard UI/library integration
* Tyler Swan - dashboard UI/library integration

## Building
RRDTP can be built easily using CMake on Windows and Linux. Mac OSX will be supported as well, but has not yet been thoroughly tested.
Automatic build scripts are included for Windows and Linux. Simply run build.bat or build.sh to build the library on your platform.

## Roadmap
* Arduino support
* Multithreading for supported platforms
* Finish incomplete Java wrapper (almost complete)
* Performance optimizations
* Mac OSX support (almost complete)

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

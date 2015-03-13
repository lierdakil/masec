### Description ###
This is a modular system designed for conducting physical experiments.

It consists of three logical parts: control module, a set of a number of "hardware" modules, which can talk to hardware in any way they need and return results to control module, and supplementary modules, like data file parser, or on-line plotting module.

### Dependencies ###
Application is made using Qt 4 framework, open-source edition. For more information on Qt, see http://www.qtsoftware.org/

For communication between modules, DBus is used. For more information on DBus, see http://www.freedesktop.org/wiki/Software/dbus

To draw plots, an excellent Qwt widgets are used. For more information on Qwt, see http://qwt.sourceforge.net/

### Cross-platform ###
Base platform for application development is (unfortunately) Windows XP. This means that, although application is indeed potentially usable on Linux, it was not thoroughly tested, and there are some caveats.

Control module works on both Linux and Windows with no modification whatsoever.

As of yet, "hardware" modules require VISA libraries, which means they require either windows, or TAMS I/O Library (which costs almost 500 euro). I have not found any kind of working VISA implementation for UNIX-like systems. There was that OpenVISA project, but, AFAIK, it stopped on a very early stage.

However, it is possible to modify low-level I/O module to work with virtually any string I/O driver, such as Linux GPIB project driver. Specifically, it's required to reimplement two functions in asec\_gpib/gpibctrl.cpp, namely GPIBctrl::write and GPIBctrl::readArray, and parts of constructor and destructor of GPIBctrl class (which open and close device I/O).

To compile under Windows, you will need Qt 4 with dbus support. It means that you will have to compile and install WinDBus, which is Windows port of DBus, found here: http://sourceforge.net/projects/windbus. Moreover, you will have to rebuild Qt with DBus support enabled.

I strongly recommend to build WinDBus from source, using SVN stable version.

Also, all builds are tested with MinGW GCC compiler. I did test some versions with VC++ 2008 nmake, and they worked, but it is way too troublesome to test every revision, so I do not guarantee that it will build.

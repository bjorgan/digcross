Building
--------

This project uses CMake.

First-time build:

1. mkdir build
2. cd build
3. cmake ..
4. make

Alternatively: Use qmake directly.

1. mkdir build
2. cd build
3. qmake ..
4. make

Requirements: Qt5 (qt5-default in Debian), CMake.

Daemon
------

Transactions are sent using DBus' system bus to an external daemon (digcrossd).
The daemon is installed using

make install

This also takes care of installing the necessary configuration files to
DBus' system-wide configuration directories. The daemon is run
automatically as a DBus service whenever requested from digcross.

Building
--------

This project uses CMake.

First-time build:

```
1. mkdir build
2. cd build
3. cmake ..
4. make
```

Alternatively: Use qmake directly.

```
1. mkdir build
2. cd build
3. qmake ..
4. make
```

Requirements: Qt5 (qt5-default in Debian), CMake.

Daemon
------

By design, transactions will be sent to an external daemon. The daemon and its
necessary (DBus) configuration files are installed using

```
make install
```

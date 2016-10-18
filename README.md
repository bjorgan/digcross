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

To build with tests, use:

```
3. cmake -DWITH_TESTING=True ..
4. make
```

Tests can be run using `make test`.

Requirements: Qt5 (qt5-default in Debian), CMake.

Daemon
------

By design, transactions will be sent to an external daemon. The daemon and its
necessary (DBus) configuration files are installed using

```
make install
```

(Since DBus looks for configuration files in a few (system-wide) paths, the above command
has to be run with root privileges regardless of install prefix.)

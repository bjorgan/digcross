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

Virtual keyboard
----------------

Qt's virtual keyboard can be used as a screen keyboard. Instructions:

1. For Qt version 5.6: git clone http://code.qt.io/cgit/qt/qtvirtualkeyboard.git/, git checkout -b 5.6 origin/5.6, mkdir build, cd build, qmake .., make, make install.
2. For Qt version >= 5.7: It is probably included. Install the necessary module through the package manager.
3. Install: qml-module-qtquick2, qml-module-qtquick-layouts, qml-module-qt-labs-folderlistmodel (if anything else is needed, Qt will probably display QML warnings when an input context is needed).

The keyboard will automatically be launched when a keyboard context is required.

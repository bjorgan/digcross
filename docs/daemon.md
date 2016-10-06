Transaction daemon
==================

In order to:

* Restrict read access to user database
* Restrict access to backend authentication key

, the direct backend access and userdatabase access is abstracted away into a separate application/daemon. The daemon is called `digcrossd`. All source code related to the daemon is organized under `daemon/`, while the daemon client is located under the GUI source folder (`daemon_client.cpp/.h`).

For simplification, IPC (inter-process communication) is done using DBus.
The GUI sends a transaction using `DaemonClient::processTransaction()`, which
takes care of connecting to the DBus service provided by `digcrossd`, sending the appropriate calls and connecting the eventual reply to the `DaemonClient::transactionFinished()` signal.

On the daemon's end, transactions are received using `Daemon::processTransaction()`. It is assumed that all database logic and backend API calls will be done here before sending the transaction reply back to the client.

Configuration files
-------------------

It is assumed that `digcrossd` will eventually be run by a separate user from the user running the GUI application. The DBus calls are therefore sent through the system bus, as the session bus is per-user. This neccessitates special DBus policies for the user running `digcrossd`, which has been specified in `daemon-config/org.digcross.daemonconnection.conf.in`.

In addition, for convenience, `digcrossd` is activated as a DBus service when needed by the GUI application. This means that `digcrossd` won't have to be started in advance or during boot, but will start when needed, automagically. This is specified in `daemon-config/org.digcross.daemonconnection.service.in`.

Both configuration files contain references to the user running the `digcrossd` application. The username is specified in the variable `DIGCROSSD_USER` in `CMakeLists.txt`, currently set to be the user compiling the application.

Both configuration files need to be installed to system-wide DBus configuration directories. This is specified by install rules in `CMakeLists.txt`.

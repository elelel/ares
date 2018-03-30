# Ares server build and installation instructions on Linux

### Installing prerequisites

This procedure assumes that you already downloaded Ares sources (e.g. into ~/sources/ares)

The following build prerequisites should be installed:

- [CMake](https://cmake.org) (tested version 3.10) 
- C++ compiler. [GCC](https://gcc.gnu.org) (tested version 7.3.1). Clang's support for C++17 is not good enough (tested version 6.0.0).
- [Zlib](https://www.zlib.net)
- [libpq](https://www.postgresql.org/docs/current/static/libpq.html) C library to interface with [PostgreSQL](https://www.postgresql.org/)
- [libpqxx](https://www.github.com/jtv/libpqxx) C++ library to interface with [PostgreSQL](https://www.postgresql.org/). Version 5 and above is ok. Version 4 branch is ok if all bugfixis are applied which is most likely is not the case if your Linux distro still has version 4 in packages.
- (Optionally) [Boost libraries](https://www.boost.org) version 1.66.0 and above. These are needed if your compiler's support for C++17 is
incomplete, in this case Ares will try to fallback to Boost libraries. Currently builds with GCC need them.

Usually these dependencies can be installed with your distro's package manager. If your distro doesn't have a packaged dependency or if
it's too old you may need to install dependencies from alternative packages or from sources.

#### Notes on installing build prerequisites manually

##### libpqxx

If you need to install libpqxx manually (e.g. your system's version is too old), you need to clone https://github.com/jtv/libpqxx.git
and follow it's instructions for the installation. It is recommended that you don't install it systemwide (it is likely that your OS
has other outdated packages that depend on old libpqxx) but use a unique install prefix. In this case you will have to add two
options when configuring your Ares build with CMake using -D command line option:
- ARES_LIBPQXX_LIB - libpqxx library path/filename
- ARES_LIBPQXX_INCLUDE_DIRS - location of libpqxx include files (from the libpqxx source directory you've cloned)

### Configuring and building

Create a directory, e.g. ~/build/ares with three subdirectories: account, character and zone for each server.

Use the following required configuration parameters to CMake:
- ARES_PACKET_SET - packet set version to build the server for

Optionally set these definitions:
- ARES_LIBPQ_LIB - libpq library
- ARES_LIBPQ_INCLUDE_DIRS - location of libpq include files
- ARES_LIBPQXX_LIB - libpqxx library (required if not using system's package)
- ARES_LIBPQXX_INCLUDE_DIRS - location of libpqxx include files (required if not using system's package)
- ARES_ZLIB_LIB - zlib library
- ARES_ZLIB_INCLUDE_DIRS - location of zlib include files (which is the directory unpacked zlib sources plus zlib build dir, where a header file is generated)

Example of configuring and building account server:
```
cd ~/build/ares/account
cmake -DARES_PACKET_SET=pets_5a4c7c5c ~/sources/ares/account
make
```

This procedure has to be performed for each server type (account, character and zone).

Now proceed to [configure](configuring.md) your Ares installation.
# Ares server build and installation instructions on Windows

## Building with Microsoft compiler and tools

### Installing simple build prerequisites

This procedure assumes that you already have Microsoft Visual Studio 2017 (or Microsoft
Build Tools 2017) installed and Ares sources downloaded (e.g. into c:\sources\ares)

- Download and install PostgreSQL server from https://www.postgresql.org/download/windows/
Even if you don't plan to run the PostgreSQL server on the same machine, the libraries
included with the it are needed to compile Ares server. An alternative source of these
libraries can be pgAdmin tool which can also be downloaded from the official site.

### Installing not so simple build prerequisites

#### libpqxx
Clone https://github.com/jtv/libpqxx somewhere, e.g to c:\libs_for_ares\:

```
git clone https://github.com/jtv/libpqxx
```

Instructions for compiling libpqxx can be found in win32/INSTALL.txt file but may
be outdated.
Here's a quick overview of the process.

- Copy file win32/common-sample to win32/common. Open the file in a text editor.
Change the variables to point to different paths in your PostgreSQL installation (e.g. in c:\Program Files\PostgreSQL\10).
You will end up changing some of the variables to something like this (assuming you used the EDB PostgreSQL package for Windows):

```
PGSQLSRC="C:\Program Files\PostgreSQL\10\"
LIBPQINC=$(PGSQLSRC)\include
LIBPQPATH=$(PGSQLSRC)\lib
LIBPQLIB=libpq.lib
LIBPQDPATH=$(PGSQLSRC)\lib
LIBPQDDLL=libpq.dll
LIBPQDLIB=libpq.lib
```

- Copy Visual Studio compiler-specific sample headers from c:\libs_for_ares\libpqxx\config\sample-headers\compiler\VisualStudio2013\pqxx to c:\libs_for_ares\libpqxx\include\pqxx

- Launch x64 Native tools Command Prompt for VS 2017 (or any dev prompt with VC vars suitable for you)

```
cd c:\libs_for_ares\libpqxx
nmake /f win32\vc-libpqxx.mak all
```

libpqxx and libpq libraries will now be compiled/copied into c:\libs_for_ares\libpqxx\lib

#### ZLib
- Download ZLib sources from https://www.zlib.net and unpack them, e.g. to c:\libs_for_ares\zlib

- Create build directory, e.g. c:\build\zlib

- Launch x64 Native tools Command Prompt for VS 2017 (or any dev prompt with VC vars suitable for you)

```
cd c:\build\zlib
"c:\program files\cmake\bin\cmake.exe" -G "NMake Makefiles" c:\libs_for_ares\zlib
nmake
```

### Configuring and building

Create a directory, e.g. c:\build\ares with three subdirectories: account, character and zone for each server.
Use the following required configuration parameters to CMake:
- ARES_PACKET_SET - packet set version to build the server for
- ARES_LIBPQ_LIB - libpq library (from the dir you've built libpqxx in)
- ARES_LIBPQ_INCLUDE_DIRS - location of libpq include files (from PostgreSQL install dir)
- ARES_LIBPQXX_LIB - libpqxx library (the one you've built)
- ARES_LIBPQXX_INCLUDE_DIRS - location of libpqxx include files (from the libpqxx source directory you've cloned)
- ARES_ZLIB_LIB - zlib library (the one you've built)
- ARES_ZLIB_INCLUDE_DIRS - location of zlib include files (which is the directory unpacked zlib sources plus zlib build dir)

Optionally set these definitions
- _WIN32_WINNT - your target Windows version, defaults to 0x0501 for XP, efficiency of Asio library depends on this

Note that on Windows you should match Release/Debug versions of the libraries when linking.

Example of configuring and building account server:

```
cd c:\build\ares\account
"c:\program files\cmake\bin\cmake.exe" -G "NMake Makefiles" -DARES_PACKET_SET=pets_5a4c7c5c -DARES_LIBPQ_LIB="C:\libs_for_ares\libpqxx\lib\libpq.lib" -DARES_LIBPQ_INCLUDE_DIRS="C:\Program Files\PostgreSQL\10\include" -DARES_LIBPQXX_LIB="c:\libs_for_ares\libpqxx\lib\libpqxx_staticD.lib" -DARES_LIBPQXX_INCLUDE_DIRS="c:\libs_for_ares\libpqxx\include" -DARES_ZLIB_LIB="c:\build\zlib\zlibd.lib" -DARES_ZLIB_INCLUDE_DIRS="c:\libs_for_ares\zlib;c:\build\zlib" c:\sources\ares\account
nmake
```

This procedure has to be performed for each server type (account, character and zone).

Now proceed to [configure](configuring.md) your Ares installation.
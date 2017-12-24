# Ares server build & install instructions

## Runtime requirements

The following software is required to run Ares server:

- PostgresSQL server to store game data

## Build requirements

- C++ compiler supporting C++17. Currently only GCC is supported
- CMake build system
- Boost
- libpqxx library to communicate with PostgreSQL (recent enough to compile under C++17,
version 5.1 is known to work)
- libpq library to communicate with PostgreSQL (required by libpqxx)

Current CMakeFiles are not tuned for Windows builds. Building on
Windows is probably possible through a Linux environment like MSYS2.
Windows 10 native Linux subsystem won't probably do as it is
hopelessly old.

## Build example on Linux
1. Install the libraries listed in 'Build requirements' section
using your distribution's packet management system or by other means
2. Clone the repository including all submodules
```
git clone --recurse-submodules https://github.com/elelel/ares
```
3. Create build directories somewhere at a location separate from
the cloned source. Currently each server has to be compiled separately
(account, character, zone).

```
mkdir -p ~/build/ares/account ~/build/ares/character ~/build/ares/zoneto
```

4. Compile account server
```
cd build/ares/account
```
Configure the project using CMake with 'base' (or any other) packet version
```
cmake -DARES_PACKET_VER=base /path/ares/account
```
...where ```/path/ares``` is the location of the cloned git repo
Make the server
```
make
```
This will produce ares_account executable binary

5. Repeat the steps in 4 for character server, the sources are located
at path /path/ares/character

6. Repeat the steps in 4 for zone server, the sources are located
at path /path/ares/zone

7. Setup game database
Create three databases: one for account server, one for character and one for zone.
Create extention pgcrypto in Postgres (refer to Postgres doc if needed) for account server.
Use schema files in projects/postgres to populate the database
Create manually a test user (refer to account server's database directory in
sources for the SQL syntax) in account server database

## Runtime configuration

Configuration files location is either in /etc/ares directory or ${HOME}/etc/ares
with latter taking precedence if both exist.
- Account server configuration is stored in account_server.json file,
- Character server - character_server.json
- Zone server - zone_server.json

#### Example configuration for account_server.json:
```js

{
    "log" : {
        "target" : "console"
    },
    "client_version": 43,
    "listen_ipv4": [
        { "host": "127.0.0.1", "port": 6900 },
        { "host": "192.168.1.1", "port": 6900 }
    ],
    "database": {
        "dbname": "ares_account",
        "host": "127.0.0.1",
        "port": 5432,
        "user": "ares",
        "password": "password"
    },
    "char_servers": [
        { "login": "eacommlog", "password": "ea_pass"}
    ]
}
```

#### Example configuration for account_server.json:
```js
{
    "server_name": "Ares server",
    "log": {
        "target" : "console"
    },
    "listen_ipv4": [
        { "host": "192.168.1.1", "port": 6121 },
        { "host": "127.0.0.1", "port": 6121 }
    ],
    "database": {
        "dbname": "ares_character",
        "host": "127.0.0.1",
        "port": 5432,
        "user": "ares",
        "password": "password"
    },
    "account_server": {
        "name": "Ares server", 
        "login": "eacommlog",
        "password": "ea_pass",
        "connect": { "host": "192.168.1.19",
                     "port": 6900 }
    },
    "zone_servers": [
        { "login": "eacommlog",
          "password": "ea_pass" }
    ]
}

```

#### Example configuration for zone_server.json
```js
{
    "log": {
        "target" : "console"
    },
    "listen_ipv4": [
        { "host": "192.168.1.1", "port": 5121 },
        { "host": "127.0.0.1", "port": 5121 }
    ],
    "database": {
        "dbname": "ares_character",
        "host": "127.0.0.1",
        "port": 5432,
        "user": "ares",
        "password": "password"
    },
    "character_server": {
        "name": "Ares server", 
        "login": "eacommlog",
        "password": "ea_pass",
        "connect": { "host": "192.168.1.1",
                     "port": 6121 }
    }
}

```

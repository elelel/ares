# Configuring Ares server installation

## PostgreSQL configuration

Make sure that your max_connections setting in Postgres conf is at least = number of CPU cores on your each of your server * 3.
The default is 100, which should be enough in most cases, unless you're sharing this PostgreSQL instance with
other applications.

Currently all sql data is stored in a single database. That is likely to be changed in future.
For now, create a database 'ares' with user 'ares' as owner.
After doing that, login to psql as PostgreSQL admin user and create crypto extension (used for
game login authentication). Assuming that your PostgreSQL system user is postgres, it can be
done, for example, like this:

```
su postgres
psql ares
CREATE EXTENSION pgcrypto;
```

Use [database_schema.sql](../projects/postgres/database_schema.sql) file from projects/postres directory to create new database structure.
Be warned, this file drops existing data from SQL.

## Runtime configuration

Each server (account/character/zone) has it's own configuration file in JSON format.
The configuration files are first search for in ${HOME}/etc/ares directory, then in /etc/ares.

### Account server JSON configuration (account_server.json)

#### Example


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

### Character server JSON configuration (character_server.json)

Some configuration settings explained

- ''grfs'' - an array with path to client GRF files which are used to populate map data.
   These files are required at least for the first time you run Ares character server with
   the new maps, after that the data will be cached in SQL database.
 - ''zone_servers'' - an array of zone servers that this character server manages. Note
   that unlike Athena, individual maps are assigned to zone servers by character server
   to maintain consistency

#### Example

```js
{
    "server_name": "Ares server",
    "log": {
        "target" : "console"
    },
    "listen_ipv4": [
        { "host": "192.168.1.1", "port": 16121 },
        { "host": "127.0.0.1", "port": 16121 }
    ],
    "database": {
        "dbname": "ares",
        "host": "127.0.0.1",
        "port": 5432,
        "user": "ares",
        "password": "password"
    },
    "grfs": [
        "~/ro/client/data.grf",
        "~/ro/client/rdata.grf"
    ],
    "account_server": {
        "name": "Ares server", 
        "login": "eacommlog",
        "password": "ea_pass",
        "connect": { "host": "192.168.1.19",
                     "port": 6900 }
    },
    "zone_servers": [
        { "login": "zone1",
          "password": "ea_pass",
          "maps": [
              "prontera",
              "prt_are_in",
              "prt_are01",
              "pvp_room",
              "prt_castle",
              "prt_church",
              "prt_fild00",
              "prt_fild01",
              "prt_fild02",
              "prt_fild03",
              "prt_fild04",
              "prt_fild05",
              "prt_fild06",
              "prt_fild07",
              "prt_fild08",
              "prt_fild09",
              "prt_fild10",
              "prt_fild11",
              "prt_in",
              "prt_maze01",
              "prt_maze02",
              "prt_maze03",
              "prt_monk",
              "prt_sewb1",
              "prt_sewb2",
              "prt_sewb3",
              "prt_sewb4"
          ]
        }
    ]
}
```

### Zone server JSON configuration (zone_server.json)

Some configuration settings explained:
 - obfuscation_key is the key used for original packet header obfuscation. This
will likely to be moved into packet_set configuration later
 
#### Example

```js
{
    "log": {
        "target" : "console"
    },
    "listen_ipv4": [
        { "host": "192.168.1.1", "port": 15121 },
        { "host": "127.0.0.1", "port": 15121 }
    ],
    "database": {
        "dbname": "ares",
        "host": "127.0.0.1",
        "port": 5432,
        "user": "ares",
        "password": "password"
    },
    "character_server": {
        "login": "zone1",
        "password": "ea_pass",
        "connect": { "host": "192.168.1.19",
                     "port": 16121 }
    },
    "obfuscation_key": [ "0x348F4BD7", "0x7A425A54", "0x628F589A" ]
}
```

# Configuring Ares server installation

TODO Rewrite

# These are old/outdated instructions!

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

### Rewrite Athena server to modern and sane C++.

Things done and working:
- Network and event server template (buffers, reading, sending, managing connections, timers, etc)
- Elaborate and efficient packet parsing/sending system
- Database (talking to Postgres database, hiding complexities from the caller)
- Config (parsing and checking JSON config files)
- Account, Character, Zone servers - talking to each other, to the client up to the point of client login to Zone

Things not done:
- The "vertical" work (adding more cases to the things listed above)
- Game logic itself

Build and installation docs are [here](doc/install.md).

Interested in development/design details? See [developer's guide](doc/developer.md) (which is slightly out of date).

Questions? Issue an issue!

License: GPL v3

# Ares server runtime architecture and deployment

Ares server architecture mimics that of Athena servers family. At runtime it consists
of three processes: account server, character server and zone server.
The game data is stored in a PostgreSQL database, configuration
files are stored in JSON format for easy future integration with web services, etc.
The packet configuration is statically compiled (following Athena's path)

# Ares server development architecture
Ares codebase is C++17 under CMake project management potentially targeted
to multiple deployment platforms. The project is based around asynchronous
architecture using ASIO library.

## Build dependencies
- CMake (Should be 3.8.2 or above for future C++17 config flags comaptibility)
- network_buffer (included as git submodule)
- spdlog (included as git submodule)
- Catch (for tests, included as git submodule)
- Nlohmann JSON (for configuration files, included as git submodule)
- Asio (included as git submodule)
- libpqxx (to communicate with Postgres). Tested with version 5.1. The 4.0 branch
will probably work, but only if it is recent enough (source code newer than at least
Mar 24, 2012 (?)) to have C++17 deprecation fixes.

### Structure
#### Common subsystem
Located under 'common' folder and contains standalone utilities, like
random engine, string conversion utilities, etc.

#### Network subsystem
Network subsystem defines generic server interface. It is a header-only
template library located in 'network' subdirectory. Main parts:
- server skeleton and it's TCP-related dependencies (used to derive concrete
  account, character and zone servers)
- handler skeletons (to handle network and timer events)
- session skeleton (to describe connected sessions that can be
specialized as game client connections or connections of other server types)
- various network utilities, like packet size templates to determine
the packet size by it's structure

#### Runtime configuration subsystem
A static library located under 'config' directory which reads and parses JSON
configuration files, depends on Nlohman JSON

#### Database subsystem
General library for interaction with PostgreSQL database. Used as a base
to derive specific server's database. Located in 'database' directory.
Concrete specializations are located in 'src/database' directories of
each respective server. These specializations contain high-level database
queries, usually one per file, that perform an action in a db
transaction. For example, a character server persistent db query to perform
an item trade could be something like
this:
```c++
namespace ares {
  namespace character {
    namespace db {
      struct trade_items : pqxx::transactor<> {
        trade_items(const uint32_t cid1, const ...& cid1_items,
                    const uint32_t cid2, const ...& cid2_items) {
          // Save the references to the data in the object                    
        }

        void operator(argument_type& trans) {
          // Prepare UPDATE statement to update char 1
          trans.conn().prepare("cid1", "UPDATE table_name ...");
          trans.prepared("cid1")(cid1_items_)...;
          // Prepare UPDATE statement to update char 2
          trans.conn().prepare("cid2", "UPDATE table_name ...");
          trans.prepared("cid2")(cid2_items)...;
          // ... set rslt_ to true in case of success
        }
      private:
        // Data references in the object
        ... cid1_items_;
        ... cid2_items_;
        bool& rslt_;
      };
    }
  }
}

bool ares::character::database::trade_items(cid1, ..., cid2 ...) {
  bool rslt{false};
  with_wait_lock([this, &cid1, ..., &cid2, ...,  &rslt] () {
      db::trade_items t(rslt, cid1, ..., cid2, ...);
      pqxx_conn_->perform(t);
    });
  return rslt;
}

```

Under such architecture all interactions with the database will be forced
to be performed under SQL transactions and the game server's won't have
any other way of performing SQL queries. This will help to guard against
item dupes in case of crashes, etc.

#### Network packet base/protocol subsystem
Network packets are defined as C++ structures. In order to support multiple
packet versions at compiletime, avoid overhead and at the same time
retain the high-level accessiblity these structures are defined in a certain way.

##### Packet database versions
The packet database is a header-only library located in 'packets' subdirectory.
Packet database versions are defined as class names under ares::packet_version
namespace. To select a packet version during compile time ARES_PACKET_VER should
be set as a C define (using CMake command line -D option);
All possible packet versions are included in src/packets.hpp file. Each
packet version should be derived from another packet version, except the
base packet version, forming thus a packet version hierarchy. There are no special
rules about how to form a packet version, you may take, for example packet
versions of two Ragexe's and derive a new packet version as a combined definition
just by inherting from both (given that this makes sense in terms of conflicts).

Each packet version defines to classes: one for packet ids, defined in
packet_ids.hpp file, and one for the actual packets.
Packet ids structure simply defines static constants for the first word in packet's
header, e.g.:
```c++
namespace ares {
  template <>
  struct packet_ids<packet_version::base> {
    static const uint16_t AC_REFUSE_LOGIN = 0x6a;
    static const uint16_t SC_NOTIFY_BAN = 0x81;
    static const uint16_t CA_EXE_HASHCHECK = 0x204;
    static const uint16_t CA_SSO_LOGIN_REQ = 0x825;
    static const uint16_t AC_ACCEPT_LOGIN = 0xac4;
    static const uint16_t CH_ENTER = 0x65;
    static const uint16_t HC_REFUSE_ENTER = 0x6c;
    ...
  };
```

The packets structure definition defines the actual packet structure. Since
there're quite a lot of them, they are subincluded as separate files:
```c++
#pragma once

#include "../utils.hpp"

#pragma pack(push, 1)
namespace ares {
  template <>
  struct packets<packet_version::base> {
    using packet_id = packet_ids<packet_version::base>;
    
    // Common client
    #include "SC/SC_NOTIFY_BAN.hpp"
    // ...

    // Account <-> Client
    #include "CA/CA_EXE_HASHCHECK.hpp"
    #include "CA/CA_SSO_LOGIN_REQ.hpp"
    #include "AC/AC_ACCEPT_LOGIN.hpp"
    #include "AC/AC_REFUSE_LOGIN.hpp"
    // ...

    // Char <-> Client
    #include "CH/CH_ENTER.hpp"
    #include "HC/HC_REFUSE_ENTER.hpp"
    // ...

    // Account <-> Char
    #include "ATHENA/ATHENA_HA_PING_REQ.hpp"
    #include "ATHENA/ATHENA_AH_PING_ACK.hpp"
    #include "ATHENA/ATHENA_HA_LOGIN_REQ.hpp"
    #include "ATHENA/ATHENA_AH_LOGIN_RESULT.hpp"
    // ...

    // Char <-> Zone
    #include "ATHENA/ATHENA_ZH_LOGIN_REQ.hpp"
    #include "ATHENA/ATHENA_HZ_LOGIN_RESULT.hpp"
    // ...
  };
}
#pragma pack(pop)
```

##### Packet definition
A packet definition must follow carefuly crafted rules to provide both
type safety and efficiency. Here are the design requirements that were
implemented in this system:
- The structure has to be type-castable to a raw memory chunk for parsing
of incomming packets. No copy operations are allowed for this
scenario. The structure contains only raw C types.
- The structure has to be in-place constructible to create a packet
at a desired space in memory (right in the send buffer). In this
case copy operations are required
- The structure has to be constructible not in-place as well, for some
situations when the copy operation is inevitable (e.g. fragmented
buffer when sending)
- The structure has to be createable from safe C++ types in server's
objects. That means we must accept const std::string&, from example, in
our constructor and copy them as a raw C char*.
- All of the constructors have to be consistent between each other.
That means that C++ argument constructor should be defined in terms of
raw C argument constructor, and raw C argument constructor has to be
defined in terms of in-place factory constructor, etc.
- The structure must prevent direct write access to it's members, so we need
make them private and create read accessors for them
- The structure still has to define PacketType (packet id) and PacketLength (if exists)
members as public. That is needed for packet_size utilities, that determine
the packet size solely based on structure's packet id, sizeof(structure) and
the existence of PacketLength field. Otherwise we would have to specify manually
which packet is dynamically sized (like in Athena), or give up the typecasting method and
use anti-patterns like adding a VTABLE to packet structure (like it is supposedly
done in the original server).
- Precautions should be taken to zero-out the unused memory in the packet
when creating in order not to leak data.
A naive implementation (sometimes used in Athena) could be to
zero out the whole structure before filling it out, but that is largely inefficient
for big packets. 

Here's an example how a packet could be defined:
```c++
struct MY_PACKET {
  // Low level in-place constructor, to construct right inside send buffer
  void emplace(const char* contents, const size_t contents_sz,
               const size_t dyn_contents_sz) {
    // Set the packet id from packet_id database
    PacketType = packet_id::MY_PACKET;
    // Set the packet length
    PacketLength = sizeof(MY_PACKET) + dyn_contents_sz;
    // Copy the contents and zero out only unused bytes
    copy_buf_with_zero_pad(contents_, sizeof(contents_), contents, contents_sz);
    // The dynamic packet tail (here - dyn_contents_ field) will be sent as a send chunk
  }
  // In-place constructor for safe C++ types. That is the one the servers will call,
  // since they don't operate on unsafe C types like char* directly
  void emplace(const std::string& contents, // It's a safe C++ type, we don't need to pass the size as with char*
               const size_t dyn_contents_sz) {
    // Define it in terms of C in-place constructor
    emplace(contents.c_str(), contents.size(), dyn_contents_sz);
  }
  // Non-in-place constructors, here were are just mirroring/calling the corresponding in-place constructors
  // Raw C type arguments version
  explicit MY_PACKET(const char* contents, const size_t contents_sz,
                     const size_t dyn_contents_sz) {
    emplace(contents, contents_sz, dyn_contents_sz);                     
  }
  // Safe C++ type arguments version
  explicit MY_PACKET(const std::string& contents,
                     const size_t dyn_contents_sz) {
    emplace(contents, dyn_contents_sz);                     
  }
  // Read accessors for packet's fields
  const char* contents() const { return contents_; };
  const char* dyn_contents() const { return dyn_contents_; };

  uint16_t PacketType; // Packet id, has to be public for packet_size template
  uint16_t PacketLength; // Packet length, has to be public for packet_size template
private:
  char contents_[24]; // Statically-sized packet field
  char dyn_contents_[]; // Dynamically-sized packet field
};
```



# Naming convention consistency
Names in general are chosen from Athena server, but
are replaced with original server's names if known. For example, packet length field in
dynamic packets is called PacketLength and the server process called map server in
Athena is called zone server. This naming convention should be sticked to and
have prevalence over coding style guidelines.

# Coding style guidelines
The server is written in C++ and adopts modern C++ programming style.
The code looks pretty much like you would see on cppreference.com site:
lower-case underscore_separated identifiers, CamelCase is used primarily only for
template arguments, no tabs, indention is 2 space characters, etc.
This however can be overridden by naming convention consistency rules (see "Naming convention
consistency"), and since the original server does not seem to follow any
coding standards at all in this regard, some identifiers violating the naming
rules can be seen.

Considering the potential loads and clean C++ programming techniques,
some rules are observed when choosing C++ coding paths:
 - No RTTI-based techinques are allowed, like dynamic polymorphism, unless strictly needed
 - Memory allocations should be kept to minimum
 - No dangerous C preprocessor macros unless strictly needed. C macros are used when
 there's no C++ way of achieving the same thing, i.e. creating code that consciously cross
 the boundaries of type system. No #ifdefs.

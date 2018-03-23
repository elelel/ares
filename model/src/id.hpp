#pragma once

/*! \file id.hpp
  Different ids used across the server.
  The reason for defining them is to prevent accidental nonsensical morphisms, e.g. assigning account_id to AID directly by uint32_t value and to abstract conversions
*/

#include <limits>

namespace ares {
  namespace model {
    namespace detail {
      struct id_base {
        inline id_base();
        inline id_base(const id_base& other);
        inline id_base(id_base&& other);
        
        std::string to_string() const;
      protected:
        inline explicit id_base(const uint32_t& uint32);
        
        uint32_t value_;
      };
    }
    
    struct AID;
    struct GID;
    struct account_id;
    struct character_id;

    /*! AID used to identify game entities, to interface with the client
     */
    struct AID : detail::id_base {
      friend struct account_id;

      using detail::id_base::id_base;

      inline AID();
      inline AID(const account_id& acc_id);
      
      inline bool operator==(const AID& other) const;
      inline bool operator<(const AID& other) const;

      template <typename id_type>
      inline bool in_bounds() const;

      inline AID& operator=(const AID& other);
      
      inline static AID from_uint32(const uint32_t& uint32);
    };

    /*! GID used to identify game characters, to interface with the client
     */
    struct GID : detail::id_base {
      friend struct character_id;
      
      using detail::id_base::id_base;

      inline GID();
      inline GID(const character_id& char_id);
      
      inline bool operator==(const GID& other) const;
      inline bool operator<(const GID& other) const;

      template <typename id_type>
      inline bool in_bounds() const;

      inline GID& operator=(const GID& other);
      
      inline static GID from_uint32(const uint32_t& uint32);
    };

    /*! account id, as stored in the database
     */
    struct account_id : detail::id_base {
      friend struct AID;

      using detail::id_base::id_base;
      
      inline account_id();
      inline account_id(const AID& aid);

      inline account_id& operator=(const account_id& other);

      inline bool operator==(const account_id& other) const;
      inline bool operator<(const account_id& other) const;

      inline static account_id from_uint32(const uint32_t& uint32);
    };

    /*! character id, as stored in the databse
     */
    struct character_id : detail::id_base {
      friend struct GID;
      
      using detail::id_base::id_base;

      inline character_id();
      inline character_id(const GID& gid);
      
      inline character_id& operator=(const character_id& other);

      inline bool operator==(const character_id& other) const;
      inline bool operator<(const character_id& other) const;

      inline static character_id from_uint32(const uint32_t& uint32);
    };

    /*! mob id
     */
    struct mob_id : detail::id_base {
    };


    template <typename id_type>
    struct AID_limits {
    };

    template<>
    struct AID_limits<mob_id> {
      static uint32_t min() { return 3000; };
      static uint32_t max() { return 31999; };
    };
    
    template <>
    struct AID_limits<account_id> {
      static uint32_t min() { return 2000000; };
      static uint32_t max() { return std::numeric_limits<uint32_t>::max(); };
    };
    
    template <typename id_type>
    struct GID_limits {
    };

    template<>
    struct GID_limits<character_id> {
      static uint32_t min() { return 100000; };
      static uint32_t max() { return std::numeric_limits<uint32_t>::max(); };
    };


  }
}

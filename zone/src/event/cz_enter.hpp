#pragma once

namespace ares {
  namespace zone {
    namespace event {
      struct cz_enter {
        explicit cz_enter(const uint32_t& aid,
                          const uint32_t& cid,
                          const int32_t& auth_code1,
                          const uint32_t& client_time,
                          const uint8_t& sex) :
          data(std::make_shared<shared>(std::move(shared{aid, cid, auth_code1, client_time, sex}))) {
        }

        template <typename PacketSet, typename PacketName>
        inline static cz_enter from_packet(const packet::type<PacketSet, PacketName>& p) {
          return cz_enter(p.AID(), p.GID(), p.AuthCode(), p.clientTime(), p.Sex());
        }
            
        struct shared {
          uint32_t aid;
          uint32_t cid;
          int32_t auth_code1;
          uint32_t client_time;
          uint8_t sex;
        };
            
        std::shared_ptr<shared> data;
      };
    }
  }
}

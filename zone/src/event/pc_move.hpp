namespace ares {
  namespace zone {
    namespace event {
      /*! PC move command from client
       */
      struct pc_move_command {
        explicit pc_move_command(const uint32_t gid,
                                 const uint16_t to_x,
                                 const uint16_t to_y) :
          data(std::make_shared<shared>()) {
          data->gid = gid;
          data->to_x = to_x;
          data->to_y = to_y;
        }
            
        struct shared {
          uint32_t gid;
          uint16_t to_x;
          uint16_t to_y;
        };
        std::shared_ptr<shared> data;
      };
    }
  }
}

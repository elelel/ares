
namespace ares {
  /*! Key character information
   */
  struct char_main {
    /*! Character id (cid) in SQL database */
    uint32_t cid;
    uint32_t aid;
    uint8_t slot;
    std::string name;
    uint8_t sex;
    uint16_t job;
          
    int8_t rename;
    std::optional<std::chrono::system_clock::time_point> delete_date;
  };

  /*! Character appearance 
   */
  struct char_appearance {
    int16_t head;
    int16_t body;
    int16_t weapon;
    int16_t shield;
    int16_t head_top; // accessory2
    int16_t head_mid; // accessory3
    int16_t head_bottom; // accessory
    int16_t head_palette;
    int16_t body_palette;
    int32_t robe;
  };

  /*! Stat points information for a character
   */
  struct char_stats {
    uint32_t base_level;
    uint32_t job_level;
    uint64_t base_exp;
    uint32_t job_exp;
    uint64_t zeny;
          
    int8_t Str;
    int8_t Agi;
    int8_t Vit;
    int8_t Int;
    int8_t Dex;
    int8_t Luk;
          
    int32_t max_hp;
    int32_t hp;
    int32_t max_sp;
    int32_t sp;
    uint8_t job_point;
    uint8_t skill_point;
    int32_t body_state;
    int32_t health_state;
    int32_t effect_state;  // 'option'
    uint8_t virtue;
    int16_t honor;
  };
        
  /*! Character's saved location info
   */
  struct char_location {
    std::string map_name;
    uint16_t map_x;
    uint16_t map_y;
  };

  /*! For CHARACTER_INFO client structure (sent on logon to char server)
   */
  struct character_info {
    char_main main;
    char_appearance appearance;
    char_stats stats;
    char_location location;
  };
}

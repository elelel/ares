#pragma once

namespace ares {
  namespace model {
    struct pc_name_string : fixed_string<24> {
      using fixed_string<24>::fixed_string;
    };

    enum class pc_job : uint16_t {
      NOVICE = 0,
      SWORDMAN,
      AGE,
      ARCHER,
      ACOLYTE,
      MERCHANT,
      THIEF,
      KNIGHT,
      PRIEST,
      WIZARD,
      BLACKSMITH,
      HUNTER,
      ASSASSIN,
      KNIGHT2,
      CRUSADER,
      MONK,
      SAGE,
      ROGUE,
      ALCHEMIST,
      BARD,
      DANCER,
      CRUSADER2,
      WEDDING,
      SUPER_NOVICE,
      GUNSLINGER,
      NINJA,
      XMAS,
      SUMMER,
      MAX_BASIC,

      NOVICE_HIGH = 4001,
      SWORDMAN_HIGH,
      MAGE_HIGH,
      ARCHER_HIGH,
      ACOLYTE_HIGH,
      MERCHANT_HIGH,
      THIEF_HIGH,
      LORD_KNIGHT,
      HIGH_PRIEST,
      HIGH_WIZARD,
      WHITESMITH,
      SNIPER,
      ASSASSIN_CROSS,
      LORD_KNIGHT2,
      PALADIN,
      CHAMPION,
      PROFESSOR,
      STALKER,
      CREATOR,
      CLOWN,
      GYPSY,
      PALADIN2,

      BABY,
      BABY_SWORDMAN,
      BABY_MAGE,
      BABY_ARCHER,
      BABY_ACOLYTE,
      BABY_MERCHANT,
      BABY_THIEF,
      BABY_KNIGHT,
      BABY_PRIEST,
      BABY_WIZARD,
      BABY_BLACKSMITH,
      BABY_HUNTER,
      BABY_ASSASSIN,
      BABY_KNIGHT2,
      BABY_CRUSADER,
      BABY_MONK,
      BABY_SAGE,
      BABY_ROGUE,
      BABY_ALCHEMIST,
      BABY_BARD,
      BABY_DANCER,
      BABY_CRUSADER2,
      SUPER_BABY,

      TAEKWON,
      STAR_GLADIATOR,
      STAR_GLADIATOR2,
      SOUL_LINKER,

      GANGSI,
      DEATH_KNIGHT,
      DARK_COLLECTOR,

      RUNE_KNIGHT = 4054,
      WARLOCK,
      RANGER,
      ARCH_BISHOP,
      MECHANIC,
      GUILLOTINE_CROSS,

      RUNE_KNIGHT_T,
      WARLOCK_T,
      RANGER_T,
      ARCH_BISHOP_T,
      MECHANIC_T,
      GUILLOTINE_CROSS_T,

      ROYAL_GUARD,
      SORCERER,
      MINSTREL,
      WANDERER,
      SURA,
      GENETIC,
      SHADOW_CHASER,

      ROYAL_GUARD_T,
      SORCERER_T,
      MINSTREL_T,
      WANDERER_T,
      SURA_T,
      GENETIC_T,
      SHADOW_CHASER_T,

      RUNE_KNIGHT2,
      RUNE_KNIGHT_T2,
      ROYAL_GUARD2,
      ROYAL_GUARD_T2,
      RANGER2,
      RANGER_T2,
      MECHANIC2,
      MECHANIC_T2,

      BABY_RUNE = 4096,
      BABY_WARLOCK,
      BABY_RANGER,
      BABY_BISHOP,
      BABY_MECHANIC,
      BABY_CROSS,
      BABY_GUARD,
      BABY_SORCERER,
      BABY_MINSTREL,
      BABY_WANDERER,
      BABY_SURA,
      BABY_GENETIC,
      BABY_CHASER,

      BABY_RUNE2,
      BABY_GUARD2,
      BABY_RANGER2,
      BABY_MECHANIC2,

      SUPER_NOVICE_E = 4190,
      SUPER_BABY_E,

      KAGEROU = 4211,
      OBORO,
      REBELLION = 4215,

      SUMMONER = 4218,

      MAX
    };


    /*! Character info
     */
    struct pc_info {
      model::account_id account_id = model::account_id::from_uint32(0);
      model::character_id character_id = model::character_id::from_uint32(0);

      pc_name_string name;
      uint8_t sex;
      pc_job job;
      uint8_t slot;
      int8_t rename;

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
 
      uint32_t base_level;
      uint32_t job_level;
      uint64_t base_exp;
      uint32_t job_exp;
      uint64_t zeny;
      int16_t speed;
          
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
 
      map_location location_last;
      map_location location_save;

    };
  }
}

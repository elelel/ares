DROP TABLE IF EXISTS accounts;
CREATE TABLE accounts (
  id int PRIMARY KEY NOT NULL,
  normal_slots smallint NOT NULL,
  premium_slots smallint NOT NULL,
  billing_slots smallint NOT NULL,
  creatable_slots smallint NOT NULL,
  playable_slots smallint NOT NULL,
  bank_vault bigint NOT NULL,
  max_storage bigint NOT NULL
);

DROP TABLE IF EXISTS characters;
CREATE TABLE characters (
  id serial PRIMARY KEY,
  slot smallint NOT NULL,
  "name" varchar NOT NULL,
  sex smallint NOT NULL,
  job smallint NOT NULL,
  base_level smallint NOT NULL,
  job_level smallint NOT NULL,
  base_exp int NOT NULL,
  job_exp int NOT NULL,
  zeny bigint NOT NULL,

  str smallint NOT NULL,
  agi smallint NOT NULL,
  vit smallint NOT NULL,
  "int" smallint NOT NULL,
  dex smallint NOT NULL,
  luk smallint NOT NULL,
  
  max_hp int NOT NULL,
  hp int NOT NULL,
  max_sp int NOT NULL,
  sp int NOT NULL,
  job_point smallint NOT NULL,
  skill_point smallint NOT NULL,
  effect_state smallint NOT NULL,
  body_state smallint NOT NULL, 
  health_state smallint NOT NULL,
  virtue smallint NOT NULL,
  honor smallint NOT NULL,
  
  head smallint NOT NULL,
  body smallint NOT NULL,
  weapon smallint NOT NULL,
  robe smallint NOT NULL,
  shield smallint NOT NULL,
  head_top smallint NOT NULL,
  head_mid smallint NOT NULL,
  head_bottom smallint NOT NULL,
  head_palette smallint NOT NULL,
  body_palette smallint NOT NULL,

  lastmap_name varchar NOT NULL,
  last_map_x smallint NOT NULL,
  last_map_y smallint NOT NULL,
  delete_date timestamp,
  rename smallint NOT NULL
);


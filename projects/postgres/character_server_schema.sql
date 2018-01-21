DROP TABLE IF EXISTS accounts;
CREATE TABLE accounts (
  id int PRIMARY KEY,
  normal_slots smallint,
  premium_slots smallint,
  billing_slots smallint,
  creatable_slots smallint,
  playable_slots smallint,
  bank_vault bigint,
  max_storage bigint
);

DROP TABLE IF EXISTS characters;
CREATE TABLE characters (
  id serial PRIMARY KEY,
  slot smallint,
  "name" varchar,
  sex smallint,
  char_class smallint,
  base_level smallint,
  job_level smallint,
  base_exp int,
  job_exp int,
  zeny bigint,
  str smallint,
  agi smallint,
  vit smallint,
  "int" smallint,
  dex smallint,
  luk smallint,
  max_hp int,
  hp int,
  max_sp int,
  sp int,
  status_point smallint,
  skill_point smallint,
  "option" smallint,
  karma smallint,
  manner smallint,
  hair smallint,
  hair_color smallint,
  clothes_color smallint,
  body smallint,
  weapon smallint,
  shield smallint,
  head_top smallint,
  head_mid smallint,
  head_bottom smallint,
  robe smallint,
  lastmap_name varchar,
  last_map_x smallint,
  last_map_y smallint
);


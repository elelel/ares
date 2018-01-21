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


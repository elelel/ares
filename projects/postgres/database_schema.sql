# CREATE EXTENSION pgcrypto;

DROP TABLE IF EXISTS users CASCADE;
CREATE TABLE users (
  id serial PRIMARY KEY,
  login varchar UNIQUE NOT NULL,
  password varchar NOT NULL,
  email varchar NOT NULL,
  level int NOT NULL,
  sex int NOT NULL,
  expiration_time timestamp,
  birthdate date,
  pin varchar(4)
);

DROP TABLE IF EXISTS account_slots;
CREATE TABLE account_slots (
  aid int REFERENCES users(id) ON DELETE CASCADE,
  normal_slots smallint NOT NULL,
  premium_slots smallint NOT NULL,
  billing_slots smallint NOT NULL,
  creatable_slots smallint NOT NULL,
  playable_slots smallint NOT NULL
);

DROP TABLE IF EXISTS account_storage;
CREATE TABLE account_storage (
  aid int REFERENCES users(id) ON DELETE CASCADE,
  bank_vault bigint NOT NULL,
  max_storage bigint NOT NULL
);

DROP TABLE IF EXISTS characters CASCADE;
CREATE TABLE characters (
  id serial PRIMARY KEY,
  aid int NOT NULL,
  slot smallint NOT NULL,
  "name" varchar NOT NULL UNIQUE,
  sex smallint NOT NULL,
  job smallint NOT NULL,
  delete_date timestamp,
  rename smallint NOT NULL
);
DROP INDEX IF EXISTS characters_aid_slot_idx;
CREATE UNIQUE INDEX characters_aid_slot_idx ON characters (aid, slot);

DROP TABLE IF EXISTS char_appearance;
CREATE TABLE char_appearance (
  cid int UNIQUE REFERENCES characters(id) ON DELETE CASCADE, 
  head smallint NOT NULL,
  body smallint NOT NULL,
  weapon smallint NOT NULL,
  shield smallint NOT NULL,
  robe smallint NOT NULL,
  head_top smallint NOT NULL,
  head_mid smallint NOT NULL,
  head_bottom smallint NOT NULL,
  head_palette smallint NOT NULL,
  body_palette smallint NOT NULL
);

DROP TABLE IF EXISTS char_stats;
CREATE TABLE char_stats (
  cid int UNIQUE REFERENCES characters(id) ON DELETE CASCADE, 
  base_level smallint NOT NULL,
  job_level smallint NOT NULL,
  base_exp int NOT NULL,
  job_exp int NOT NULL,

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
  honor smallint NOT NULL
);

DROP TABLE IF EXISTS char_zeny;
CREATE TABLE char_zeny (
  cid int UNIQUE REFERENCES characters(id) ON DELETE CASCADE, 
  zeny bigint NOT NULL
);  

DROP TABLE IF EXISTS maps;
CREATE TABLE maps (
  id serial PRIMARY KEY NOT NULL,
  name varchar(18) UNIQUE NOT NULL,
  x_size smallint NOT NULL,
  y_size smallint NOT NULL,
  cell_flags bytea NOT NULL,

  external_id int
);

DROP TABLE IF EXISTS char_last_location;
CREATE TABLE char_last_location (
  cid int UNIQUE REFERENCES characters(id) ON DELETE CASCADE,
  map_id int REFERENCES maps(id),
  x smallint NOT NULL,
  y smallint NOT NULL
);

DROP TABLE IF EXISTS char_save_location;
CREATE TABLE char_save_location (
  cid int UNIQUE REFERENCES characters(id) ON DELETE CASCADE,
  map_id int REFERENCES maps(id),
  x smallint NOT NULL,
  y smallint NOT NULL
);

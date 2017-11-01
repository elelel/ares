# CREATE EXTENSION pgcrypto;

DROP TABLE IF EXISTS users;
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

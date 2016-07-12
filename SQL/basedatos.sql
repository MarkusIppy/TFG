CREATE TABLE tracks (
  id INTEGER,
  initdate DATE,
  inithour TIME,
  initpos GIS,
  distance INTEGER,
  area GIS,
  polylind GIS,
  GPS INT,
  PRIMARY KEY (id),
  people_id INTEGER,
  car_vin VARCHAR(30),
  FOREIGN KEY (people_id) REFERENCES people(id),
  FOREIGN KEY (car_vin) REFERENCES vehicles(car_vin)
  );

CREATE TABLE vehicles (
  car_vin VARCHAR(30),
  PRIMARY KEY (car_vin)
  );

CREATE TABLE models (
  engine VARCHAR(15),
  cc INTEGER,
  fuel VARCHAR(1),
  hp INTEGER,
  weight INTEGER
  car_vin VARCHAR(30),
  PRIMARY KEY (car_vin)
  );

CREATE TABLE obd_parameters (
  hexcode INTEGER,
  label VARCHAR(20),
  PRIMARY KEY (hexcode)
  );

CREATE TABLE samples (
  Pname VARCHAR(15),
  INTvalue INTEGER,
  FLOATvalue FLOAT,
  Ts INTEGER,
  PRIMARY KEY (Pname, Ts),
  hexcode INTEGER,
  tracks_id INTEGER, 
  FOREIGN KEY (hexcode) REFERENCES obd_parameters(hexcode),
  FOREIGN KEY (tracks_id) REFERENCES tracks(id)
  );

CREATE TABLE people (
  id INTEGER,
  name VARCHAR(40),
  PRIMARY KEY (id)
  );

CREATE TABLE vehper (
  car_vin VARCHAR(30),
  people_id INTEGER,
  PRIMARY KEY (car_vin, people_id),
  FOREIGN KEY (car_vin) REFERENCES vehicles(car_vin),
  FOREIGN KEY (people_id) REFERENCES people(id)
  );

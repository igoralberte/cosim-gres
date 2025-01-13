CREATE TABLE images(
	id serial,
	img bytea,
	signature bytea,
	path text,
	queryable boolean,
	CONSTRAINT primary_key UNIQUE(id)
);

CREATE TABLE images100(
	id serial,
	img bytea,
	signature bytea,
	path text,
	queryable boolean,
	CONSTRAINT primary_key100 UNIQUE(id)
);

CREATE TABLE images5k(
	id serial,
	img bytea,
	signature bytea,
	path text,
	queryable boolean,
	CONSTRAINT primary_key5k UNIQUE(id)
);

CREATE TABLE images5k_color(
	id serial,
	img bytea,
	signature bytea,
	path text,
	queryable boolean,
	CONSTRAINT primary_key5kcolor UNIQUE(id)
);

CREATE TABLE images5k_color_structure(
	id serial,
	img bytea,
	signature bytea,
	path text,
	queryable boolean,
	CONSTRAINT primary_key5kcolor_structure UNIQUE(id)
);

CREATE TABLE images5k_histogram(
	id serial,
	img bytea,
	signature bytea,
	path text,
	queryable boolean,
	CONSTRAINT primary_key5khistogram UNIQUE(id)
);




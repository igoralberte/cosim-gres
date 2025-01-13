-- use database kiara
\c kiara

-- create table points3d
CREATE TABLE points3d(
id INT primary key,
x DOUBLE PRECISION,
y DOUBLE PRECISION,
z DOUBLE PRECISION,
feature_vector DOUBLE PRECISION[],
feature_vector_bytea bytea);

-- inserting data
INSERT INTO points3d(id, x, y, z) VALUES
(1, 1, 1, 1),
(2, 2, 2, 2),
(3, 3, 3, 3),
(4, 1, 2, 3),
(5, 10, 5, 5),
(6, 8.5, 4.2, 8.9),
(7, -5.6, -3.2, 1.1);

-- setting the feature_vector as [x, y, z]
UPDATE points3d SET feature_vector = ARRAY[x, y, z];

-- setting the feature_vector_bytea as the bytea version of feature_vector
UPDATE points3d SET feature_vector_bytea = text_to_signature(array_to_string(feature_vector, ' ', '*'));

-- Range Query with radius = 3; query center = tuple with id=1; euclidean distance
SELECT * FROM select_simrange('points3d', 'feature_vector_bytea', 'euclidean_distance', (SELECT feature_vector_bytea FROM points3d WHERE id=1), 3, 'id');

-- KNN Query with k=4; query center = tuple with id=2; euclidean distance;
SELECT * FROM select_knn('points3d', 'feature_vector_bytea', 'euclidean_distance', (SELECT feature_vector_bytea FROM points3d WHERE id=2), 4, 'id');

-- creating index on Points3d table, with euclidean distance
SELECT simql_create_index('idx_points3d_euclidean', 'points3d', 'id', 'feature_vector_bytea', 'euclidean_distance', 8192);

-- query the created indexes
SELECT * FROM similarql_indexes;

-- show the details about query execution
UPDATE similarql_settings SET value = TRUE WHERE param = 'similarql_d_trace';



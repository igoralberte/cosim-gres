-- Just to be sure that we are working on the right database
\c kiara

-----------------------------------------------------------
-- INSERT METADATA
-----------------------------------------------------------
-- Creates the image type
INSERT INTO metatable_complex_type (name) VALUES ('IMAGE');

-- Creates all the distance functions
INSERT INTO metatable_df (reserved_word) VALUES ('manhattan_distance');
INSERT INTO metatable_df (reserved_word) VALUES ('euclidean_distance');
INSERT INTO metatable_df (reserved_word) VALUES ('chebyshev_distance');
INSERT INTO metatable_df (reserved_word) VALUES ('canberra_distance');
INSERT INTO metatable_df (reserved_word) VALUES ('kullback_leibler_divergence_distance');
INSERT INTO metatable_df (reserved_word) VALUES ('jeffrey_divergence_distance');
INSERT INTO metatable_df (reserved_word) VALUES ('hamming_distance');

-- Creates all the feature extraction methods
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('color_layout_extractor',
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('scalable_color_extractor',
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('color_structure_extractor',
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('color_temperature_extractor',
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('edge_histogram_extractor',
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('texture_browsing_extractor',
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('lbp_extractor',
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('haralick_extractor',
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));
INSERT INTO metatable_fem (reserved_word, complex_type_id) VALUES ('pHash_extractor', 
	(SELECT id FROM metatable_complex_type WHERE name='IMAGE'));

-- Creates all the possible metrics (combinations, so far)
-- Color Layout - CL
SELECT create_fem_instance('color_layout_extractor_instance', 'color_layout_extractor');
SELECT create_metric('CL_CB', 'manhattan_distance',                   'color_layout_extractor_instance');
SELECT create_metric('CL_EU', 'euclidean_distance',                   'color_layout_extractor_instance');
SELECT create_metric('CL_CH', 'chebyshev_distance',                   'color_layout_extractor_instance');
SELECT create_metric('CL_CA', 'canberra_distance',                    'color_layout_extractor_instance');
SELECT create_metric('CL_KU', 'kullback_leibler_divergence_distance', 'color_layout_extractor_instance');
SELECT create_metric('CL_JF', 'jeffrey_divergence_distance',          'color_layout_extractor_instance');


-- Scalable Color - SC
SELECT create_fem_instance('scalable_color_extractor_instance', 'scalable_color_extractor');
SELECT create_metric('SC_CB', 'manhattan_distance',                   'scalable_color_extractor_instance');
SELECT create_metric('SC_EU', 'euclidean_distance',                   'scalable_color_extractor_instance');
SELECT create_metric('SC_CH', 'chebyshev_distance',                   'scalable_color_extractor_instance');
SELECT create_metric('SC_CA', 'canberra_distance',                    'scalable_color_extractor_instance');
SELECT create_metric('SC_KU', 'kullback_leibler_divergence_distance', 'scalable_color_extractor_instance');
SELECT create_metric('SC_JF', 'jeffrey_divergence_distance',          'scalable_color_extractor_instance');

-- Color Structure - CS
SELECT create_fem_instance('color_structure_extractor_instance', 'color_structure_extractor');
SELECT create_metric('CS_CB', 'manhattan_distance',                   'color_structure_extractor_instance');
SELECT create_metric('CS_EU', 'euclidean_distance',                   'color_structure_extractor_instance');
SELECT create_metric('CS_CH', 'chebyshev_distance',                   'color_structure_extractor_instance');
SELECT create_metric('CS_CA', 'canberra_distance',                    'color_structure_extractor_instance');
SELECT create_metric('CS_KU', 'kullback_leibler_divergence_distance', 'color_structure_extractor_instance');
SELECT create_metric('CS_JF', 'jeffrey_divergence_distance',          'color_structure_extractor_instance');

-- Color Temperature - CT
SELECT create_fem_instance('color_temperature_extractor_instance', 'color_temperature_extractor');
SELECT create_metric('CT_CB', 'manhattan_distance',                   'color_temperature_extractor_instance');
SELECT create_metric('CT_EU', 'euclidean_distance',                   'color_temperature_extractor_instance');
SELECT create_metric('CT_CH', 'chebyshev_distance',                   'color_temperature_extractor_instance');
SELECT create_metric('CT_CA', 'canberra_distance',                    'color_temperature_extractor_instance');
SELECT create_metric('CT_KU', 'kullback_leibler_divergence_distance', 'color_temperature_extractor_instance');
SELECT create_metric('CT_JF', 'jeffrey_divergence_distance',          'color_temperature_extractor_instance');

-- Edge Histogram - EH
SELECT create_fem_instance('edge_histogram_extractor_instance', 'edge_histogram_extractor');
SELECT create_metric('EH_CB', 'manhattan_distance',                   'edge_histogram_extractor_instance');
SELECT create_metric('EH_EU', 'euclidean_distance',                   'edge_histogram_extractor_instance');
SELECT create_metric('EH_CH', 'chebyshev_distance',                   'edge_histogram_extractor_instance');
SELECT create_metric('EH_CA', 'canberra_distance',                    'edge_histogram_extractor_instance');
SELECT create_metric('EH_KU', 'kullback_leibler_divergence_distance', 'edge_histogram_extractor_instance');
SELECT create_metric('EH_JF', 'jeffrey_divergence_distance',          'edge_histogram_extractor_instance');

-- Texture Browsing - TB
SELECT create_fem_instance('texture_browsing_extractor_instance', 'texture_browsing_extractor');
SELECT create_metric('TB_CB', 'manhattan_distance',                   'texture_browsing_extractor_instance');
SELECT create_metric('TB_EU', 'euclidean_distance',                   'texture_browsing_extractor_instance');
SELECT create_metric('TB_CH', 'chebyshev_distance',                   'texture_browsing_extractor_instance');
SELECT create_metric('TB_CA', 'canberra_distance',                    'texture_browsing_extractor_instance');
SELECT create_metric('TB_KU', 'kullback_leibler_divergence_distance', 'texture_browsing_extractor_instance');
SELECT create_metric('TB_JF', 'jeffrey_divergence_distance',          'texture_browsing_extractor_instance');

-- Linear Binary Pattern - LBP
SELECT create_fem_instance('lbp_extractor_instance', 'lbp_extractor');
SELECT create_metric('LBP_CB', 'manhattan_distance',                   'lbp_extractor_instance');
SELECT create_metric('LBP_EU', 'euclidean_distance',                   'lbp_extractor_instance');
SELECT create_metric('LBP_CH', 'chebyshev_distance',                   'lbp_extractor_instance');
SELECT create_metric('LBP_CA', 'canberra_distance',                    'lbp_extractor_instance');
SELECT create_metric('LBP_KU', 'kullback_leibler_divergence_distance', 'lbp_extractor_instance');
SELECT create_metric('LBP_JF', 'jeffrey_divergence_distance',          'lbp_extractor_instance');

-- Haralick - HR
SELECT create_fem_instance('haralick_extractor_instance', 'haralick_extractor');
SELECT create_metric('HR_CB', 'manhattan_distance',                   'haralick_extractor_instance');
SELECT create_metric('HR_EU', 'euclidean_distance',                   'haralick_extractor_instance');
SELECT create_metric('HR_CH', 'chebyshev_distance',                   'haralick_extractor_instance');
SELECT create_metric('HR_CA', 'canberra_distance',                    'haralick_extractor_instance');
SELECT create_metric('HR_KU', 'kullback_leibler_divergence_distance', 'haralick_extractor_instance');

-- pHash Extractor - HD
SELECT create_fem_instance('pHash_extractor_instance', 'pHash_extractor');
SELECT create_metric('HE_HD', 'hamming_distance',                   'pHash_extractor_instance');
-----------------------------------------------------------
-- INSERT METADATA
-----------------------------------------------------------

 CREATE OR REPLACE FUNCTION bytea_import(p_path text, OUT p_result bytea)
  RETURNS bytea
  LANGUAGE plpgsql
 AS $function$
 DECLARE
   l_oid OID;
   r RECORD;
 BEGIN
   p_result := '';
   SELECT lo_import(p_path) INTO l_oid;
   for r in ( SELECT data
              FROM pg_largeobject
              WHERE loid = l_oid
              ORDER BY pageno ) LOOP
     p_result = p_result || r.data;
   END LOOP;
   perform lo_unlink(l_oid);
 END;
 $function$;

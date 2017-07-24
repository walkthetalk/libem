CREATE TYPE EFTI_MODE		AS ENUM ('AUTO','FOLLOW','SM','DS','NZ','MM');
CREATE TYPE EFUSION_MODE	AS ENUM ('AUTO','CALIBRATE','NORMAL','SPECIAL');
CREATE TYPE EALIGN_MODE		AS ENUM ('AUTO','CLAD', 'CORE', 'FINE', 'MANUAL');
CREATE TYPE ELOSSEST_MODE	AS ENUM ('OFF', 'FINE', 'CORE', 'CLAD');
CREATE TYPE EFS_RESULT	AS ENUM (
		'SUCCESS',
		'MEDDIACY',
		'UNKOWN_ERR',
		'COVER_OPENNED',
		'NO_FIBER',
		'FIBER_DEFECT',
		'FIBER_RECO',
		'CROSS_OVER',
		'OFF_CENTER',
		'IMG_LUM',
		'ABNORMAL_ARC',
		'TENSE_TEST_FAIL',
		'FIBER_BROKEN',
		'QUIT_MIDWAY',
		'PUSH_TIMEOUT',
		'CALIBRATE_TIMEOUT',
		'RESET_TIMEOUT',
		'ARC_TIME_ZERO',
		'ARC1_REVISE',
		'ARC2_REVISE',
		'FOCUS_X',
		'FOCUS_Y',
		'IMG_PROCESS',
		'SYSTEM_ERR',
		'FIBER_OFFSIDE',
		'CMOS_EXP',
		'LOSS_EST',
		'ARC_POS',
		'ARC_OVERFLOW');
CREATE DOMAIN DT_VOLTAGE	AS DECIMAL(5,3);	-- unit: v
CREATE DOMAIN DT_LENGTH		AS DECIMAL(8,3);	-- unit: um
CREATE DOMAIN DT_TIME		AS INT4;		-- unit: ms
CREATE DOMAIN DT_SPEED		AS INT4;		-- unit: um/s (nm/ms)
CREATE DOMAIN DT_ANGLE		AS DECIMAL(5,2);	-- unit: degree
CREATE DOMAIN DT_LOSS		AS DECIMAL(8,3);	-- unit: dB

-- length     : xxxx.x um
-- angle      :   xx.x degree
-- arc mag    :   xx.xxx volt
-- coefficient:  xxx.xx
-- spped      :   xx.xxx
-- time       :        ms
CREATE TABLE fs_param (
	  seqn		INT2		NOT NULL	PRIMARY KEY
	, name		TEXT		NOT NULL
	, fusion_mode	EFUSION_MODE	NOT NULL	DEFAULT 'AUTO'
	, lfti		EFTI_MODE	NOT NULL	DEFAULT 'SM'
	, rfti		EFTI_MODE	NOT NULL	DEFAULT 'FOLLOW'
	, align_mode	EALIGN_MODE	NOT NULL	DEFAULT 'FINE'
	, x_focus	BOOL		NOT NULL	DEFAULT TRUE
	, y_focus	BOOL		NOT NULL	DEFAULT TRUE
	, ecf_redress	BOOL		NOT NULL	DEFAULT TRUE
	, auto_mag	BOOL		NOT NULL	DEFAULT TRUE
	, tense_test	BOOL		NOT NULL	DEFAULT FALSE
	, tense_speed	DT_SPEED	NOT NULL	DEFAULT 200	-- unit: um/s
	, tense_length	DT_LENGTH	NOT NULL	DEFAULT 100	-- unit: um
	, vangle_limit	DT_ANGLE	NOT NULL	DEFAULT 2.0
	, hangle_limit  DT_ANGLE	NOT NULL	DEFAULT 1.0
	, clr_mag	DT_VOLTAGE	NOT NULL	DEFAULT 0.3	-- unit: volt
	, clr_time	DT_TIME		NOT NULL	DEFAULT 300
	, clr_pos	DT_LENGTH	NOT NULL	DEFAULT 100	-- unit: um
	, position	DT_LENGTH	NOT NULL	DEFAULT 0
	, gap		DT_LENGTH	NOT NULL	DEFAULT 8
	, overlap	DT_LENGTH	NOT NULL	DEFAULT 6
	, pre_mag	DT_VOLTAGE	NOT NULL	DEFAULT 0.3	-- unit: volt
	, pre_time	DT_TIME		NOT NULL	DEFAULT 150
	, arc1_mag	DT_VOLTAGE	NOT NULL	DEFAULT 0.82
	, arc1_time	DT_TIME		NOT NULL	DEFAULT 2200
	, arc2_mag	DT_VOLTAGE	NOT NULL	DEFAULT 0.4
	, arc2_time	DT_TIME		NOT NULL	DEFAULT 0
	, arc2_on_time	DT_TIME		NOT NULL	DEFAULT 180
	, arc2_off_time	DT_TIME		NOT NULL	DEFAULT 0
	, arc_man_time	DT_TIME		NOT NULL	DEFAULT 800
	, taper_splice	BOOL		NOT NULL	DEFAULT FALSE
	, taper_wait_time	DT_TIME	NOT NULL	DEFAULT 400
	, taper_length	DT_LENGTH	NOT NULL	DEFAULT 10
	, taper_speed	DT_SPEED	NOT NULL	DEFAULT 200	-- unit: um/s
	, loss_mode	ELOSSEST_MODE	NOT NULL	DEFAULT 'FINE'
	, loss_limit	DT_LOSS		NOT NULL	DEFAULT 0.20
	, loss_min	DT_LOSS		NOT NULL	DEFAULT 0.01
	, lft_mfd	DT_LENGTH	NOT NULL	DEFAULT 9.3
	, rt_mfd	DT_LENGTH	NOT NULL	DEFAULT 9.3
	, syn_bend_co	DECIMAL(5,2)	NOT NULL	DEFAULT 0.7
	, opp_bend_co	DECIMAL(5,2)	NOT NULL	DEFAULT 0.3
	, mfd_mis_co	DECIMAL(5,2)	NOT NULL	DEFAULT 0
	, lft_push_speed	DT_SPEED	NOT NULL	DEFAULT 100	-- unit: um/s
	, rt_push_speed		DT_SPEED	NOT NULL	DEFAULT 100	-- unit: um/s
);

INSERT INTO fs_param (
	seqn,	name,	fusion_mode,	lfti,	rfti,	align_mode,	x_focus,	y_focus,	ecf_redress,	auto_mag,	tense_test,	vangle_limit,	hangle_limit,	clr_mag,	clr_time,	position,	gap,		overlap,	pre_mag,	pre_time,	arc1_mag,	arc1_time,	arc2_mag,	arc2_time,	arc2_on_time,	arc2_off_time,	arc_man_time,	taper_splice,	taper_wait_time,	taper_length,	taper_speed,	loss_mode,	loss_limit,	loss_min,	lft_mfd,	rt_mfd,	syn_bend_co,	opp_bend_co,	mfd_mis_co,	lft_push_speed,	rt_push_speed
) VALUES
	(1,	'Auto SM',	'AUTO',		'SM',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(2,	'Auto DS',	'AUTO',		'DS',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(3,	'Auto NZ',	'AUTO',		'NZ',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.77,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(4,	'Auto MM',	'AUTO',		'MM',	'FOLLOW',	'CLAD',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.70,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'CLAD',	0.2,	0.00,	50.0,	50.0,	0.3,	0.7,	0,	0.1,	0.1),
	(5,	'Calibrate SM',	'CALIBRATE',	'SM',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(6,	'Calibrate DS',	'CALIBRATE',	'DS',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(7,	'Calibrate NZ',	'CALIBRATE',	'NZ',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.77,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(8,	'Calibrate MM',	'CALIBRATE',	'MM',	'FOLLOW',	'CLAD',	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.70,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'CLAD',	0.2,	0.00,	50.0,	50.0,	0.3,	0.7,	0,	0.1,	0.1),
	(9,	'Normal SM-SM',	'NORMAL',	'SM',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(10,	'Normal DS-DS',	'NORMAL',	'DS',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(11,	'Noraml NZ-NZ',	'NORMAL',	'NZ',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.77,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(12,	'Normal MM-MM',	'NORMAL',	'MM',	'FOLLOW',	'CLAD',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.70,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'CLAD',	0.2,	0.00,	50.0,	50.0,	0.3,	0.7,	0,	0.1,	0.1);



-- 	fs_param lib
CREATE TABLE fs_param_lib () INHERITS (fs_param);

INSERT INTO fs_param_lib (
	seqn,	name,	fusion_mode,	lfti,	rfti,	align_mode,	x_focus,	y_focus,	ecf_redress,	auto_mag,	tense_test,	vangle_limit,	hangle_limit,	clr_mag,	clr_time,	position,	gap,		overlap,	pre_mag,	pre_time,	arc1_mag,	arc1_time,	arc2_mag,	arc2_time,	arc2_on_time,	arc2_off_time,	arc_man_time,	taper_splice,	taper_wait_time,	taper_length,	taper_speed,	loss_mode,	loss_limit,	loss_min,	lft_mfd,	rt_mfd,	syn_bend_co,	opp_bend_co,	mfd_mis_co,	lft_push_speed,	rt_push_speed
) VALUES
	(1,	'Auto SM',	'AUTO',		'SM',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(2,	'Auto DS',	'AUTO',		'DS',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(3,	'Auto NZ',	'AUTO',		'NZ',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.77,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(4,	'Auto MM',	'AUTO',		'MM',	'FOLLOW',	'CLAD',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.70,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'CLAD',	0.2,	0.00,	50.0,	50.0,	0.3,	0.7,	0,	0.1,	0.1),
	(5,	'Calibrate SM',	'CALIBRATE',	'SM',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(6,	'Calibrate DS',	'CALIBRATE',	'DS',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(7,	'Calibrate NZ',	'CALIBRATE',	'NZ',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.77,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(8,	'Calibrate MM',	'CALIBRATE',	'MM',	'FOLLOW',	'CLAD',	FALSE,	FALSE,	TRUE,	TRUE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.70,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'CLAD',	0.2,	0.00,	50.0,	50.0,	0.3,	0.7,	0,	0.1,	0.1),
	(9,	'Normal SM-SM',	'NORMAL',	'SM',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(10,	'Normal DS-DS',	'NORMAL',	'DS',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.82,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(11,	'Noraml NZ-NZ',	'NORMAL',	'NZ',	'FOLLOW',	'FINE',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.77,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'FINE',	0.2,	0.00,	9.3,	9.3,	0.3,	0.7,	0,	0.1,	0.1),
	(12,	'Normal MM-MM',	'NORMAL',	'MM',	'FOLLOW',	'CLAD',	FALSE,	FALSE,	TRUE,	FALSE,	FALSE,	2.0,	1.0,	0.30,	300,	0,	8,	6,	0.30,	150,	0.70,	2200,	0.40,	0,	180,	0,	800,	FALSE,	400,	10,	0.20,	'CLAD',	0.2,	0.00,	50.0,	50.0,	0.3,	0.7,	0,	0.1,	0.1);

CREATE TYPE EHEAT_MATERIAL		AS ENUM ('Standard', 'Micro250', 'Micro400', 'Micro900', 'Connector');
CREATE TYPE EHEAT_LENGTH		AS ENUM ('20mm', '40mm', '60mm');

CREATE TABLE heat_param (
	  seqn		INT2		NOT NULL	PRIMARY KEY
	, name		TEXT		NOT NULL
	, material	EHEAT_MATERIAL	NOT NULL	DEFAULT 'Standard'
	, length	EHEAT_LENGTH	NOT NULL	DEFAULT '60mm'
	, auto_heat	BOOL		NOT NULL	DEFAULT TRUE
	, heat_time	INT2		NOT NULL	DEFAULT 6
	, heat_temp	INT2		NOT NULL	DEFAULT 150
	, finish_temp	INT2		NOT NULL	DEFAULT 140
	, fast_heat	BOOL		NOT NULL	DEFAULT FALSE
	, temp_stay	INT2		NOT NULL	DEFAULT 60
);

INSERT INTO heat_param (
	seqn,	name,		material,	length,		auto_heat,	heat_time,	heat_temp,	finish_temp,	fast_heat,	temp_stay
) VALUES
	(1,	'Standard 60mm',	'Standard',	'60mm',	FALSE,	6,	150,	140,	FALSE,	60),
	(2,	'Standard 40mm',	'Standard',	'40mm',	FALSE,	6,	150,	140,	FALSE,	80),
	(3,	'Micro-250-40mm',	'Micro250',	'40mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(4,	'Micro-250-20mm',	'Micro250',	'20mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(5,	'Micro-400-40mm',	'Micro400',	'40mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(6,	'Micro-400-20mm',	'Micro400',	'20mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(7,	'Micro-900-60mm',	'Micro900',	'60mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(8,	'Micro-900-40mm',	'Micro900',	'40mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(9,	'Micro-900-20mm',	'Micro900',	'20mm',	FALSE,	35,	150,	80,	FALSE,	80);

--	heat_param lib
CREATE TABLE heat_param_lib () INHERITS (heat_param);

INSERT INTO heat_param_lib (
	seqn,	name,		material,	length,		auto_heat,	heat_time,	heat_temp,	finish_temp,	fast_heat,	temp_stay
) VALUES
	(1,	'Standard 60mm',	'Standard',	'60mm',	FALSE,	6,	150,	140,	FALSE,	60),
	(2,	'Standard 40mm',	'Standard',	'40mm',	FALSE,	6,	150,	140,	FALSE,	80),
	(3,	'Micro-250-40mm',	'Micro250',	'40mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(4,	'Micro-250-20mm',	'Micro250',	'20mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(5,	'Micro-400-40mm',	'Micro400',	'40mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(6,	'Micro-400-20mm',	'Micro400',	'20mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(7,	'Micro-900-60mm',	'Micro900',	'60mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(8,	'Micro-900-40mm',	'Micro900',	'40mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(9,	'Micro-900-20mm',	'Micro900',	'20mm',	FALSE,	35,	150,	80,	FALSE,	80),
	(10,	'Connector 40mm',	'Connector',	'40mm',	FALSE,	6,	150,	80,	FALSE,	80),
	(11,	'Connector 20mm',	'Connector',	'20mm',	FALSE,	6,	150,	80,	FALSE,	80);

CREATE TABLE fs_record (
	  sn		SERIAL		NOT NULL	PRIMARY KEY
	, ts		TIMESTAMPTZ	NOT NULL	DEFAULT CURRENT_TIMESTAMP
	, time_consume	INT4		NOT NULL	--unit: ms
	, code		EFS_RESULT	NOT NULL

	, recinfo_lft_ft	EFTI_MODE	NOT NULL
	, recinfo_lft_clad_dm	DT_LENGTH	NOT NULL
	, recinfo_lft_core_dm	DT_LENGTH	NOT NULL

	, recinfo_rt_ft		EFTI_MODE	NOT NULL
	, recinfo_rt_clad_dm	DT_LENGTH	NOT NULL
	, recinfo_rt_core_dm	DT_LENGTH	NOT NULL

	, defect_yzl_dbmp	INT4		NOT NULL
	, defect_yzl_hangle	DT_ANGLE	NOT NULL
	, defect_yzl_vangle	DT_ANGLE	NOT NULL
	, defect_yzl_clad_dm	DT_LENGTH	NOT NULL

	, defect_yzr_dbmp	INT4		NOT NULL
	, defect_yzr_hangle	DT_ANGLE	NOT NULL
	, defect_yzr_vangle	DT_ANGLE	NOT NULL
	, defect_yzr_clad_dm	DT_LENGTH	NOT NULL

	, defect_xzl_dbmp	INT4		NOT NULL
	, defect_xzl_hangle	DT_ANGLE	NOT NULL
	, defect_xzl_vangle	DT_ANGLE	NOT NULL
	, defect_xzl_clad_dm	DT_LENGTH	NOT NULL

	, defect_xzr_dbmp	INT4		NOT NULL
	, defect_xzr_hangle	DT_ANGLE	NOT NULL
	, defect_xzr_vangle	DT_ANGLE	NOT NULL
	, defect_xzr_clad_dm	DT_LENGTH	NOT NULL

	, defect_yz_hangle	DT_ANGLE	NOT NULL
	, defect_xz_hangle	DT_ANGLE	NOT NULL
	, defect_lft_vangle	DT_ANGLE	NOT NULL
	, defect_rt_vangle	DT_ANGLE	NOT NULL

	, defect_yz_img		BYTEA
	, defect_xz_img		BYTEA
	, defect_yz_defect_img	BYTEA
	, defect_xz_defect_img	BYTEA

	, prestate_core_offset	DT_LENGTH	NOT NULL
	, prestate_clad_offset	DT_LENGTH	NOT NULL
	, prestate_endface_gap	DT_LENGTH	NOT NULL
	, prestate_vertex_angle	DT_ANGLE	NOT NULL

	, loss			DT_LOSS		NOT NULL

	, manual_arc_count	INT2		NOT NULL

	, tense_test_exed	BOOL		NOT NULL
	, tense_test_pass	BOOL		NOT NULL

	, xz_final_img		BYTEA
	, yz_final_img		BYTEA
) INHERITS (fs_param);

CREATE TABLE others (
	  name		TEXT	NOT NULL	PRIMARY KEY
	, cfg		JSON	NOT NULL
);

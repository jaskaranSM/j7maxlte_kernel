/*
 *  sound/soc/codecs/rt5509.h
 *  Include header file to Richtek RT5509 SPK Amp driver
 *
 *  Copyright (C) 2015 Richtek Technology Corp.
 *  cy_huang <cy_huang@richtek.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

#ifndef __RT5509_H
#define __RT5509_H
#include <linux/atomic.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/workqueue.h>
#include <mt-plat/rt-regmap.h>

#define RT5509_DEVICE_NAME		"rt5509"
#define RT5509_DRV_VER			"1.0.11_MS"

#ifdef CONFIG_RT_REGMAP
#define RT5509_SIMULATE_DEVICE	0
#if RT5509_SIMULATE_DEVICE
int rt5509_calculate_offset(int reg);
int rt5509_calculate_total_size(void);
#endif /* #if RT5509_SIMULATE_DEVICE */
#else
#define RT5509_SIMULATE_DEVICE	0
#endif /* #ifdef CONFIG_RT_REGMAP */

enum {
	RT5509_CHIP_REVB = 0,
	RT5509_CHIP_REVC = 2,
	RT5509_CHIP_REVD,
};

enum {
	RT5509_CFG_GENERAL,
	RT5509_CFG_BOOSTCONV,
	RT5509_CFG_SPEAKERPROT,
	RT5509_CFG_SAFEGUARD,
	RT5509_CFG_EQ,
	RT5509_CFG_BEW,
	RT5509_CFG_DCR,
	RT5509_CFG_MBDRC,
	RT5509_CFG_ALC,
	RT5509_CFG_MAX,
};

struct rt5509_proprietary_param {
	u8 *cfg[RT5509_CFG_MAX];
	u32 cfg_size[RT5509_CFG_MAX];
};

struct rt5509_pdata {
	struct rt5509_proprietary_param *p_param;
};

struct rt5509_calib_classdev {
	struct device *dev;
	uint32_t n20db;
	uint32_t n15db;
	uint32_t n10db;
	uint32_t gsense_otp;
	uint32_t rspk;
	uint32_t dcr_offset;
	uint32_t rapp;
	int32_t rspkmin;
	int32_t rspkmax;
	int32_t alphaspk;
	/* data logging ++ */
	struct mutex var_lock;
	int32_t tmax_raw;
	uint32_t xmax_raw;
	int32_t t;
	int32_t tmax;
	uint32_t xpeak;
	uint32_t xmax;
	uint32_t xmax_count;
	uint32_t tmax_count;
	uint32_t tmax_overcount;
	/* data logging -- */
	int (*trigger_read)(struct rt5509_calib_classdev *);
	int (*trigger_write)(struct rt5509_calib_classdev *);
	int (*trigger_calculation)(struct rt5509_calib_classdev *);
};

struct rt5509_chip {
	struct i2c_client *i2c;
	struct device *dev;
	struct rt5509_pdata *pdata;
	struct snd_soc_codec *codec;
	struct platform_device *pdev;
	struct rt5509_calib_classdev calib_dev;
	struct rt_regmap_device *rd;
#if RT5509_SIMULATE_DEVICE
	void *sim;
#endif /* #if RT5509_SIMULATE_DEVICE */
	struct delayed_work polling_work;
	struct semaphore io_semaphore;
	atomic_t power_count;
	u8 chip_rev;
	u8 mode_store;
	u8 func_en;
	u8 spk_prot_en;
	u8 alc_gain;
	u8 alc_min_gain;
	u8 classd_gain_store;
	u8 pgain_gain_store;
	u8 sig_gain_store;
	u16 sig_max_store;
	u16 pilot_freq;
	u8 wk_vol;
	u8 wk_step1;
	u8 wk_step2;
	u8 recv_spec_set:1;
	u8 bypass_dsp:1;
	u8 calibrated:1;
	u8 tdm_mode:1;
	u8 rlr_func:1;
	u8 amp_on:1;
	u8 wk_backup:1;
	int dev_cnt;
};

/* RT5509_REGISTER_LIST */
#define RT5509_REG_CHIPREV		0x00
#define RT5509_REG_EVENTINFO		0x01
#define RT5509_REG_DMGFLAG		0x02
#define RT5509_REG_CHIPEN		0x03
#define RT5509_REG_AUDFMT		0x04
#define RT5509_REG_AUDSR		0x05
#define RT5509_REG_I2SSEL		0x06
#define RT5509_REG_I2SDOLRSEL		0x07
#define RT5509_REG_I2SDOSEL		0x08
#define RT5509_REG_FUNCEN		0x09
#define RT5509_REG_CLIP_THR		0x10
#define RT5509_REG_CLIP_CTRL		0x11
#define RT5509_REG_CLIP_SLOPE		0x12
#define RT5509_REG_CLIP_VOMIN		0x13
#define RT5509_REG_CLIP_SIGMAX		0x14
#define RT5509_REG_AMPCONF		0x15
#define RT5509_REG_DACRNKGAIN		0x16
#define RT5509_REG_SAMPOFFS		0x17
#define RT5509_REG_SAMPCONF		0x18
#define RT5509_REG_DAGAIN		0x19
#define RT5509_REG_FFGAIN		0x1A
#define RT5509_REG_VBATGAIN		0x1B
#define RT5509_REG_RLDCOEF1		0x1C
#define RT5509_REG_RLDCOEF2		0x1D
#define RT5509_REG_BST_MODE		0x1E
#define RT5509_REG_BST_TH1		0x1F
#define RT5509_REG_BST_TH2		0x20
#define RT5509_REG_BST_TH3		0x21
#define RT5509_REG_BST_CONF1		0x22
#define RT5509_REG_BST_SIG_GAIN		0x23
#define RT5509_REG_BST_CONF2		0x24
#define RT5509_REG_BST_CONF3		0x25
#define RT5509_REG_OCPOTPEN		0x26
#define RT5509_REG_IDAC1TST		0x27
#define RT5509_REG_IDAC2TST		0x28
#define RT5509_REG_IDAC3TST		0x29
#define RT5509_REG_IDACTSTEN		0x2A
#define RT5509_REG_CCMAX		0x2B
#define RT5509_REG_OCPMAX		0x2C
#define RT5509_REG_INTERRUPT		0x2D
#define RT5509_REG_INTRMASK		0x2E
#define RT5509_REG_DEGLITCH		0x2F
#define RT5509_REG_SICRTNSTHACT		0x30
#define RT5509_REG_TIMEDET		0x31
#define RT5509_REG_TDELAY		0x32
#define RT5509_REG_TATKSEL		0x33
#define RT5509_REG_TREL			0x34
#define RT5509_REG_THOLDREL		0x35
#define RT5509_REG_STHLMT		0x38
#define RT5509_REG_XTHLMT		0x39
#define RT5509_REG_STHALC		0x3A
#define RT5509_REG_XTHALC		0x3B
#define RT5509_REG_INITUDT		0x3C
#define RT5509_REG_UDT			0x3D
#define RT5509_REG_DNHALFT		0x3E
#define RT5509_REG_ALCGAIN		0x3F
#define RT5509_REG_ADAPTCONF		0x40
#define RT5509_REG_INITIMPLDMU		0x41
#define RT5509_REG_IMPLDMU		0x42
#define RT5509_REG_GPILOT		0x43
#define RT5509_REG_PILOTEN		0x44
#define RT5509_REG_PILOTNISENSE		0x45
#define RT5509_REG_ISENSEGAIN		0x46
#define RT5509_REG_RAPP			0x47
#define RT5509_REG_DCR_MAX		0x48
#define RT5509_REG_DCR_KD		0x49
#define RT5509_REG_DCR_KP		0x4A
#define RT5509_REG_DCR_KI		0x4B
#define RT5509_REG_INITDCRIDMU		0x4C
#define RT5509_REG_DCRIDMU		0x4D
#define RT5509_REG_CALIB_DCR		0x4E
#define RT5509_REG_CALIB_BL		0x4F
#define RT5509_REG_CALIB_CTRL		0x50
#define RT5509_REG_CALIB_REQ		0x51
#define RT5509_REG_CALIB_GAIN		0x52
#define RT5509_REG_CALIB_OUT0		0x53
#define RT5509_REG_CALIB_OUT1		0x54
#define RT5509_REG_XTHLMTDAM		0x55
#define RT5509_REG_RMAXDAM		0x56
#define RT5509_REG_TSCALEDAM		0x57
#define RT5509_REG_RECOVERT		0x58
#define RT5509_REG_SETRESFREQ		0x59
#define RT5509_REG_GETRESFREQ		0x5A
#define RT5509_REG_VOLCTL		0x5B
#define RT5509_REG_VOLUME		0x5C
#define RT5509_REG_CALIB_OUTX		0x5D
#define RT5509_REG_CALIB_OUTY		0x5E
#define RT5509_REG_BQ1			0x60
#define RT5509_REG_BQ2			0x61
#define RT5509_REG_BQ3			0x62
#define RT5509_REG_BQ4			0x63
#define RT5509_REG_BQ5			0x64
#define RT5509_REG_BQ6			0x65
#define RT5509_REG_BQ7			0x66
#define RT5509_REG_BQ8			0x67
#define RT5509_REG_BQ9			0x68
#define RT5509_REG_BQ10			0x69
#define RT5509_REG_VBBQ1		0x6A
#define RT5509_REG_VBBQ2		0x6B
#define RT5509_REG_VBBQ3		0x6C
#define RT5509_REG_VBBQ4		0x6D
#define RT5509_REG_VBBQ5		0x6E
#define RT5509_REG_VBBQ6		0x6F
#define RT5509_REG_VBBQ7		0x70
#define RT5509_REG_VBBQ8		0x71
#define RT5509_REG_VBBQ9		0x72
#define RT5509_REG_VBFCN		0x73
#define RT5509_REG_VBGAIN1		0x74
#define RT5509_REG_VBGAIN2		0x75
#define RT5509_REG_VBGAIN3		0x76
#define RT5509_REG_VBGAIN4		0x77
#define RT5509_REG_VBGAIN5		0x78
#define RT5509_REG_VBGAIN6		0x79
#define RT5509_REG_VBGAIN7		0x7A
#define RT5509_REG_VBGAIN8		0x7B
#define RT5509_REG_VBGAIN9		0x7C
#define RT5509_REG_VBGAIN10		0x7D
#define RT5509_REG_SLOPCONST		0x7E
#define RT5509_REG_BWCOEFF		0x7F
#define RT5509_REG_SWRESET		0x80
#define RT5509_REG_SPKGAIN		0x81
#define RT5509_REG_DSPKCONF1		0x82
#define RT5509_REG_DSPKCONF2		0x83
#define RT5509_REG_DSPKCONF3		0x84
#define RT5509_REG_DSPKCONF4		0x85
#define RT5509_REG_DSPKVMID		0x86
#define RT5509_REG_DSPKZCBOOST		0x87
#define RT5509_REG_ISENSE_CTRL		0x88
#define RT5509_REG_DIMADC		0x89
#define RT5509_REG_DSPKEN1		0x8A
#define RT5509_REG_VBATDATA		0x8B
#define RT5509_REG_VTHRMDATA		0x8C
#define RT5509_REG_VBATSENSE		0x8D
#define RT5509_REG_IDACTSTNINFO		0x8E
#define RT5509_REG_IDACBOOST		0x8F
#define RT5509_REG_DSPKEN2		0x90
#define RT5509_REG_DSPKIBCONF1		0x91
#define RT5509_REG_DSPKIBCONF2		0x92
#define RT5509_REG_DSPKIBCONF3		0x93
#define RT5509_REG_DSPKCONF5		0x94
#define RT5509_REG_DSPKCONF6		0x95
#define RT5509_REG_OVPUVPCTRL		0x96
#define RT5509_REG_PLLCONF1		0x97
#define RT5509_REG_PLLCONF2		0x98
#define RT5509_REG_PLLCONF3		0x99
#define RT5509_REG_PLLCONF4		0x9A
#define RT5509_REG_PLLINFO		0x9B
#define RT5509_REG_PLLDIVISOR		0x9C
#define RT5509_REG_ZCCONF		0x9D
#define RT5509_REG_DCADJ		0x9E
#define RT5509_REG_I2CBCKLRCKCONF	0x9F
#define RT5509_REG_TDEN			0xA0
#define RT5509_REG_ALPHACONF		0xA1
#define RT5509_REG_SPKRPTSEL		0xA2
#define RT5509_REG_SPKRPT		0xA3
#define RT5509_REG_NDELAY		0xA4
#define RT5509_REG_DELAYRES		0xA5
#define RT5509_REG_PHI1			0xA6
#define RT5509_REG_PHI2			0xA7
#define RT5509_REG_PHI3			0xA8
#define RT5509_REG_PHI4			0xA9
#define RT5509_REG_PHI5			0xAA
#define RT5509_REG_ADAPTB0		0xAB
#define RT5509_REG_ADAPTB1		0xAC
#define RT5509_REG_ADAPTB2		0xAD
#define RT5509_REG_ADAPTB3		0xAE
#define RT5509_REG_ADAPTB4		0xAF
#define RT5509_REG_ADAPTB5		0xB0
#define RT5509_REG_COEFSIERA		0xB1
#define RT5509_REG_COEFHPF		0xB2
#define RT5509_REG_MIMATC_CTRL		0xB3
#define RT5509_REG_TDM_CTRL		0xB4
#define RT5509_REG_ECO_CTRL		0xB5
#define RT5509_REG_BSTTM		0xB8
#define RT5509_REG_ALCMINGAIN		0xB9
#define RT5509_REG_RESVECO0		0xBA
#define RT5509_REG_OTPCONF		0xC0
#define RT5509_REG_OTPADDR		0xC1
#define RT5509_REG_OTPDIN		0xC2
#define RT5509_REG_VBG_TRIM		0xC3
#define RT5509_REG_VTEMP_TRIM		0xC4
#define RT5509_REG_TCOEFF		0xC5
#define RT5509_REG_SPSCONF		0xC6
#define RT5509_REG_SPSTHR		0xC7
#define RT5509_REG_VTHERMBATEN		0xC8
#define RT5509_REG_DBGADS		0xC9
#define RT5509_REG_TESTDAC		0xCA
#define RT5509_REG_SPKDCS		0xCB
#define RT5509_REG_MSKFLAG		0xCC
#define RT5509_REG_DRCMINGAIN		0xCF
#define RT5509_REG_DRC_SEL		0xD0
#define RT5509_REG_DRC_ATTACK		0xD1
#define RT5509_REG_DRC_PARAM		0xD2
#define RT5509_REG_DRCBQ1		0xD3	/* Limiter1 */
#define RT5509_REG_DRCBQ2		0xD4	/* Limiter2 */
#define RT5509_REG_DRCBQ3		0xD5	/* High1 */
#define RT5509_REG_DRCBQ4		0xD6	/* High2 */
#define RT5509_REG_DRCBQ5		0xD7	/* Mid1 */
#define RT5509_REG_DRCBQ6		0xD8	/* Mid2 */
#define RT5509_REG_DRCBQ7		0xD9	/* Mid3 */
#define RT5509_REG_DRCBQ8		0xDA	/* Mid4 */
#define RT5509_REG_DRCBQ9		0xDB	/* Low1 */
#define RT5509_REG_DRCBQ10		0xDC	/* Low2 */
#define RT5509_REG_DRCBQ11		0xDD	/* Sub1 */
#define RT5509_REG_DRCBQ12		0xDE	/* Sub2 */
#define RT5509_REG_DRCEN		0xDF
#define RT5509_REG_MTPFLOW1		0xE1
#define RT5509_REG_MTPFLOW2		0xE2
#define RT5509_REG_MTPFLOW3		0xE3
#define RT5509_REG_MTPFLOW4		0xE4
#define RT5509_REG_MTPFLOW5		0xE5
#define RT5509_REG_MTPFLOW6		0xE6
#define RT5509_REG_MTPFLOW7		0xE7
#define RT5509_REG_MTPFLOW8		0xE8
#define RT5509_REG_MTPFLOW9		0xE9
#define RT5509_REG_MTPFLOWA		0xEA
#define RT5509_REG_MTPFLOWB		0xEB
#define RT5509_REG_MTPFLOWC		0xEC
#define RT5509_REG_MTPFLOWD		0xED
#define RT5509_REG_MTPFLOWE		0xEE
#define RT5509_REG_MTPFLOWF		0xEF
#define RT5509_REG_TESTMODE1		0xF0
#define RT5509_REG_RAMIND1		0xF1
#define RT5509_REG_RAMIND2		0xF2
#define RT5509_REG_SCANMODE		0xF3
#define RT5509_REG_CLKEN1		0xF4
#define RT5509_REG_CLKEN2		0xF5
#define RT5509_REG_PADDRV		0xF6
#define RT5509_REG_TESTMODE2		0xF7
#define RT5509_REG_SLEWRATE1		0xF8
#define RT5509_REG_SLEWRATE2		0xF9
#define RT5509_REG_BIASRESISTOR		0xFA
#define RT5509_REG_SPKDRV		0xFB
#define RT5509_REG_BLOCKREF1		0xFC
#define RT5509_REG_BLOCKREF2		0xFD
#define RT5509_REG_BIASCURRENT		0xFE
#define RT5509_REG_BIASOPTION		0xFF
#define RT5509_MAX_REGS			0x100

/* RT5509_REG_CHIPREV: 0x00 */
#define RT5509_CHIPID_MASK	0xF0
#define RT5509_CHIP_ID		0xE0
#define RT5509_CHIPREV_MASK	0x0F
#define RT5509_CHIPREV_SHFT	0

/* RT5509_REG_EVENTINFO: 0x01 */
#define RT5509_BCKLOCK_STATSHFT	7
#define RT5509_PLLLOCK_STATSHFT	6
#define RT5509_BATUV_STATSHFT	5
#define RT5509_AMPOV_STATSHFT	4
#define RT5509_AMPOC_STATSHFT	3
#define RT5509_AMPOT_STATSHFT	2
#define RT5509_BSTOC_STATSHFT	1
#define RT5509_BSTOT_STATSHFT	0

/* RT5509_REG_CHIPEN: 0x03 */
#define RT5509_TRIWAVE_ENMASK	0x20
#define RT5509_SPKAMP_ENMASK	0x08
#define RT5509_SPKAMP_ENSHFT	3
#define RT5509_SPKMUTE_ENMASK	0x04
#define RT5509_SPKPROT_ENMASK	0x02
#define RT5509_SPKPROT_ENSHFT	1
#define RT5509_CHIPPD_ENMASK	0x01

/* RT5509_REG_AUDFMT: 0x04 */
enum {
	RT5509_DSP_MODEA = 0,
	RT5509_DSP_MODEB,
};
#define RT5509_DSPMODE_MASK	0x10
#define RT5509_DSPMODE_SHFT	4
enum {
	RT5509_AUDFMT_I2S = 0,
	RT5509_AUDFMT_LEFTJ,
	RT5509_AUDFMT_RIGHTJ,
};
#define RT5509_AUDFMT_MASK	0x0C
#define RT5509_AUDFMT_SHFT	2
enum {
	RT5509_AUDBIT_24 = 0,
	RT5509_AUDBIT_20,
	RT5509_AUDBIT_18,
	RT5509_AUDBIT_16,
};
#define RT5509_AUDBIT_MASK	0x03
#define RT5509_AUDBIT_SHFT	0

/* RT5509_REG_AUDSR: 0x05 */
enum {
	RT5509_BCKMODE_32FS = 0,
	RT5509_BCKMODE_48FS,
	RT5509_BCKMODE_64FS,
};
#define RT5509_BCKMODE_MASK	0x18
#define RT5509_BCKMODE_SHFT	3
enum {
	RT5509_SRMODE_8K = 0,
	RT5509_SRMODE_12K,
	RT5509_SRMODE_16K,
	RT5509_SRMODE_24K,
	RT5509_SRMODE_32K,
	RT5509_SRMODE_48K,
	RT5509_SRMODE_96K,
	RT5509_SRMODE_192K,
};
#define RT5509_SRMODE_MASK	0x07
#define RT5509_SRMODE_SHFT	0

/* RT5509_REG_I2SSEL: 0x06 */
#define RT5509_I2SLRSEL_SHFT	2
#define RT5509_I2SSEL_SHFT	0

/* RT5509_REG_FUNCEN: 0x09 */
#define RT5509_LMTEN_SHFT	5
#define RT5509_ALCEN_SHFT	4
#define RT5509_MBDRCEN_SHFT	3
#define RT5509_BEWEN_SHFT	2
#define RT5509_HPFEN_SHFT	1
#define RT5509_BQEN_SHFT	0

/* RT5509_REG_CLIP_CTRL : 0x11 */
#define RT5509_CLIPEN_SHFT	7

/* RT5509_REG_BST_MODE: 0x1E */
#define RT5509_BSTMODE_SHFT	0
#define RT5509_BSTMODE_MASK	0x03

/* RT5509_REG_SWRESET: 0x80 */
#define RT5509_SWRST_MASK	0x80

/* RT5509_REG_SPKGAIN: 0x81 */
#define RT5509_BSTGAIN_SHFT	5
#define RT5509_POSTPGAGAIN_SHFT	0

/* RT5509_REG_DSPKCONF1: 0x82 */
#define RT5509_PREPGAGAIN_SHFT	0

/* RT5509_REG_DSPKVMID: 0x86 */
#define RT5509_VMID_ENMASK	0x80

/* RT5509_REG_DSPKEN1: 0x8A */
#define RT5509_BUF_ENMASK	0x80
#define RT5509_BIAS_ENMASK	0x40

/* RT5509_REG_DSPKCONF5: 0x94 */
#define RT5509_VBG_ENMASK	0x40

/* RT5509_REG_TDM_CTRL: 0xB4 */
#define RT5509_TDM_ENMASK	0x08

/* RT5509_REG_CLKEN1: 0xF4 */
#define RT5509_CLKEN1_MASK	0xFF

/* RT5509_REG_CLKEN2: 0xF5 */
#define RT5509_CLKEN2_MASK	0x03

struct rt_regmap_device *rt5509_regmap_register(
	struct rt_regmap_fops *regmap_ops,
	struct device *parent, void *client, void *drvdata);
int rt5509_calib_create(struct rt5509_chip *chip);
void rt5509_calib_destroy(struct rt5509_chip *chip);

#endif /* #ifndef __RT5509_H */

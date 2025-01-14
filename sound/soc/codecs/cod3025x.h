/*
 * Copyright (c) 2014 Samsung Electronics Co. Ltd.
 *
 * Author: Sayanta Pattanayak <sayanta.p@samsung.com>
 *				<sayantapattanayak@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _COD3025X_H
#define _COD3025X_H

#include <linux/completion.h>

#include <sound/soc.h>
#include <linux/switch.h>
#include <linux/iio/consumer.h>

extern const struct regmap_config cod3025x_regmap;
extern int cod3025x_jack_mic_register(struct snd_soc_codec *codec);

#define COD3025X_OTP_MAX_REG		0x0f
#define COD3025X_MAX_REGISTER		0xde
#define COD3025X_OTP_REG_WRITE_START	0xd0

#define COD3025X_REGCACHE_SYNC_START_REG	0x0
#define COD3025X_REGCACHE_SYNC_END_REG	0x80

#define COD3025X_BTN_RELEASED_MASK	BIT(0)
#define COD3025X_BTN_PRESSED_MASK	BIT(1)

struct cod3025x_jack_det {
	bool jack_det;
	bool mic_det;
	bool button_det;
	unsigned int button_code;
	int button_state;
};

struct jack_buttons_zone {
	unsigned int code;
	unsigned int adc_low;
	unsigned int adc_high;
};

struct cod3025x_priv {
	struct regmap *regmap;
	struct snd_soc_codec *codec;
	struct device *dev;

	struct regulator *vdd;

	int sysclk;
	int asyncclk;

	int num_inputs;
	int int_gpio;

	struct pinctrl *pinctrl;
	unsigned int spk_ena:2;

	bool is_suspend;
	bool is_probe_done;
	struct cod3025x_jack_det jack_det;
	struct mutex jackdet_lock;
	struct switch_dev sdev;
	struct completion initialize_complete;

	struct input_dev *input;
	unsigned int key_code[10];
	unsigned int key_pressed_count;
	struct delayed_work key_work;
	struct mutex key_lock;
	struct timer_list timer;
	unsigned short i2c_addr;
	unsigned char otp_reg[COD3025X_OTP_MAX_REG];
	unsigned int mic_bias1_voltage;
	unsigned int mic_bias2_voltage;
	unsigned int mic_bias_ldo_voltage;
	unsigned int aifrate;
	bool update_fw;
	bool use_external_jd;
	int vol_hpl;
	int vol_hpr;
	struct jack_buttons_zone jack_buttons_zones[4];
	struct work_struct buttons_work;
	struct workqueue_struct *buttons_wq;
	struct iio_channel *jack_adc;
	unsigned int use_btn_adc_mode;
};

/*
 * Helper macros for creating bitmasks
 */
#define MASK(width, shift)	(((0x1 << (width)) - 1) << shift)

/*
 * Register values.
*/
#define COD3025X_00_BASE_REG			0x00
#define COD3025X_01_IRQ1PEND			0x01
#define COD3025X_02_IRQ2PEND			0x02
#define COD3025X_03_IRQ3PEND			0x03
#define COD3025X_04_IRQ4PEND			0x04
#define COD3025X_05_IRQ1M			0x05
#define COD3025X_06_IRQ2M			0x06
#define COD3025X_07_IRQ3M			0x07
#define COD3025X_08_IRQ4M			0x08
#define COD3025X_09_STATUS1			0x09
#define COD3025X_0A_STATUS2			0x0a
#define COD3025X_0B_STATUS3			0x0b

#define COD3025X_10_PD_REF			0x10
#define COD3025X_11_PD_AD1			0x11
#define COD3025X_12_PD_AD2			0x12
#define COD3025X_13_PD_DA1			0x13
#define COD3025X_14_PD_DA2			0x14
#define COD3025X_15_PD_DA3			0x15
#define COD3025X_16_PWAUTO_AD			0x16
#define COD3025X_17_PWAUTO_DA			0x17
#define COD3025X_18_CTRL_REF			0x18
#define COD3025X_19_ZCD_AD			0x19
#define COD3025X_1A_ZCD_DA			0x1a
#define COD3025X_1B_ZCD				0x1b
#define COD3025X_1C_SV_DA			0x1c

#define COD3025X_20_VOL_AD1			0x20
#define COD3025X_21_VOL_AD2			0x21
#define COD3025X_22_VOL_AD3			0x22
#define COD3025X_23_MIX_AD			0x23
#define COD3025X_24_DSM_ADS			0x24

#define COD3025X_30_VOL_HPL			0x30
#define COD3025X_31_VOL_HPR			0x31
#define COD3025X_32_VOL_EP_SPK			0x32
#define COD3025X_33_CTRL_EP			0x33
#define COD3025X_34_CTRL_HP			0x34
#define COD3025X_35_CTRL_SPK			0x35
#define COD3025X_36_MIX_DA1			0x36
#define COD3025X_37_MIX_DA2			0x37
#define COD3025X_38_DCT_CLK1			0x38

#define COD3025X_40_DIGITAL_POWER		0x40
#define COD3025X_41_FORMAT			0x41
#define COD3025X_42_ADC1			0x42
#define COD3025X_43_ADC_L_VOL			0x43
#define COD3025X_44_ADC_R_VOL			0x44

#define COD3025X_50_DAC1			0x50
#define COD3025X_51_DAC_L_VOL			0x51
#define COD3025X_52_DAC_R_VOL			0x52
#define COD3025X_53_MQS				0x53
#define COD3025X_54_DNC1			0x54
#define COD3025X_55_DNC2			0x55
#define COD3025X_56_DNC3			0x56
#define COD3025X_57_DNC4			0x57
#define COD3025X_58_DNC5			0x58
#define COD3025X_59_DNC6			0x59
#define COD3025X_5A_DNC7			0x5a
#define COD3025X_5B_DNC8			0x5b
#define COD3025X_5C_DNC9			0x5c
#define COD3025X_5D_SPKLIMIT1			0x5d
#define COD3025X_5C_SPKLIMIT2			0x5e
#define COD3025X_5F_SPKLIMIT3			0x5f

#define COD3025X_60_OFFSET1			0x60
#define COD3025X_61_RESERVED			0x61
#define COD3025X_62_IRQ_R			0x62

#define COD3025X_70_CLK1_AD			0x70
#define COD3025X_71_CLK1_DA			0x71
#define COD3025X_72_DSM1_DA			0x72
#define COD3025X_73_DSM2_DA			0x73
#define COD3025X_74_TEST			0x74
#define COD3025X_75_CHOP_AD			0x75
#define COD3025X_76_CHOP_DA			0x76
#define COD3025X_77_CTRL_CP			0x77
#define COD3025X_78_DSM_AD			0x78
#define COD3025X_79_SL_DA1			0x79
#define COD3025X_7A_SL_DA2			0x7a

#define COD3025X_80_DET_PDB			0x80
#define COD3025X_81_DET_ON			0x81
#define COD3025X_82_MIC_BIAS			0x82
#define COD3025X_83_JACK_DET1			0x83
#define COD3025X_84_JACK_DET2			0x84
#define COD3025X_85_MIC_DET			0x85
#define COD3025X_86_DET_TIME			0x86
#define COD3025X_87_LDO_DIG			0x87
#define COD3025X_88_KEY_TIME			0x88

/* OTP registers */
#define COD3025X_D0_CTRL_IREF1			0xd0
#define COD3025X_D1_CTRL_IREF2			0xd1
#define COD3025X_D2_CTRL_IREF3			0xd2
#define COD3025X_D3_CTRL_IREF4			0xd3
#define COD3025X_D4_OFFSET_DAL			0xd4
#define COD3025X_D5_OFFSET_DAR			0xd5
#define COD3025X_D6_CTRL_IREF5			0xd6
#define COD3025X_D7_CTRL_CP1			0xd7
#define COD3025X_D8_CTRL_CP2			0xd8
#define COD3025X_D9_CTRL_CP3			0xd9
#define COD3025X_DA_CTRL_CP4			0xda
#define COD3025X_DB_CTRL_HPS			0xdb
#define COD3025X_DC_CTRL_EPS			0xdc
#define COD3025X_DD_CTRL_SPKS1			0xdd
#define COD3025X_DE_CTRL_SPKS2			0xde

/* COD3025X_01_IRQ1PEND */
#define IRQ1_HOOK_DET				BIT(7)
#define IRQ1_VOL_UP_DET				BIT(6)
#define IRQ1_VOL_DN_DET				BIT(5)
#define IRQ1_KEY_L_RELEASE			BIT(4)
#define IRQ1_KP_DET_L				BIT(3)
#define IRQ1_KP_DET_S				BIT(2)
#define IRQ1_MIC_DET_R				BIT(1)
#define IRQ1_JACK_DET_R				BIT(0)

#define HOOK_PUSH_SHORT		(IRQ1_HOOK_DET | IRQ1_KP_DET_S)
#define HOOK_PUSH_LONG		(IRQ1_HOOK_DET | IRQ1_KP_DET_L)
#define HOOK_RELEASE		(IRQ1_HOOK_DET | IRQ1_KEY_L_RELEASE)
#define VOL_UP_PUSH_SHORT	(IRQ1_VOL_UP_DET | IRQ1_KP_DET_S)
#define VOL_UP_PUSH_LONG	(IRQ1_VOL_UP_DET | IRQ1_KP_DET_L)
#define VOL_UP_RELEASE		(IRQ1_VOL_UP_DET | IRQ1_KEY_L_RELEASE)
#define VOL_DOWN_PUSH_SHORT	(IRQ1_VOL_DN_DET | IRQ1_KP_DET_S)
#define VOL_DOWN_PUSH_LONG	(IRQ1_VOL_DN_DET | IRQ1_KP_DET_L)
#define VOL_DOWN_RELEASE	(IRQ1_VOL_DN_DET | IRQ1_KEY_L_RELEASE)

/* COD3025X_02_IRQ2PEND */
#define IRQ2_HOOK_DET_R				BIT(7)
#define IRQ2_HOOK_DET_F				BIT(6)
#define IRQ2_VOL_UP_DET_R			BIT(5)
#define IRQ2_VOL_UP_DET_F			BIT(4)
#define IRQ2_VOL_DN_DET_R			BIT(3)
#define IRQ2_VOL_DN_DET_F			BIT(2)
#define IRQ2_MIC_DET_F				BIT(1)
#define IRQ2_JACK_DET_F				BIT(0)

/* COD3025X_03_IRQ3PEND */
#define IRQ3_FLG_PW_MIC3_R_SHIFT		7
#define IRQ3_FLG_PW_MIC1_R_SHIFT		6
#define IRQ3_FLG_PW_MIC2_R_SHIFT		5
#define IRQ3_FLG_PW_MTVOL_CODEC_R_SHIFT		4
#define IRQ3_FLG_PW_HP_R_SHIFT			2
#define IRQ3_FLG_PW_EP_R_SHIFT			1
#define IRQ3_FLG_PW_SPK_R_SHIFT			0

/* COD3025X_04_IRQ4PEND */
#define IRQ4_FLG_PW_MIC3_F_SHIFT		7
#define IRQ4_FLG_PW_MIC1_F_SHIFT		6
#define IRQ4_FLG_PW_MIC2_F_SHIFT		5
#define IRQ4_FLG_PW_MTVOL_CODEC_F_SHIFT		4
#define IRQ4_FLG_PW_HP_F_SHIFT			2
#define IRQ4_FLG_PW_EP_F_SHIFT			1
#define IRQ4_FLG_PW_SPK_F_SHIFT			0

/* COD3025X_05_IRQ1M */
#define IRQ1M_HOOK_DET_M_SHIFT                  7
#define IRQ1M_HOOK_DET_M_MASK                   BIT(IRQ1M_HOOK_DET_M_SHIFT)

#define IRQ1M_VOLUP_DET_M_SHIFT                 6
#define IRQ1M_VOLUP_DET_M_MASK                  BIT(IRQ1M_VOLUP_DET_M_SHIFT)

#define IRQ1M_VOLDN_DET_M_SHIFT                 5
#define IRQ1M_VOLDN_DET_M_MASK                  BIT(IRQ1M_VOLDN_DET_M_SHIFT)

#define IRQ1M_KPL_DET_M_SHIFT                   4
#define IRQ1M_KPL_DET_M_MASK                    BIT(IRQ1M_KPL_DET_M_SHIFT)

#define IRQ1M_KP_DET_M_SHIFT                    3
#define IRQ1M_KP_DET_M_MASK                     BIT(IRQ1M_KP_DET_M_SHIFT)

#define IRQ1M_MIC_DET_R_M_SHIFT                 1
#define IRQ1M_MIC_DET_R_M_MASK                  BIT(IRQ1M_MIC_DET_R_M_SHIFT)

#define IRQ1M_JACK_DET_R_M_SHIFT                0
#define IRQ1M_JACK_DET_R_M_MASK                 BIT(IRQ1M_JACK_DET_R_M_SHIFT)

#define IRQ1M_MASK_ALL				0xFF

/* COD3025X_06_IRQ2M */
#define IRQ2M_HOOK_DET_R_M_SHIFT                7
#define IRQ2M_HOOK_DET_R_M_MASK                 BIT(IRQ2M_HOOK_DET_R_M_SHIFT)

#define IRQ2M_HOOK_DET_F_M_SHIFT                6
#define IRQ2M_HOOK_DET_F_M_MASK                 BIT(IRQ2M_HOOK_DET_F_M_SHIFT)

#define IRQ2M_VOLP_DET_R_M_SHIFT                5
#define IRQ2M_VOLP_DET_R_M_MASK                 BIT(IRQ2M_VOLP_DET_R_M_SHIFT)

#define IRQ2M_VOLP_DET_F_M_SHIFT                4
#define IRQ2M_VOLP_DET_F_M_MASK                 BIT(IRQ2M_VOLP_DET_F_M_SHIFT)

#define IRQ2M_VOLN_DET_R_M_SHIFT                3
#define IRQ2M_VOLN_DET_R_M_MASK                 BIT(IRQ2M_VOLP_DET_R_M_SHIFT)

#define IRQ2M_VOLN_DET_F_M_SHIFT                4
#define IRQ2M_VOLN_DET_F_M_MASK                 BIT(IRQ2M_VOLP_DET_F_M_SHIFT)

#define IRQ2M_MIC_DET_F_M_SHIFT                 1
#define IRQ2M_MIC_DET_F_M_MASK                  BIT(IRQ2M_MIC_DET_F_M_SHIFT)

#define IRQ2M_JACK_DET_F_M_SHIFT                0
#define IRQ2M_JACK_DET_F_M_MASK                 BIT(IRQ2M_JACK_DET_F_M_SHIFT)

#define IRQ2M_MASK_ALL				0xFF

/* COD3025X_07_IRQ3M */
#define IRQ3M_FLG_PW_MIC3_R_M_SHIFT		7
#define IRQ3M_FLG_PW_MIC1_R_M_SHIFT		6
#define IRQ3M_FLG_PW_MIC2_R_M_SHIFT		5
#define IRQ3M_FLG_PW_MTVOL_CODEC_R_M_SHIFT	4
#define IRQ3M_FLG_PW_HP_R_M_SHIFT		2
#define IRQ3M_FLG_PW_EP_R_M_SHIFT		1
#define IRQ3M_FLG_PW_SPK_R_M_SHIFT		0

/* COD3025X_08_IRQ8 */
#define IRQ4M_FLG_PW_MIC3_F_M_SHIFT		7
#define IRQ4M_FLG_PW_MIC1_F_M_SHIFT		6
#define IRQ4M_FLG_PW_MIC2_F_M_SHIFT		5
#define IRQ4M_FLG_PW_MTVOL_CODEC_F_M_SHIFT	4
#define IRQ4M_FLG_PW_HP_F_M_SHIFT		2
#define IRQ4M_FLG_PW_EP_F_M_SHIFT		1
#define IRQ4_FLG_PW_SPK_F_M_SHIFT		0

/* COD3025X_09_STATUS1 */
#define STATUS1_VOLN_DET_SHIFT			4
#define STATUS1_VOLP_DET_SHIFT			3
#define STATUS1_HOOK_DET_SHIFT			2
#define STATUS1_MIC_DET_SHIFT			1
#define STATUS1_JACK_DET_SHIFT			0

/* COD3025X_0A_STATUS2 */
#define STATUS2_BTN_IMP_SHIFT		2
#define STATUS2_BTN_IMP_WIDTH		3
#define STATUS2_BTN_IMP_MASK		MASK(STATUS2_BTN_IMP_WIDTH, \
					STATUS2_BTN_IMP_SHIFT)

#define STATUS2_MIC_IMP_SHIFT		0
#define STATUS2_MIC_IMP_WIDTH		2
#define STATUS2_MIC_IMP_MASK		MASK(STATUS2_MIC_IMP_WIDTH, \
					STATUS2_MIC_IMP_SHIFT)

/* COD3025X_0B_STATUS3 */
#define STATUS3_FLG_PW_MIC3_SHIFT		7
#define STATUS3_FLG_PW_MIC1_SHIFT		6
#define STATUS3_FLG_PW_MIC2_SHIFT		5
#define STATUS3_FLG_PW_MTVOL_CODEC_SHIFT	4
#define STATUS3_FLG_PW_HP_SHIFT			2
#define STATUS3_FLG_PW_EP_SHIFT			1
#define STATUS3_FLG_PW_SPK_SHIFT		0

/* COD3025X_10_PD_REF */
#define PDB_VMID_SHIFT				5
#define PDB_VMID_MASK				BIT(PDB_VMID_SHIFT)

#define PDB_IGEN_SHIFT				4
#define PDB_IGEN_MASK				BIT(PDB_IGEN_SHIFT)

#define PDB_IGEN_AD_SHIFT			3
#define PDB_IGEN_AD_MASK			BIT(PDB_IGEN_AD_SHIFT)

#define PDB_MCB1_SHIFT				2
#define PDB_MCB1_MASK				BIT(PDB_MCB1_SHIFT)

#define PDB_MCB2_SHIFT				1
#define PDB_MCB2_MASK				BIT(PDB_MCB2_SHIFT)

#define PDB_MCB_LDO_CODEC_SHIFT			0
#define PDB_MCB_LDO_CODEC_MASK			BIT(PDB_MCB_LDO_CODEC_SHIFT)

/* COD3025X_11_PD_AD1 */
#define RESETB_DSMR_SHIFT	0
#define RESETB_DSMR_MASK	BIT(RESETB_DSMR_SHIFT)

#define RESETB_DSML_SHIFT	1
#define RESETB_DSML_MASK	BIT(RESETB_DSML_SHIFT)

#define PDB_DSMR_SHIFT		2
#define PDB_DSMR_MASK		BIT(PDB_DSMR_SHIFT)

#define PDB_DSML_SHIFT		3
#define PDB_DSML_MASK		BIT(PDB_DSML_SHIFT)

#define PDB_RESETB_DSML_DSMR_MASK \
				(RESETB_DSMR_MASK | RESETB_DSML_MASK | \
				PDB_DSMR_MASK | PDB_DSML_MASK)

#define PDB_RESETB_DSML_DSMR \
				(RESETB_DSMR_MASK | RESETB_DSML_MASK | \
				PDB_DSMR_MASK | PDB_DSML_MASK)

/* COD3025X_12_PD_AD2 */
#define PDB_MIC3_SHIFT		4
#define PDB_MIC3_MASK		BIT(PDB_MIC3_SHIFT)

#define PDB_MIC1_SHIFT		3
#define PDB_MIC1_MASK		BIT(PDB_MIC1_SHIFT)

#define PDB_MIC2_SHIFT		2
#define PDB_MIC2_MASK		BIT(PDB_MIC2_SHIFT)

#define PDB_MIXL_SHIFT		1
#define PDB_MIXL_MASK		BIT(PDB_MIXL_SHIFT)

#define PDB_MIXR_SHIFT		0
#define PDB_MIXR_MASK		BIT(PDB_MIXR_SHIFT)

/* COD3025X_13_PD_DA1 */
#define EN_DCTL_PREQ_SHIFT	5
#define EN_DCTL_PREQ_MASK	BIT(EN_DCTL_PREQ_SHIFT)

#define EN_DCTR_PREQ_SHIFT	4
#define EN_DCTR_PREQ_MASK	BIT(EN_DCTR_PREQ_SHIFT)

#define PDB_DCTL_SHIFT		3
#define PDB_DCTL_MASK		BIT(PDB_DCTL_SHIFT)

#define PDB_DCTR_SHIFT		2
#define PDB_DCTR_MASK		BIT(PDB_DCTR_SHIFT)

#define RESETB_DCTL_SHIFT	1
#define RESETB_DCTL_MASK	BIT(RESETB_DCTL_SHIFT)

#define RESETB_DCTR_SHIFT	0
#define RESETB_DCTR_MASK	BIT(RESETB_DCTR_SHIFT)

/* COD3025X_14_PD_DA2 */
#define PDB_SPK_PGA_SHIFT	3
#define PDB_SPK_PGA_MASK	BIT(PDB_SPK_PGA_SHIFT)

#define PDB_SPK_BIAS_SHIFT	2
#define PDB_SPK_BIAS_MASK	BIT(PDB_SPK_BIAS_SHIFT)

#define PDB_SPK_DAMP_SHIFT	1
#define PDB_SPK_DAMP_MASK	BIT(PDB_SPK_DAMP_SHIFT)

#define PDB_SPK_PROT_SHIFT	0
#define PDB_SPK_PROT_MASK	BIT(PDB_SPK_PROT_SHIFT)

/* COD3025X_15_PD_DA3 */
#define PDB_DOUBLER_SHIFT	7
#define PDB_DOUBLER_MASK	BIT(PDB_DOUBLER_SHIFT)

#define PDB_CP_SHIFT		6
#define PDB_CP_MASK		BIT(PDB_CP_SHIFT)

#define PDB_HP_CORE1_SHIFT	5
#define PDB_HP_CORE1_MASK	BIT(PDB_HP_CORE1_SHIFT)

#define PDB_HP_CORE2_SHIFT	4
#define PDB_HP_CORE2_MASK	BIT(PDB_HP_CORE2_SHIFT)

#define PDB_HP_DRV_SHIFT	3
#define PDB_HP_DRV_MASK		BIT(PDB_HP_DRV_SHIFT)

#define PDB_HP_MIXER_SHIFT	2
#define PDB_HP_MIXER_MASK	BIT(PDB_HP_MIXER_SHIFT)

#define PDB_EP_CORE_SHIFT	1
#define PDB_EP_CORE_MASK	BIT(PDB_EP_CORE_SHIFT)

#define PDB_EP_DRV_SHIFT	0
#define PDB_EP_DRV_MASK		BIT(PDB_EP_DRV_SHIFT)

/* COD3025X_16_PWAUTO_AD */
#define APW_DLYST_AD_SHIFT	5
#define APW_DLYST_AD_MASK	BIT(APW_DLYST_AD_SHIFT)

#define APW_AUTO_AD_SHIFT	4
#define APW_AUTO_AD_MASK	BIT(APW_AUTO_AD_SHIFT)

#define APW_MIC3_SHIFT		2
#define APW_MIC3_MASK		BIT(APW_MIC3_SHIFT)

#define APW_MIC1_SHIFT		1
#define APW_MIC1_MASK		BIT(APW_MIC1_SHIFT)

#define APW_MIC2_SHIFT		0
#define APW_MIC2_MASK		BIT(APW_MIC2_SHIFT)

/* COD3025X_17_PWAUTO_DA */
#define EN_DLYST_DA_SHIFT	5
#define EN_DLYST_DA_MASK	BIT(EN_DLYST_DA_SHIFT)

#define PW_AUTO_DA_SHIFT	4
#define PW_AUTO_DA_MASK		BIT(PW_AUTO_DA_SHIFT)

#define APW_SPK_SHIFT		2
#define APW_SPK_MASK		BIT(APW_SPK_SHIFT)

#define APW_HP_SHIFT		1
#define APW_HP_MASK		BIT(APW_HP_SHIFT)

#define APW_EP_SHIFT		0
#define APW_EP_MASK		BIT(APW_EP_SHIFT)

/* COD3025X_18_CTRL_REF */
#define CTMF_VMID_SHIFT		6
#define CTMF_VMID_WIDTH		2
#define CTMF_VMID_MASK		MASK(CTMF_VMID_WIDTH, CTMF_VMID_SHIFT)

#define CTRV_MCB1_SHIFT		4
#define CTRV_MCB1_WIDTH		2
#define CTRV_MCB1_MASK		MASK(CTRV_MCB1_WIDTH, CTRV_MCB1_SHIFT)

#define MIC_BIAS1_VO_2_5V	1
#define MIC_BIAS1_VO_2_6V	2
#define MIC_BIAS1_VO_3_0V	3

#define CTRM_MCB2_SHIFT		3
#define CTRM_MCB2_MASK		BIT(CTRM_MCB2_SHIFT)

#define EN_DET_CLK_SHIFT	0
#define EN_DET_CLK_MASK		BIT(EN_DET_CLK_SHIFT)

/* COD3025X_19_ZCD_AD */
#define ZCD_RESET_TIME_SHIFT		4
#define ZCD_RESET_TIME_WIDTH		2
#define ZCD_RESET_TIME_MASK		MASK(ZCD_RESET_TIME_WIDTH, \
						ZCD_RESET_TIME_SHIFT)

#define EN_MIC3_ZCD_SHIFT	2
#define EN_MIC3_ZCD_MASK	BIT(EN_MIC3_ZCD_SHIFT)

#define EN_MIC1_ZCD_SHIFT	1
#define EN_MIC1_ZCD_MASK	BIT(EN_MIC1_ZCD_SHIFT)

#define EN_MIC2_ZCD_SHIFT	0
#define EN_MIC2_ZCD_MASK	BIT(EN_MIC2_ZCD_SHIFT)

/* COD3025X_1C_SV_DA */
#define EN_EP_SV_SHIFT		7
#define EN_EP_SV_MASK		BIT(EN_EP_SV_SHIFT)

#define EN_SPK_SV_SHIFT		6
#define EN_SPK_SV_MASK		BIT(EN_SPK_SV_SHIFT)

#define DLY_SV_SHIFT		4
#define DLY_SV_WIDTH		2
#define DLY_SV_MASK		MASK(DLY_SV_WIDTH, DLY_SV_SHIFT)

#define EN_FAST_SV_SHIFT	3
#define EN_FAST_SV_MASK		BIT(EN_FAST_SV_SHIFT)

#define EN_HP_SV_SHIFT		2
#define EN_HP_SV_MASK		BIT(EN_HP_SV_SHIFT)

#define DLY_HP_SV_SHIFT		0
#define DLY_HP_SV_WIDTH		2
#define DLY_HP_SV_MASK		MASK(DLY_HP_SV_WIDTH, DLY_HP_SV_SHIFT)

#define SV_DLY_SEL_8192_X_FS	0
#define SV_DLY_SEL_2048_X_FS	1
#define SV_DLY_SEL_512_X_FS	2
#define SV_DLY_SEL_128_X_FS	3

/* COD3025X_20_VOL_AD1 */
#define VOLAD1_CTVOL_BST1_SHIFT		5
#define VOLAD1_CTVOL_BST1_WIDTH		2

#define VOLAD1_CTVOL_BST_PGA1_SHIFT	0
#define VOLAD1_CTVOL_BST_PGA1_WIDTH	5

/* COD3025X_21_VOL_AD2 */
#define VOLAD2_CTVOL_BST2_SHIFT		5
#define VOLAD2_CTVOL_BST2_WIDTH		2

#define VOLAD2_CTVOL_BST_PGA2_SHIFT	0
#define VOLAD2_CTVOL_BST_PGA2_WIDTH	5

/* COD3025X_22_VOL_AD3 */
#define VOLAD3_CTVOL_BST3_SHIFT		5
#define VOLAD3_CTVOL_BST3_WIDTH		2

#define VOLAD3_CTVOL_BST_PGA3_SHIFT	0
#define VOLAD3_CTVOL_BST_PGA3_WIDTH	5

/* COD3025X_23_MIX_AD */
#define EN_MIX_MIC3L_SHIFT	5
#define EN_MIX_MIC3L_MASK	BIT(EN_MIX_MIC3L_SHIFT)

#define EN_MIX_MIC3R_SHIFT	4
#define EN_MIX_MIC3R_MASK	BIT(EN_MIX_MIC3R_SHIFT)

#define EN_MIX_MIC1L_SHIFT	3
#define EN_MIX_MIC1L_MASK	BIT(EN_MIX_MIC1L_SHIFT)

#define EN_MIX_MIC1R_SHIFT	2
#define EN_MIX_MIC1R_MASK	BIT(EN_MIX_MIC1R_SHIFT)

#define EN_MIX_MIC2L_SHIFT	1
#define EN_MIX_MIC2L_MASK	BIT(EN_MIX_MIC2L_SHIFT)

#define EN_MIX_MIC2R_SHIFT	0
#define EN_MIX_MIC2R_MASK	BIT(EN_MIX_MIC2R_SHIFT)

/* COD3025X_30_VOL_HPL */
/* COD3025X_31_VOL_HPR */
#define VOLHP_CTVOL_HP_SHIFT	0
#define VOLHP_CTVOL_HP_WIDTH	6
#define VOLHP_CTVOL_HP_MASK	MASK(VOLHP_CTVOL_HP_WIDTH, VOLHP_CTVOL_HP_SHIFT)

/* COD3025X_32_VOL_EP_SPK */
#define CTVOL_EP_SHIFT		4
#define CTVOL_EP_WIDTH		4
#define CTVOL_EP_MASK		MASK(CTVOL_EP_WIDTH, \
					CTVOL_EP_SHIFT)

#define CTVOL_SPK_PGA_SHIFT	0
#define CTVOL_SPK_PGA_WIDTH	4
#define CTVOL_SPK_PGA_MASK	MASK(CTVOL_SPK_PGA_WIDTH, \
					CTVOL_SPK_PGA_SHIFT)

/* COD3025X_33_CTRL_EP */

#define CTMV_CP_MODE_SHIFT	4
#define CTMV_CP_MODE_WIDTH	2
#define CTMV_CP_MODE_MASK	MASK(CTMV_CP_MODE_WIDTH, \
					CTMV_CP_MODE_SHIFT)

#define CTMV_CP_MODE_ANALOG	3
#define CTMV_CP_MODE_DIGITAL	2
#define CTMV_CP_MODE_HALF_VDD	1
#define CTMV_CP_MODE_FULL_VDD	0

#define EN_EP_PRT_SHIFT		1
#define EN_EP_PRT_MASK		BIT(EN_EP_PRT_SHIFT)

#define EN_EP_IDET_SHIFT	0
#define EN_EP_IDET_MASK		BIT(EN_EP_IDET_SHIFT)

/* COD3025X_34_CTRL_HP */
#define EN_HP_OUTTIE_SHIFT	6
#define EN_HP_OUTTIE_MASK	BIT(EN_HP_OUTTIE_SHIFT)

#define EN_HP_MUTEL_SHIFT	5
#define EN_HP_MUTEL_MASK	BIT(EN_HP_MUTEL_SHIFT)

#define EN_HP_MUTER_SHIFT	4
#define EN_HP_MUTER_MASK	BIT(EN_HP_MUTER_SHIFT)

#define EN_HP_PRT_SHIFT		3
#define EN_HP_PRT_MASK		BIT(EN_HP_PRT_SHIFT)

#define CTPOP_HP_SHIFT		2
#define CTPOP_HP_MASK		BIT(CTPOP_HP_SHIFT)

#define EN_HP_BPS_SHIFT		1
#define EN_HP_BPS_MASK		BIT(EN_HP_BPS_SHIFT)

#define EN_HP_IDET_SHIFT	0
#define EN_HP_IDET_MASK		BIT(EN_HP_IDET_SHIFT)

/* COD3025X_35_CTRL_SPK */
#define EN_SPK_CAR_SHIFT	3
#define EN_SPK_CAR_MASK		BIT(EN_SPK_CAR_SHIFT)

#define EN_SPK_DAMP_SHIFT	2
#define EN_SPK_DAMP_MASK	BIT(EN_SPK_DAMP_SHIFT)

/* COD3025X_36_MIX_DA1 */
#define EN_HP_MIXL_DCTL_SHIFT	7
#define EN_HP_MIXL_DCTL_MASK	BIT(EN_HP_MIXL_DCTL_SHIFT)

#define EN_HP_MIXL_DCTR_SHIFT	6
#define EN_HP_MIXL_DCTR_MASK	BIT(EN_HP_MIXL_DCTR_SHIFT)

#define EN_HP_MIXL_MIXL_SHIFT	5
#define EN_HP_MIXL_MIXL_MASK	BIT(EN_HP_MIXL_MIXL_SHIFT)

#define EN_HP_MIXL_MIXR_SHIFT	4
#define EN_HP_MIXL_MIXR_MASK	BIT(EN_HP_MIXL_MIXR_SHIFT)

#define EN_HP_MIXR_DCTL_SHIFT	3
#define EN_HP_MIXR_DCTL_MASK	BIT(EN_HP_MIXR_DCTL_SHIFT)

#define EN_HP_MIXR_DCTR_SHIFT	2
#define EN_HP_MIXR_DCTR_MASK	BIT(EN_HP_MIXR_DCTR_SHIFT)

#define EN_HP_MIXR_MIXL_SHIFT	1
#define EN_HP_MIXR_MIXL_MASK	BIT(EN_HP_MIXR_MIXL_SHIFT)

#define EN_HP_MIXR_MIXR_SHIFT	0
#define EN_HP_MIXR_MIXR_MASK	BIT(EN_HP_MIXR_MIXR_SHIFT)

/* COD3025X_37_MIX_DA2 */
#define EN_EP_MIX_DCTL_SHIFT	7
#define EN_EP_MIX_DCTL_MASK	BIT(EN_EP_MIX_DCTL_SHIFT)

#define EN_EP_MIX_DCTR_SHIFT	6
#define EN_EP_MIX_DCTR_MASK	BIT(EN_EP_MIX_DCTR_SHIFT)

#define EN_SPK_MIX_DCTL_SHIFT	3
#define EN_SPK_MIX_DCTL_MASK	BIT(EN_SPK_MIX_DCTL_SHIFT)

#define EN_SPK_MIX_DCTR_SHIFT	2
#define EN_SPK_MIX_DCTR_MASK	BIT(EN_SPK_MIX_DCTR_SHIFT)

/* COD3025X_40_DIGITAL_POWER */
#define PDB_ADCDIG_SHIFT	7
#define PDB_ADCDIG_MASK		BIT(PDB_ADCDIG_SHIFT)

#define RSTB_DAT_AD_SHIFT	6
#define RSTB_DAT_AD_MASK	BIT(RSTB_DAT_AD_SHIFT)

#define PDB_DACDIG_SHIFT	5
#define PDB_DACDIG_MASK		BIT(PDB_DACDIG_SHIFT)

#define RSTB_DAT_DA_SHIFT	4
#define RSTB_DAT_DA_MASK	BIT(RSTB_DAT_DA_SHIFT)

#define SYS_RSTB_SHIFT		3
#define SYS_RSTB_MASK		BIT(SYS_RSTB_SHIFT)

#define RSTB_OVFW_DA_SHIFT	2
#define RSTB_OVFW_DA_MASK	BIT(RSTB_OVFW_DA_SHIFT)

#define ABN_STA_CHK_SHIFT	1
#define ABN_STA_CHK_MASK	BIT(ABN_STA_CHK_SHIFT)

/* COD3025X_41_FORMAT */
#define DATA_WORD_LENGTH_SHIFT	6
#define DATA_WORD_LENGTH_WIDTH	2
#define DATA_WORD_LENGTH_MASK	MASK(DATA_WORD_LENGTH_WIDTH, \
					DATA_WORD_LENGTH_SHIFT)
#define DATA_WORD_LENGTH_16	0
#define DATA_WORD_LENGTH_20	1
#define DATA_WORD_LENGTH_24	2
#define DATA_WORD_LENGTH_32	3

#define INPUT_BCLK_FREQ_SHIFT	4
#define INPUT_BCLK_FREQ_WIDTH	2
#define INPUT_BCLK_FREQ_MASK	MASK(INPUT_BCLK_FREQ_WIDTH, \
					INPUT_BCLK_FREQ_SHIFT)
#define INPUT_BCLK_FREQ_32FS	0
#define INPUT_BCLK_FREQ_48FS	1
#define INPUT_BCLK_FREQ_64FS	2

#define I2S_AUDIO_FORMAT_SHIFT	3
#define I2S_AUDIO_FORMAT_MASK	BIT(I2S_AUDIO_FORMAT_SHIFT)
#define I2S_AUDIO_FORMAT_LJ_RJ	0
#define I2S_AUDIO_FORMAT_I2S	1

#define LRJ_AUDIO_FORMAT_SHIFT	2
#define LRJ_AUDIO_FORMAT_MASK	BIT(LRJ_AUDIO_FORMAT_SHIFT)

#define BCLK_POL_SHIFT		1
#define BCLK_POL_MASK		BIT(BCLK_POL_SHIFT)

#define LRCLK_POL_SHIFT		0
#define LRCLK_POL_MASK		BIT(LRCLK_POL_SHIFT)

/* COD3025X_42_ADC1 */
#define ADC1_MAXSCALE_SHIFT	4
#define ADC1_MAXSCALE_WIDTH	2
#define ADC1_MAXSCALE_MASK	MASK(ADC1_MAXSCALE_WIDTH, ADC1_MAXSCALE_SHIFT)

#define ADC1_MAXSCALE_1_5_DB	0
#define ADC1_MAXSCALE_0_5_DB	1
#define ADC1_MAXSCALE_1_0_DB	2
#define ADC1_MAXSCALE_0_0_DB	3

#define ADC1_HPF_EN_SHIFT	3
#define ADC1_HPF_EN_MASK	BIT(ADC1_HPF_EN_SHIFT)

#define ADC1_HPF_SEL_SHIFT	1
#define ADC1_HPF_SEL_WIDTH	2
#define ADC1_HPF_SEL_MASK	MASK(ADC1_HPF_SEL_WIDTH, ADC1_HPF_SEL_SHIFT)

#define ADC1_HPF_SEL_238HZ	0
#define ADC1_HPF_SEL_200HZ	1
#define ADC1_HPF_SEL_100HZ	2
#define ADC1_HPF_SEL_14_9HZ	3

#define ADC1_MUTE_AD_EN_SHIFT	0
#define ADC1_MUTE_AD_EN_MASK	BIT(ADC1_MUTE_AD_EN_SHIFT)

/* COD3025X_50_DAC1 */
#define DAC1_MONOMIX_SHIFT	4
#define DAC1_SOFT_MUTE_SHIFT	1
#define DAC1_SOFT_MUTE_MASK	BIT(DAC1_SOFT_MUTE_SHIFT)

#define DAC1_HARD_MUTE_SHIFT	0
#define DAC1_HARD_MUTE_MASK	BIT(DAC1_HARD_MUTE_SHIFT)

/**
 * COD3025X_43_ADC_L_VOL, COD3025X_44_ADC_R_VOL
 * COD3025X_51_DAC_L_VOL, COD3025X_52_DAC_R_VOL
 */
#define AD_DA_DVOL_SHIFT	0
#define AD_DA_DVOL_WIDTH	8
#define AD_DA_DVOL_MAXNUM	0x97

/** COD3025X_53_MQS **/
#define MQS_MODE_SHIFT		0
#define MQS_MODE_MASK		BIT(MQS_MODE_SHIFT)

/** COD3025X_54_DNC1	**/
#define EN_DNC_SHIFT		7
#define EN_DNC_MASK		BIT(EN_DNC_SHIFT)

#define DNC_START_GAIN_SHIFT	6
#define DNC_START_GAIN_MASK	BIT(DNC_START_GAIN_SHIFT)

#define DNC_MODE_SHIFT		4
#define DNC_MODE_WIDTH		2
#define DNC_MODE_MASK		MASK(DNC_MODE_WIDTH, DNC_MODE_SHIFT)

#define DNC_MODE_STERIO		0
#define DNC_MODE_RIGHT		1
#define DNC_MODE_LEFT		2

#define DNC_LIMIT_SEL_SHIFT	0
#define DNC_LIMIT_SEL_WIDTH	4
#define DNC_LIMIT_SEL_MASK	MASK(DNC_LIMIT_SEL_WIDTH, DNC_LIMIT_SEL_SHIFT)

/** COD3025X_55_DNC2 **/
#define DNC_MIN_GAIN_SHIFT	5
#define DNC_MIN_GAIN_WIDTH	3

#define DNC_MAX_GAIN_SHIFT	0
#define DNC_MAX_GAIN_WIDTH	5

/** COD3025X_56_DNC3 **/
#define DNC_LVL_L_SHIFT		4
#define DNC_LVL_L_WIDTH		3

#define DNC_LVL_R_SHIFT		0
#define DNC_LVL_R_WIDTH		3

/** COD3025X_57_DNC4 **/
#define DNC_WINSEL_SHIFT	5
#define DNC_WINSEL_WIDTH	3
#define DNC_WINSEL_MASK		MASK(DNC_WINSEL_WIDTH, DNC_WINSEL_SHIFT)

#define DNC_WIN_SIZE_20HZ	7
#define DNC_WIN_SIZE_40HZ	6
#define DNC_WIN_SIZE_80HZ	5
#define DNC_WIN_SIZE_160HZ	4
#define DNC_WIN_SIZE_375HZ	3
#define DNC_WIN_SIZE_750HZ	2
#define DNC_WIN_SIZE_1500HZ	1
#define DNC_WIN_SIZE_3000HZ	0

/** COD3025X_71_CLK1_DA **/
#define CLK_DA_INV_SHIFT	7
#define CLK_DA_INV_MASK		BIT(CLK_DA_INV_SHIFT)

#define CLK_DACHOP_INV_SHIFT	6
#define CLK_DACHOP_INV_MASK	BIT(CLK_DACHOP_INV_SHIFT)

#define SEL_CHCLK_DA_SHIFT	4
#define SEL_CHCLK_DA_WIDTH	2
#define SEL_CHCLK_DA_MASK	MASK(SEL_CHCLK_DA_WIDTH, SEL_CHCLK_DA_SHIFT)

#define DAC_CHOP_CLK_1_BY_4	0
#define DAC_CHOP_CLK_1_BY_8	1
#define DAC_CHOP_CLK_1_BY_16	2
#define DAC_CHOP_CLK_1_BY_32	3

#define EN_HALF_CHOP_HP_SHIFT	2
#define EN_HALF_CHOP_HP_WIDTH	2
#define EN_HALF_CHOP_HP_MASK	MASK(EN_HALF_CHOP_HP_WIDTH, \
					EN_HALF_CHOP_HP_SHIFT)

#define DAC_HP_PHASE_SEL_ORIG	0
#define DAC_HP_PHASE_SEL_1_BY_4	1
#define DAC_HP_PHASE_SEL_2_BY_4	2
#define DAC_HP_PHASE_SEL_3_BY_4	3

#define EN_HALF_CHOP_DA_SHIFT	0
#define EN_HALF_CHOP_DA_WIDTH	2
#define EN_HALF_CHOP_DA_MASK	MASK(EN_HALF_CHOP_DA_WIDTH, \
					EN_HALF_CHOP_DA_SHIFT)

#define DAC_PHASE_SEL_ORIG	0
#define DAC_PHASE_SEL_1_BY_4	1
#define DAC_PHASE_SEL_2_BY_4	2
#define DAC_PHASE_SEL_3_BY_4	3

/** COD3025X_72_DSM1_DA **/
#define DAC_OSR128_SHIFT	6
#define DAC_OSR128_MASK		BIT(DAC_OSR128_SHIFT)

#define DAC_DWAB_SHIFT		5
#define DAC_DWAB_MASK		BIT(DAC_DWAB_SHIFT)

#define DAC_DTHON_SHIFT		4
#define DAC_DTHON_MASK		BIT(DAC_DTHON_SHIFT)

#define DAC_SYM_DTH_SHIFT	3
#define DAC_SYM_DTH_MASK	BIT(DAC_SYM_DTH_SHIFT)

#define DAC_AMT_DTH_SHIFT	0
#define DAC_AMT_DTH_WIDTH	3
#define DAC_AMT_DTH_MASK	MASK(DAC_AMT_DTH_WIDTH, DAC_AMT_DTH_SHIFT)

/** COD3025X_74_TEST **/
#define TEST_LOOPSEL_DIG_SHIFT	6
#define TEST_LOOPSEL_DIG_WIDTH	2
#define TEST_LOOPSEL_DIG_MASK	MASK(TEST_LOOPSEL_DIG_WIDTH, \
					TEST_LOOPSEL_DIG_SHIFT)

#define TEST_LOOPBACK_DIG_SHIFT	5
#define TEST_LOOPBACK_DIG_MASK	BIT(TEST_LOOPBACK_DIG_SHIFT)

#define TEST_TOSE_AD_SHIFT	3
#define TEST_TOSE_AD_WIDTH	2
#define TEST_TOSE_AD_MASK	MASK(TEST_TOSE_AD_WIDTH, TEST_TOSE_AD_SHIFT)

#define TEST_LOOPSEL_FULL_SHIFT	1
#define TEST_LOOPSEL_FULL_WIDTH	2
#define TEST_LOOPSEL_FULL_MASK	MASK(TEST_LOOPSEL_FULL_WIDTH, \
					TEST_LOOPSEL_FULL_SHIFT)

#define TEST_LOOPBACK_FULL_SHIFT	0
#define TEST_LOOPBACK_FULL_MASK		BIT(TEST_LOOPBACK_FULL_SHIFT)

/* COD3025X_75_CHOP_AD */
#define EN_MIC3_CHOP_SHIFT	5
#define EN_MIC3_CHOP_MASK	BIT(EN_MIC3_CHOP_SHIFT)

#define EN_MIC_CHOP_SHIFT	4
#define EN_MIC_CHOP_MASK	BIT(EN_MIC_CHOP_SHIFT)

#define EN_DSM_CHOP_SHIFT	3
#define EN_DSM_CHOP_MASK	BIT(EN_DSM_CHOP_SHIFT)

#define EN_MIX_CHOP_SHIFT	2
#define EN_MIX_CHOP_MASK	BIT(EN_MIX_CHOP_SHIFT)

#define EN_MCB1_CHOP_SHIFT	1
#define EN_MCB1_CHOP_MASK	BIT(EN_MCB1_CHOP_SHIFT)

#define EN_MCB2_CHOP_SHIFT	0
#define EN_MCB2_CHOP_MASK	BIT(EN_MCB2_CHOP_SHIFT)

/** COD3025X_76_CHOP_DA **/
#define EN_DCT_CHOP_SHIFT	5
#define EN_DCT_CHOP_MASK	BIT(EN_DCT_CHOP_SHIFT)

#define EN_HP_CHOP_SHIFT	4
#define EN_HP_CHOP_MASK		BIT(EN_HP_CHOP_SHIFT)

#define EN_EP_CHOP_SHIFT	3
#define EN_EP_CHOP_MASK		BIT(EN_EP_CHOP_SHIFT)

#define EN_SPK_PGA_CHOP_SHIFT	2
#define EN_SPK_PGA_CHOP_MASK	BIT(EN_SPK_PGA_CHOP_SHIFT)

#define EN_SPK_CHOP_SHIFT	1
#define EN_SPK_CHOP_MASK	BIT(EN_SPK_CHOP_SHIFT)

/** COD3025X_80_DET_PDB **/
#define DET_EN_TEST_DET_SHIFT   0
#define DET_EN_TEST_DET_MASK    BIT(DET_EN_TEST_DET_SHIFT)

#define DET_PDB_MCB2_SHIFT      2
#define DET_PDB_MCB2_MASK       BIT(DET_PDB_MCB2_SHIFT)

#define DET_PDB_MCB_LDO_SHIFT   3
#define DET_PDB_MCB_LDO_MASK    BIT(DET_PDB_MCB_LDO_SHIFT)

#define DET_PDB_BTN_DET_SHIFT   4
#define DET_PDB_BTN_DET_MASK    BIT(DET_PDB_BTN_DET_SHIFT)

#define DET_PDB_MIC_DET_SHIFT   5
#define DET_PDB_MIC_DET_MASK    BIT(DET_PDB_MIC_DET_SHIFT)

#define DET_PDB_BTN_RES1_SHIFT  6
#define DET_PDB_BTN_RES1_MASK   BIT(DET_PDB_BTN_RES1_SHIFT)

#define DET_PDB_BTN_RES2_SHIFT  7
#define DET_PDB_BTN_RES2_MASK   BIT(DET_PDB_BTN_RES2_SHIFT)

/** COD3025X_81_DET_ON **/
#define EN_PDB_JD_CLK_SHIFT	1
#define EN_PDB_JD_CLK_MASK	BIT(EN_PDB_JD_CLK_SHIFT)

#define EN_PDB_JD_SHIFT		0
#define EN_PDB_JD_MASK		BIT(EN_PDB_JD_SHIFT)

/** COD3025X_82_MIC_BIAS **/
#define CTRV_MCB2_SHIFT		2
#define CTRV_MCB2_WIDTH		2
#define CTRV_MCB2_MASK		MASK(CTRV_MCB2_WIDTH, CTRV_MCB2_SHIFT)

#define MIC_BIAS2_VO_2_5V	1
#define MIC_BIAS2_VO_2_6V	2
#define MIC_BIAS2_VO_3_0V	3

#define CTRV_MCB_LDO_SHIFT	0
#define CTRV_MCB_LDO_WIDTH	2
#define CTRV_MCB_LDO_MASK	MASK(CTRV_MCB_LDO_WIDTH, CTRV_MCB_LDO_SHIFT)

#define MIC_BIAS_LDO_VO_2_8V	1
#define MIC_BIAS_LDO_VO_3_0V	2
#define MIC_BIAS_LDO_VO_3_3V	3

/** COD3025X_83_JACK_DET1 **/
#define CTMP_JD_MODE_SHIFT	4
#define CTMP_JD_MODE_MASK	BIT(CTMP_JD_MODE_SHIFT)

#define CTRV_JD_POP_SHIFT	2
#define CTRV_JD_POP_WIDTH	2
#define CTRV_JD_POP_MASK	MASK(CTRV_JD_POP_WIDTH, CTRV_JD_POP_SHIFT)

#define CTRV_JD_VTH_SHIFT	0
#define CTRV_JD_VTH_WIDTH	2
#define CTRV_JD_VTH_MASK	MASK(CTRV_JD_VTH_WIDTH, CTRV_JD_VTH_SHIFT)

/** COD3025_84_JACK_DET2 **/
#define CTMD_JD_IRQ_DBNC_SHIFT  4
#define CTMD_JD_IRQ_DBNC_WIDTH  2
#define CTMD_JD_IRQ_DBNC_MASK   MASK(CTMD_JD_IRQ_DBNC_WIDTH, CTMD_JD_IRQ_DBNC_SHIFT)

#define CTMD_JD_DBNC_SHIFT      0
#define CTMD_JD_DBNC_WIDTH      4
#define CTMD_JD_DBNC_MASK       MASK(CTMD_JD_DBNC_WIDTH, CTMD_JD_DBNC_SHIFT)

/** COD3025X_86_DET_TIME **/
#define CTMF_DETB_PERIOD_SHIFT	4
#define CTMF_DETB_PERIOD_WIDTH	4
#define CTMF_DETB_PERIOD_MASK	MASK(CTMF_DETB_PERIOD_WIDTH, \
					CTMF_DETB_PERIOD_SHIFT)
#define CTMF_DETB_PERIOD_8      0
#define CTMF_DETB_PERIOD_16     1
#define CTMF_DETB_PERIOD_32     2
#define CTMF_DETB_PERIOD_64     3
#define CTMF_DETB_PERIOD_128    4
#define CTMF_DETB_PERIOD_256    5
#define CTMF_DETB_PERIOD_512    6
#define CTMF_DETB_PERIOD_1024   7
#define CTMF_DETB_PERIOD_2048   8
#define CTMF_DETB_PERIOD_4096   9
#define CTMF_DETB_PERIOD_8192   10
#define CTMF_DETB_PERIOD_16384  11
#define CTMF_DETB_PERIOD_16384  11

#define CTMF_BTN_ON_SHIFT	2
#define CTMF_BTN_ON_WIDTH	2
#define CTMF_BTN_ON_MASK	MASK(CTMF_BTN_ON_WIDTH, CTMF_BTN_ON_SHIFT)

#define CTMF_BTN_ON_8_CLK	0
#define CTMF_BTN_ON_10_CLK	1
#define CTMF_BTN_ON_12_CLK	2
#define CTMF_BTN_ON_14_CLK	3

#define CTMD_BTN_DBNC_SHIFT	0
#define CTMD_BTN_DBNC_WIDTH	2
#define CTMD_BTN_DBNC_MASK	MASK(CTMD_BTN_DBNC_WIDTH, CTMD_BTN_DBNC_SHIFT)

#define CTMD_BTN_DBNC_2		0
#define CTMD_BTN_DBNC_3		1
#define CTMD_BTN_DBNC_4		2
#define CTMD_BTN_DBNC_5		3

/** COD3025X_87_LDO_DIG */
#define CTMD_JD_DBNC_4_SHIFT	6
#define CTMD_JD_DBNC_4_MASK	BIT(CTMD_JD_DBNC_4_SHIFT)

/* COD3025X_D0_CTRL_IREF1 */
#define CTMI_VCM_SHIFT		4
#define CTMI_VCM_WIDTH		3
#define CTMI_VCM_MASK		MASK(CTMI_VCM_WIDTH, CTMI_VCM_SHIFT)

#define CTMI_VCM_2U		0x0
#define CTMI_VCM_3U		0x1
#define CTMI_VCM_4U		0x2
#define CTMI_VCM_5U		0x3
#define CTMI_VCM_6U		0x4
#define CTMI_VCM_7U		0x5
#define CTMI_VCM_8U		0x6
#define CTMI_VCM_9U		0x7

#define CTMI_MIX_SHIFT		0
#define CTMI_MIX_WIDTH		3
#define CTMI_MIX_MASK		MASK(CTMI_MIX_WIDTH, CTMI_MIX_SHIFT)

#define CTMI_MIX_1U		0x0
#define CTMI_MIX_1_5U		0x1
#define CTMI_MIX_2U		0x2
#define CTMI_MIX_2_5U		0x3
#define CTMI_MIX_3U		0x4
#define CTMI_MIX_3_5U		0x5
#define CTMI_MIX_4U		0x6
#define CTMI_MIX_4_5U		0x7

/* COD3025X_D1_CTRL_IREF2 */
#define CTMI_BST_SHIFT		4
#define CTMI_BST_WIDTH		3
#define CTMI_BST_MASK		MASK(CTMI_BST_WIDTH, CTMI_BST_SHIFT)

#define CTMI_INT1_SHIFT		0
#define CTMI_INT1_WIDTH		3
#define CTMI_INT1_MASK		MASK(CTMI_INT1_WIDTH, CTMI_INT1_SHIFT)

#define CTMI_INT1_2U		0x0
#define CTMI_INT1_3U		0x1
#define CTMI_INT1_4U		0x2
#define CTMI_INT1_5U		0x3
#define CTMI_INT1_6U		0x4
#define CTMI_INT1_7U		0x5
#define CTMI_INT1_8U		0x6
#define CTMI_INT1_9U		0x7

/* COD3025X_D2_CTRL_IREF3 */
#define CTMI_MIC2_SHIFT		4
#define CTMI_MIC2_WIDTH		3
#define CTMI_MIC2_MASK		MASK(CTMI_MIC2_WIDTH, CTMI_MIC2_SHIFT)

#define CTMI_MIC2_1U		0x0
#define CTMI_MIC2_1_5U		0x1
#define CTMI_MIC2_2U		0x2
#define CTMI_MIC2_2_5U		0x3
#define CTMI_MIC2_3U		0x4
#define CTMI_MIC2_3_5U		0x5
#define CTMI_MIC2_4U		0x6
#define CTMI_MIC2_4_5U		0x7

#define CTMI_MIC1_SHIFT		0
#define CTMI_MIC1_WIDTH		3
#define CTMI_MIC1_MASK		MASK(CTMI_MIC1_WIDTH, CTMI_MIC1_SHIFT)

#define CTMI_MIC1_1U		0x0
#define CTMI_MIC1_1_5U		0x1
#define CTMI_MIC1_2U		0x2
#define CTMI_MIC1_2_5U		0x3
#define CTMI_MIC1_3U		0x4
#define CTMI_MIC1_3_5U		0x5
#define CTMI_MIC1_4U		0x6
#define CTMI_MIC1_4_5U		0x7

/* COD3025X_D3_CTRL_IREF4 */
#define CTMI_MIC_BUFF_SHIFT	4
#define CTMI_MIC_BUFF_WIDTH	3
#define CTMI_MIC_BUFF_MASK	MASK(CTMI_MIC_BUFF_WIDTH, CTMI_MIC_BUFF_SHIFT)

#define CTMI_MIC_BUFF_1U		0x0
#define CTMI_MIC_BUFF_1_5U		0x1
#define CTMI_MIC_BUFF_2U		0x2
#define CTMI_MIC_BUFF_2_5U		0x3
#define CTMI_MIC_BUFF_3U		0x4
#define CTMI_MIC_BUFF_3_5U		0x5
#define CTMI_MIC_BUFF_4U		0x6
#define CTMI_MIC_BUFF_4_5U		0x7

#define CTMI_MIC3_SHIFT		0
#define CTMI_MIC3_WIDTH		3
#define CTMI_MIC3_MASK		MASK(CTMI_MIC3_WIDTH, CTMI_MIC3_SHIFT)

#define CTMI_MIC3_1U		0x0
#define CTMI_MIC3_1_5U		0x1
#define CTMI_MIC3_2U		0x2
#define CTMI_MIC3_2_5U		0x3
#define CTMI_MIC3_3U		0x4
#define CTMI_MIC3_3_5U		0x5
#define CTMI_MIC3_4U		0x6
#define CTMI_MIC3_4_5U		0x7

/* COD3025X_D7_CTRL_CP1 */
#define CTRV_CP_POSREF_SHIFT	4
#define CTRV_CP_POSREF_WIDTH	4
#define CTRV_CP_POSREF_MASK	MASK(CTRV_CP_POSREF_WIDTH, \
					CTRV_CP_POSREF_SHIFT)

#define CTRV_CP_NEGREF_SHIFT	0
#define CTRV_CP_NEGREF_WIDTH	4
#define CTRV_CP_NEGREF_MASK	MASK(CTRV_CP_NEGREF_WIDTH, \
					CTRV_CP_NEGREF_SHIFT)

/* COD3025X_D8_CTRL_CP2 */
#define CTMD_CP_H2L_SHIFT	2
#define CTMD_CP_H2L_WIDTH	2
#define CTMD_CP_H2L_MASK	MASK(CTMD_CP_H2L_WIDTH, \
					CTMD_CP_H2L_SHIFT)

#define VIDD_HALF_VDD_DELAY_256MS	3
#define VIDD_HALF_VDD_DELAY_128MS	2
#define VIDD_HALF_VDD_DELAY_64MS	1
#define VIDD_HALF_VDD_DELAY_32MS	0

#define CTMF_CP_CLK_SHIFT	0
#define CTMF_CP_CLK_WIDTH	2
#define CTMF_CP_CLK_MASK	MASK(CTMF_CP_CLK_WIDTH, \
					CTMF_CP_CLK_SHIFT)

#define CP_MAIN_CLK_3MHZ	3
#define CP_MAIN_CLK_1_5MHZ	2
#define CP_MAIN_CLK_750KHZ	1
#define CP_MAIN_CLK_375KHZ	0

/* COD3025X_DB_CTRL_HPS */
#define CTMI_HP_A_SHIFT		6
#define CTMI_HP_A_WIDTH		2
#define CTMI_HP_A_MASK		MASK(CTMI_HP_A_WIDTH, CTMI_HP_A_SHIFT)

#define CTMI_HP_1_UA		0
#define CTMI_HP_2_UA		1
#define CTMI_HP_3_UA		2
#define CTMI_HP_4_UA		3

/* COD3025X_DC_CTRL_EPS */
#define CTMI_EP_A_SHIFT		6
#define CTMI_EP_A_WIDTH		2
#define CTMI_EP_A_MASK		MASK(CTMI_EP_A_WIDTH, CTMI_EP_A_SHIFT)

#define CTMI_EP_A_1_UA		0
#define CTMI_EP_A_2_UA		1
#define CTMI_EP_A_3_UA		2
#define CTMI_EP_A_4_UA		3

#define CTMI_EP_P_SHIFT		3
#define CTMI_EP_P_WIDTH		3
#define CTMI_EP_P_MASK		MASK(CTMI_EP_P_WIDTH, CTMI_EP_P_SHIFT)

#define CTMI_EP_D_SHIFT		0
#define CTMI_EP_D_WIDTH		3
#define CTMI_EP_D_MASK		MASK(CTMI_EP_D_WIDTH, CTMI_EP_D_SHIFT)

#define CTMI_EP_P_D_2_UA	0
#define CTMI_EP_P_D_3_UA	1
#define CTMI_EP_P_D_3_5_UA	2
#define CTMI_EP_P_D_4_UA	3
#define CTMI_EP_P_D_4_5_UA	4
#define CTMI_EP_P_D_5_UA	5
#define CTMI_EP_P_D_6_UA	6
#define CTMI_EP_P_D_7_UA	7

/* COD3025X_F4_OFFSET_DAL */
#define OFFSET_SIGN_DAL_SHIFT	7
#define OFFSET_SIGN_DAL_MASK	BIT(OFFSET_SIGN_DAL_SHIFT)

#define OFFSET_LV_DAL_SHIFT	0
#define OFFSET_LV_DAL_WIDTH	7
#define OFFSET_LV_DAL_MASK	MASK(OFFSET_LV_DAL_WIDTH, OFFSET_LV_DAL_SHIFT)

#endif /* _COD3025X_H */

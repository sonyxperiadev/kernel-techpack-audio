/*
* SPDX-License-Identifier: GPL-2.0-only
* Copyright (c) 2020-2021, The Linux Foundation. All rights reserved.
* Copyright (c) 2022-2023, Qualcomm Innovation Center, Inc. All rights reserved.
*/
#ifndef _MSM_COMMON_H_
#define _MSM_COMMON_H_

#include <sound/soc.h>
#include <sound/pcm.h>

typedef enum snd_card_status_t {
	SND_CARD_STATUS_OFFLINE = 0,
	SND_CARD_STATUS_ONLINE  = 1,
} snd_card_status_t;

enum {
	PRIM_MI2S = 0,
	SEC_MI2S,
	TERT_MI2S,
	QUAT_MI2S,
	QUIN_MI2S,
	SEN_MI2S,
	MI2S_MAX,
};

struct snd_card_pdata {
	struct kobject snd_card_kobj;
	int card_status;
};

struct msm_asoc_mach_data {
	struct snd_info_entry *codec_root;
	int usbc_en2_gpio; /* used by gpio driver API */
	int hph_en1_gpio;
	int hph_en0_gpio;
	int lito_v2_enabled;
	struct device_node *dmic01_gpio_p; /* used by pinctrl API */
	struct device_node *dmic23_gpio_p; /* used by pinctrl API */
	struct device_node *dmic45_gpio_p; /* used by pinctrl API */
	struct device_node *mi2s_gpio_p[MI2S_MAX]; /* used by pinctrl API */
	atomic_t mi2s_gpio_ref_count[MI2S_MAX]; /* used by pinctrl API */
	struct device_node *us_euro_gpio_p; /* used by pinctrl API */
	struct pinctrl *usbc_en2_gpio_p; /* used by pinctrl API */
	struct device_node *hph_en1_gpio_p; /* used by pinctrl API */
	struct device_node *hph_en0_gpio_p; /* used by pinctrl API */
	bool is_afe_config_done;
	struct device_node *fsa_handle;
	struct clk *lpass_audio_hw_vote;
	int core_audio_vote_count;
	u32 tdm_max_slots; /* Max TDM slots used */
	int wcd_disabled;
	u32 wsa_max_devs;
};

int msm_lpass_audio_hw_vote_req(struct snd_pcm_substream *substream, bool enable);
int snd_card_notify_user(snd_card_status_t card_status);
int snd_card_set_card_status(snd_card_status_t card_status);
#endif

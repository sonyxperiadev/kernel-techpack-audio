// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2020-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2022-2023, Qualcomm Innovation Center, Inc. All rights reserved.
 */

#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/of_device.h>
#include <sound/control.h>
#include <sound/core.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>
#include <sound/info.h>


#include "msm_common.h"

#define BUF_SZ 32
#define DIR_SZ 10

struct snd_card_pdata *snd_card_pdata;

static struct attribute card_state_attr = {
	.name = "card_state",
	.mode = 0666,
};

int snd_card_notify_user(snd_card_status_t card_status)
{
	snd_card_pdata->card_status = card_status;
	sysfs_notify(&snd_card_pdata->snd_card_kobj, NULL, "card_state");
	return 0;
}
EXPORT_SYMBOL(snd_card_notify_user);

int snd_card_set_card_status(snd_card_status_t card_status)
{
	snd_card_pdata->card_status = card_status;
	return 0;
}
EXPORT_SYMBOL(snd_card_set_card_status);

int msm_lpass_audio_hw_vote_req(struct snd_pcm_substream *substream, bool enable)
{
        struct snd_soc_pcm_runtime *rtd = substream->private_data;
        struct snd_soc_card *card = rtd->card;
        struct msm_asoc_mach_data *pdata = snd_soc_card_get_drvdata(card);
        int rc = 0;

        if (enable) {
                if (pdata->lpass_audio_hw_vote == NULL) {
                        dev_err(rtd->card->dev, "%s: Invalid lpass audio hw node\n",
                                __func__);
                        rc = -EINVAL;
                        goto rtn;
                }
                if (pdata->core_audio_vote_count == 0) {
                        rc = clk_prepare_enable(pdata->lpass_audio_hw_vote);
                        if (rc < 0) {
                                dev_err(rtd->card->dev, "%s: audio vote error\n",
                                                __func__);
                                goto rtn;
                        }
                }

                pdata->core_audio_vote_count++;
        } else {
                if (pdata->lpass_audio_hw_vote != NULL) {
                        if (--pdata->core_audio_vote_count == 0) {
                                clk_disable_unprepare(
                                        pdata->lpass_audio_hw_vote);
                        } else if (pdata->core_audio_vote_count < 0) {
                                pr_err("%s: audio vote mismatch\n", __func__);
                                pdata->core_audio_vote_count = 0;
                        }
                } else {
                        pr_err("%s: Invalid lpass audio hw node\n", __func__);
                }
        }

rtn:
  return rc;
}
EXPORT_SYMBOL(msm_lpass_audio_hw_vote_req);

static ssize_t snd_card_sysfs_show(struct kobject *kobj,
		struct attribute *attr, char *buf)
{
	return snprintf(buf, BUF_SZ, "%d", snd_card_pdata->card_status);
}

static ssize_t snd_card_sysfs_store(struct kobject *kobj,
		struct attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d", &snd_card_pdata->card_status);
	sysfs_notify(&snd_card_pdata->snd_card_kobj, NULL, "card_state");
	return count;
}

static const struct sysfs_ops snd_card_sysfs_ops = {
	.show = snd_card_sysfs_show,
	.store = snd_card_sysfs_store,
};

static struct kobj_type snd_card_ktype = {
	.sysfs_ops = &snd_card_sysfs_ops,
};

int snd_card_sysfs_init(void)
{
	int ret = 0;
	char dir[DIR_SZ] = "snd_card";

	snd_card_pdata = kcalloc(1, sizeof(struct snd_card_pdata), GFP_KERNEL);
	if(!snd_card_pdata) {
		ret = -ENOMEM;
		goto done;
	}
	ret = kobject_init_and_add(&snd_card_pdata->snd_card_kobj, &snd_card_ktype,
		kernel_kobj, dir);
	if (ret < 0) {
		pr_err("%s: Failed to add kobject %s, err = %d\n",
			__func__, dir, ret);
		goto done;
	}

	ret = sysfs_create_file(&snd_card_pdata->snd_card_kobj, &card_state_attr);
	if (ret < 0) {
		pr_err("%s: Failed to add snd_card sysfs entry to %s\n",
			__func__, dir);
		goto fail_create_file;
	}

	return ret;

fail_create_file:
	kobject_put(&snd_card_pdata->snd_card_kobj);
done:
	return ret;
}

int  __init msm_common_init(void) {
	snd_card_sysfs_init();
    return 0;
}

void  msm_common_exit(void) {
	if(snd_card_pdata)
		kfree(snd_card_pdata);
}


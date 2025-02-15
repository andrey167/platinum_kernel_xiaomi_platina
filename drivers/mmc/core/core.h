/*
 *  linux/drivers/mmc/core/core.h
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright 2007 Pierre Ossman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _MMC_CORE_CORE_H
#define _MMC_CORE_CORE_H

#include <linux/delay.h>

#define MMC_CMD_RETRIES        3

void mmc_attach_bus(struct mmc_host *host, const struct mmc_bus_ops *ops);
void mmc_detach_bus(struct mmc_host *host);

struct device_node *mmc_of_find_child_device(struct mmc_host *host,
		unsigned func_num);

void mmc_init_erase(struct mmc_card *card);

void mmc_set_chip_select(struct mmc_host *host, int mode);
void mmc_set_clock(struct mmc_host *host, unsigned int hz);
int mmc_clk_update_freq(struct mmc_host *host,
		unsigned long freq, enum mmc_load state);
void mmc_gate_clock(struct mmc_host *host);
void mmc_ungate_clock(struct mmc_host *host);
void mmc_set_ungated(struct mmc_host *host);
void mmc_set_bus_mode(struct mmc_host *host, unsigned int mode);
void mmc_set_bus_width(struct mmc_host *host, unsigned int width);
u32 mmc_select_voltage(struct mmc_host *host, u32 ocr);
int mmc_set_signal_voltage(struct mmc_host *host, int signal_voltage, u32 ocr);
int __mmc_set_signal_voltage(struct mmc_host *host, int signal_voltage);
void mmc_set_timing(struct mmc_host *host, unsigned int timing);
void mmc_set_driver_type(struct mmc_host *host, unsigned int drv_type);
int mmc_select_drive_strength(struct mmc_card *card, unsigned int max_dtr,
			      int card_drv_type, int *drv_type);
void mmc_power_up(struct mmc_host *host, u32 ocr);
void mmc_power_off(struct mmc_host *host);
void mmc_power_cycle(struct mmc_host *host, u32 ocr);
void mmc_set_initial_state(struct mmc_host *host);

static inline void mmc_delay(unsigned int ms)
{
	if (ms < 1000 / HZ) {
		cond_resched();
		mdelay(ms);
	} else if (ms < jiffies_to_msecs(2)) {
		usleep_range(ms * 1000, (ms + 1) * 1000);
	} else {
		msleep(ms);
	}
}

void mmc_rescan(struct work_struct *work);
void mmc_start_host(struct mmc_host *host);
void mmc_stop_host(struct mmc_host *host);

int _mmc_detect_card_removed(struct mmc_host *host);

int mmc_attach_mmc(struct mmc_host *host);
int mmc_attach_sd(struct mmc_host *host);
int mmc_attach_sdio(struct mmc_host *host);

/* Module parameters */
extern int use_spi_crc;

/* Debugfs information for hosts and cards */
void mmc_add_host_debugfs(struct mmc_host *host);
void mmc_remove_host_debugfs(struct mmc_host *host);

void mmc_add_card_debugfs(struct mmc_card *card);
void mmc_remove_card_debugfs(struct mmc_card *card);

void mmc_init_context_info(struct mmc_host *host);

extern bool mmc_can_scale_clk(struct mmc_host *host);
extern int mmc_init_clk_scaling(struct mmc_host *host);
extern int mmc_resume_clk_scaling(struct mmc_host *host);
extern int mmc_exit_clk_scaling(struct mmc_host *host);
extern unsigned long mmc_get_max_frequency(struct mmc_host *host);

int mmc_execute_tuning(struct mmc_card *card);
int mmc_hs200_to_hs400(struct mmc_card *card);
int mmc_hs400_to_hs200(struct mmc_card *card);

#endif


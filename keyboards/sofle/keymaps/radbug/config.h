// Copyright 2026 radbug
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Split sync for the OLED status: the right half is the slave (MASTER_LEFT)
// and only sees layer/mods/WPM state if the master syncs it over TRRS.
// These are config.h defines, not rules.mk options.

#pragma once

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_MODS_ENABLE
#define SPLIT_WPM_ENABLE
#define SPLIT_LED_STATE_ENABLE

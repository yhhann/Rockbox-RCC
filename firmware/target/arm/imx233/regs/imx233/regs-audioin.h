/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 2.1.8
 * XML versions: imx233:3.4.0
 *
 * Copyright (C) 2013 by Amaury Pouly
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#ifndef __HEADERGEN__IMX233__AUDIOIN__H__
#define __HEADERGEN__IMX233__AUDIOIN__H__

#define REGS_AUDIOIN_BASE (0x8004c000)

#define REGS_AUDIOIN_VERSION "3.4.0"

/**
 * Register: HW_AUDIOIN_CTRL
 * Address: 0
 * SCT: yes
*/
#define HW_AUDIOIN_CTRL                         (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x0 + 0x0))
#define HW_AUDIOIN_CTRL_SET                     (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x0 + 0x4))
#define HW_AUDIOIN_CTRL_CLR                     (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x0 + 0x8))
#define HW_AUDIOIN_CTRL_TOG                     (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x0 + 0xc))
#define BP_AUDIOIN_CTRL_SFTRST                  31
#define BM_AUDIOIN_CTRL_SFTRST                  0x80000000
#define BF_AUDIOIN_CTRL_SFTRST(v)               (((v) << 31) & 0x80000000)
#define BP_AUDIOIN_CTRL_CLKGATE                 30
#define BM_AUDIOIN_CTRL_CLKGATE                 0x40000000
#define BF_AUDIOIN_CTRL_CLKGATE(v)              (((v) << 30) & 0x40000000)
#define BP_AUDIOIN_CTRL_RSRVD3                  21
#define BM_AUDIOIN_CTRL_RSRVD3                  0x3fe00000
#define BF_AUDIOIN_CTRL_RSRVD3(v)               (((v) << 21) & 0x3fe00000)
#define BP_AUDIOIN_CTRL_DMAWAIT_COUNT           16
#define BM_AUDIOIN_CTRL_DMAWAIT_COUNT           0x1f0000
#define BF_AUDIOIN_CTRL_DMAWAIT_COUNT(v)        (((v) << 16) & 0x1f0000)
#define BP_AUDIOIN_CTRL_RSRVD1                  11
#define BM_AUDIOIN_CTRL_RSRVD1                  0xf800
#define BF_AUDIOIN_CTRL_RSRVD1(v)               (((v) << 11) & 0xf800)
#define BP_AUDIOIN_CTRL_LR_SWAP                 10
#define BM_AUDIOIN_CTRL_LR_SWAP                 0x400
#define BF_AUDIOIN_CTRL_LR_SWAP(v)              (((v) << 10) & 0x400)
#define BP_AUDIOIN_CTRL_EDGE_SYNC               9
#define BM_AUDIOIN_CTRL_EDGE_SYNC               0x200
#define BF_AUDIOIN_CTRL_EDGE_SYNC(v)            (((v) << 9) & 0x200)
#define BP_AUDIOIN_CTRL_INVERT_1BIT             8
#define BM_AUDIOIN_CTRL_INVERT_1BIT             0x100
#define BF_AUDIOIN_CTRL_INVERT_1BIT(v)          (((v) << 8) & 0x100)
#define BP_AUDIOIN_CTRL_OFFSET_ENABLE           7
#define BM_AUDIOIN_CTRL_OFFSET_ENABLE           0x80
#define BF_AUDIOIN_CTRL_OFFSET_ENABLE(v)        (((v) << 7) & 0x80)
#define BP_AUDIOIN_CTRL_HPF_ENABLE              6
#define BM_AUDIOIN_CTRL_HPF_ENABLE              0x40
#define BF_AUDIOIN_CTRL_HPF_ENABLE(v)           (((v) << 6) & 0x40)
#define BP_AUDIOIN_CTRL_WORD_LENGTH             5
#define BM_AUDIOIN_CTRL_WORD_LENGTH             0x20
#define BF_AUDIOIN_CTRL_WORD_LENGTH(v)          (((v) << 5) & 0x20)
#define BP_AUDIOIN_CTRL_LOOPBACK                4
#define BM_AUDIOIN_CTRL_LOOPBACK                0x10
#define BF_AUDIOIN_CTRL_LOOPBACK(v)             (((v) << 4) & 0x10)
#define BP_AUDIOIN_CTRL_FIFO_UNDERFLOW_IRQ      3
#define BM_AUDIOIN_CTRL_FIFO_UNDERFLOW_IRQ      0x8
#define BF_AUDIOIN_CTRL_FIFO_UNDERFLOW_IRQ(v)   (((v) << 3) & 0x8)
#define BP_AUDIOIN_CTRL_FIFO_OVERFLOW_IRQ       2
#define BM_AUDIOIN_CTRL_FIFO_OVERFLOW_IRQ       0x4
#define BF_AUDIOIN_CTRL_FIFO_OVERFLOW_IRQ(v)    (((v) << 2) & 0x4)
#define BP_AUDIOIN_CTRL_FIFO_ERROR_IRQ_EN       1
#define BM_AUDIOIN_CTRL_FIFO_ERROR_IRQ_EN       0x2
#define BF_AUDIOIN_CTRL_FIFO_ERROR_IRQ_EN(v)    (((v) << 1) & 0x2)
#define BP_AUDIOIN_CTRL_RUN                     0
#define BM_AUDIOIN_CTRL_RUN                     0x1
#define BF_AUDIOIN_CTRL_RUN(v)                  (((v) << 0) & 0x1)

/**
 * Register: HW_AUDIOIN_STAT
 * Address: 0x10
 * SCT: yes
*/
#define HW_AUDIOIN_STAT                 (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x10 + 0x0))
#define HW_AUDIOIN_STAT_SET             (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x10 + 0x4))
#define HW_AUDIOIN_STAT_CLR             (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x10 + 0x8))
#define HW_AUDIOIN_STAT_TOG             (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x10 + 0xc))
#define BP_AUDIOIN_STAT_ADC_PRESENT     31
#define BM_AUDIOIN_STAT_ADC_PRESENT     0x80000000
#define BF_AUDIOIN_STAT_ADC_PRESENT(v)  (((v) << 31) & 0x80000000)
#define BP_AUDIOIN_STAT_RSRVD3          0
#define BM_AUDIOIN_STAT_RSRVD3          0x7fffffff
#define BF_AUDIOIN_STAT_RSRVD3(v)       (((v) << 0) & 0x7fffffff)

/**
 * Register: HW_AUDIOIN_ADCSRR
 * Address: 0x20
 * SCT: yes
*/
#define HW_AUDIOIN_ADCSRR                       (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x20 + 0x0))
#define HW_AUDIOIN_ADCSRR_SET                   (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x20 + 0x4))
#define HW_AUDIOIN_ADCSRR_CLR                   (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x20 + 0x8))
#define HW_AUDIOIN_ADCSRR_TOG                   (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x20 + 0xc))
#define BP_AUDIOIN_ADCSRR_OSR                   31
#define BM_AUDIOIN_ADCSRR_OSR                   0x80000000
#define BV_AUDIOIN_ADCSRR_OSR__OSR6             0x0
#define BV_AUDIOIN_ADCSRR_OSR__OSR12            0x1
#define BF_AUDIOIN_ADCSRR_OSR(v)                (((v) << 31) & 0x80000000)
#define BF_AUDIOIN_ADCSRR_OSR_V(v)              ((BV_AUDIOIN_ADCSRR_OSR__##v << 31) & 0x80000000)
#define BP_AUDIOIN_ADCSRR_BASEMULT              28
#define BM_AUDIOIN_ADCSRR_BASEMULT              0x70000000
#define BV_AUDIOIN_ADCSRR_BASEMULT__SINGLE_RATE 0x1
#define BV_AUDIOIN_ADCSRR_BASEMULT__DOUBLE_RATE 0x2
#define BV_AUDIOIN_ADCSRR_BASEMULT__QUAD_RATE   0x4
#define BF_AUDIOIN_ADCSRR_BASEMULT(v)           (((v) << 28) & 0x70000000)
#define BF_AUDIOIN_ADCSRR_BASEMULT_V(v)         ((BV_AUDIOIN_ADCSRR_BASEMULT__##v << 28) & 0x70000000)
#define BP_AUDIOIN_ADCSRR_RSRVD2                27
#define BM_AUDIOIN_ADCSRR_RSRVD2                0x8000000
#define BF_AUDIOIN_ADCSRR_RSRVD2(v)             (((v) << 27) & 0x8000000)
#define BP_AUDIOIN_ADCSRR_SRC_HOLD              24
#define BM_AUDIOIN_ADCSRR_SRC_HOLD              0x7000000
#define BF_AUDIOIN_ADCSRR_SRC_HOLD(v)           (((v) << 24) & 0x7000000)
#define BP_AUDIOIN_ADCSRR_RSRVD1                21
#define BM_AUDIOIN_ADCSRR_RSRVD1                0xe00000
#define BF_AUDIOIN_ADCSRR_RSRVD1(v)             (((v) << 21) & 0xe00000)
#define BP_AUDIOIN_ADCSRR_SRC_INT               16
#define BM_AUDIOIN_ADCSRR_SRC_INT               0x1f0000
#define BF_AUDIOIN_ADCSRR_SRC_INT(v)            (((v) << 16) & 0x1f0000)
#define BP_AUDIOIN_ADCSRR_RSRVD0                13
#define BM_AUDIOIN_ADCSRR_RSRVD0                0xe000
#define BF_AUDIOIN_ADCSRR_RSRVD0(v)             (((v) << 13) & 0xe000)
#define BP_AUDIOIN_ADCSRR_SRC_FRAC              0
#define BM_AUDIOIN_ADCSRR_SRC_FRAC              0x1fff
#define BF_AUDIOIN_ADCSRR_SRC_FRAC(v)           (((v) << 0) & 0x1fff)

/**
 * Register: HW_AUDIOIN_ADCVOLUME
 * Address: 0x30
 * SCT: yes
*/
#define HW_AUDIOIN_ADCVOLUME                        (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x30 + 0x0))
#define HW_AUDIOIN_ADCVOLUME_SET                    (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x30 + 0x4))
#define HW_AUDIOIN_ADCVOLUME_CLR                    (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x30 + 0x8))
#define HW_AUDIOIN_ADCVOLUME_TOG                    (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x30 + 0xc))
#define BP_AUDIOIN_ADCVOLUME_RSRVD5                 29
#define BM_AUDIOIN_ADCVOLUME_RSRVD5                 0xe0000000
#define BF_AUDIOIN_ADCVOLUME_RSRVD5(v)              (((v) << 29) & 0xe0000000)
#define BP_AUDIOIN_ADCVOLUME_VOLUME_UPDATE_LEFT     28
#define BM_AUDIOIN_ADCVOLUME_VOLUME_UPDATE_LEFT     0x10000000
#define BF_AUDIOIN_ADCVOLUME_VOLUME_UPDATE_LEFT(v)  (((v) << 28) & 0x10000000)
#define BP_AUDIOIN_ADCVOLUME_RSRVD4                 26
#define BM_AUDIOIN_ADCVOLUME_RSRVD4                 0xc000000
#define BF_AUDIOIN_ADCVOLUME_RSRVD4(v)              (((v) << 26) & 0xc000000)
#define BP_AUDIOIN_ADCVOLUME_EN_ZCD                 25
#define BM_AUDIOIN_ADCVOLUME_EN_ZCD                 0x2000000
#define BF_AUDIOIN_ADCVOLUME_EN_ZCD(v)              (((v) << 25) & 0x2000000)
#define BP_AUDIOIN_ADCVOLUME_RSRVD3                 24
#define BM_AUDIOIN_ADCVOLUME_RSRVD3                 0x1000000
#define BF_AUDIOIN_ADCVOLUME_RSRVD3(v)              (((v) << 24) & 0x1000000)
#define BP_AUDIOIN_ADCVOLUME_VOLUME_LEFT            16
#define BM_AUDIOIN_ADCVOLUME_VOLUME_LEFT            0xff0000
#define BF_AUDIOIN_ADCVOLUME_VOLUME_LEFT(v)         (((v) << 16) & 0xff0000)
#define BP_AUDIOIN_ADCVOLUME_RSRVD2                 13
#define BM_AUDIOIN_ADCVOLUME_RSRVD2                 0xe000
#define BF_AUDIOIN_ADCVOLUME_RSRVD2(v)              (((v) << 13) & 0xe000)
#define BP_AUDIOIN_ADCVOLUME_VOLUME_UPDATE_RIGHT    12
#define BM_AUDIOIN_ADCVOLUME_VOLUME_UPDATE_RIGHT    0x1000
#define BF_AUDIOIN_ADCVOLUME_VOLUME_UPDATE_RIGHT(v) (((v) << 12) & 0x1000)
#define BP_AUDIOIN_ADCVOLUME_RSRVD1                 8
#define BM_AUDIOIN_ADCVOLUME_RSRVD1                 0xf00
#define BF_AUDIOIN_ADCVOLUME_RSRVD1(v)              (((v) << 8) & 0xf00)
#define BP_AUDIOIN_ADCVOLUME_VOLUME_RIGHT           0
#define BM_AUDIOIN_ADCVOLUME_VOLUME_RIGHT           0xff
#define BF_AUDIOIN_ADCVOLUME_VOLUME_RIGHT(v)        (((v) << 0) & 0xff)

/**
 * Register: HW_AUDIOIN_ADCDEBUG
 * Address: 0x40
 * SCT: yes
*/
#define HW_AUDIOIN_ADCDEBUG                                     (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x40 + 0x0))
#define HW_AUDIOIN_ADCDEBUG_SET                                 (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x40 + 0x4))
#define HW_AUDIOIN_ADCDEBUG_CLR                                 (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x40 + 0x8))
#define HW_AUDIOIN_ADCDEBUG_TOG                                 (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x40 + 0xc))
#define BP_AUDIOIN_ADCDEBUG_ENABLE_ADCDMA                       31
#define BM_AUDIOIN_ADCDEBUG_ENABLE_ADCDMA                       0x80000000
#define BF_AUDIOIN_ADCDEBUG_ENABLE_ADCDMA(v)                    (((v) << 31) & 0x80000000)
#define BP_AUDIOIN_ADCDEBUG_RSRVD1                              4
#define BM_AUDIOIN_ADCDEBUG_RSRVD1                              0x7ffffff0
#define BF_AUDIOIN_ADCDEBUG_RSRVD1(v)                           (((v) << 4) & 0x7ffffff0)
#define BP_AUDIOIN_ADCDEBUG_ADC_DMA_REQ_HAND_SHAKE_CLK_CROSS    3
#define BM_AUDIOIN_ADCDEBUG_ADC_DMA_REQ_HAND_SHAKE_CLK_CROSS    0x8
#define BF_AUDIOIN_ADCDEBUG_ADC_DMA_REQ_HAND_SHAKE_CLK_CROSS(v) (((v) << 3) & 0x8)
#define BP_AUDIOIN_ADCDEBUG_SET_INTERRUPT3_HAND_SHAKE           2
#define BM_AUDIOIN_ADCDEBUG_SET_INTERRUPT3_HAND_SHAKE           0x4
#define BF_AUDIOIN_ADCDEBUG_SET_INTERRUPT3_HAND_SHAKE(v)        (((v) << 2) & 0x4)
#define BP_AUDIOIN_ADCDEBUG_DMA_PREQ                            1
#define BM_AUDIOIN_ADCDEBUG_DMA_PREQ                            0x2
#define BF_AUDIOIN_ADCDEBUG_DMA_PREQ(v)                         (((v) << 1) & 0x2)
#define BP_AUDIOIN_ADCDEBUG_FIFO_STATUS                         0
#define BM_AUDIOIN_ADCDEBUG_FIFO_STATUS                         0x1
#define BF_AUDIOIN_ADCDEBUG_FIFO_STATUS(v)                      (((v) << 0) & 0x1)

/**
 * Register: HW_AUDIOIN_ADCVOL
 * Address: 0x50
 * SCT: yes
*/
#define HW_AUDIOIN_ADCVOL                           (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x50 + 0x0))
#define HW_AUDIOIN_ADCVOL_SET                       (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x50 + 0x4))
#define HW_AUDIOIN_ADCVOL_CLR                       (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x50 + 0x8))
#define HW_AUDIOIN_ADCVOL_TOG                       (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x50 + 0xc))
#define BP_AUDIOIN_ADCVOL_RSRVD4                    29
#define BM_AUDIOIN_ADCVOL_RSRVD4                    0xe0000000
#define BF_AUDIOIN_ADCVOL_RSRVD4(v)                 (((v) << 29) & 0xe0000000)
#define BP_AUDIOIN_ADCVOL_VOLUME_UPDATE_PENDING     28
#define BM_AUDIOIN_ADCVOL_VOLUME_UPDATE_PENDING     0x10000000
#define BF_AUDIOIN_ADCVOL_VOLUME_UPDATE_PENDING(v)  (((v) << 28) & 0x10000000)
#define BP_AUDIOIN_ADCVOL_RSRVD3                    26
#define BM_AUDIOIN_ADCVOL_RSRVD3                    0xc000000
#define BF_AUDIOIN_ADCVOL_RSRVD3(v)                 (((v) << 26) & 0xc000000)
#define BP_AUDIOIN_ADCVOL_EN_ADC_ZCD                25
#define BM_AUDIOIN_ADCVOL_EN_ADC_ZCD                0x2000000
#define BF_AUDIOIN_ADCVOL_EN_ADC_ZCD(v)             (((v) << 25) & 0x2000000)
#define BP_AUDIOIN_ADCVOL_MUTE                      24
#define BM_AUDIOIN_ADCVOL_MUTE                      0x1000000
#define BF_AUDIOIN_ADCVOL_MUTE(v)                   (((v) << 24) & 0x1000000)
#define BP_AUDIOIN_ADCVOL_RSRVD2                    14
#define BM_AUDIOIN_ADCVOL_RSRVD2                    0xffc000
#define BF_AUDIOIN_ADCVOL_RSRVD2(v)                 (((v) << 14) & 0xffc000)
#define BP_AUDIOIN_ADCVOL_SELECT_LEFT               12
#define BM_AUDIOIN_ADCVOL_SELECT_LEFT               0x3000
#define BF_AUDIOIN_ADCVOL_SELECT_LEFT(v)            (((v) << 12) & 0x3000)
#define BP_AUDIOIN_ADCVOL_GAIN_LEFT                 8
#define BM_AUDIOIN_ADCVOL_GAIN_LEFT                 0xf00
#define BF_AUDIOIN_ADCVOL_GAIN_LEFT(v)              (((v) << 8) & 0xf00)
#define BP_AUDIOIN_ADCVOL_RSRVD1                    6
#define BM_AUDIOIN_ADCVOL_RSRVD1                    0xc0
#define BF_AUDIOIN_ADCVOL_RSRVD1(v)                 (((v) << 6) & 0xc0)
#define BP_AUDIOIN_ADCVOL_SELECT_RIGHT              4
#define BM_AUDIOIN_ADCVOL_SELECT_RIGHT              0x30
#define BF_AUDIOIN_ADCVOL_SELECT_RIGHT(v)           (((v) << 4) & 0x30)
#define BP_AUDIOIN_ADCVOL_GAIN_RIGHT                0
#define BM_AUDIOIN_ADCVOL_GAIN_RIGHT                0xf
#define BF_AUDIOIN_ADCVOL_GAIN_RIGHT(v)             (((v) << 0) & 0xf)

/**
 * Register: HW_AUDIOIN_MICLINE
 * Address: 0x60
 * SCT: yes
*/
#define HW_AUDIOIN_MICLINE                      (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x60 + 0x0))
#define HW_AUDIOIN_MICLINE_SET                  (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x60 + 0x4))
#define HW_AUDIOIN_MICLINE_CLR                  (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x60 + 0x8))
#define HW_AUDIOIN_MICLINE_TOG                  (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x60 + 0xc))
#define BP_AUDIOIN_MICLINE_RSRVD6               30
#define BM_AUDIOIN_MICLINE_RSRVD6               0xc0000000
#define BF_AUDIOIN_MICLINE_RSRVD6(v)            (((v) << 30) & 0xc0000000)
#define BP_AUDIOIN_MICLINE_DIVIDE_LINE1         29
#define BM_AUDIOIN_MICLINE_DIVIDE_LINE1         0x20000000
#define BF_AUDIOIN_MICLINE_DIVIDE_LINE1(v)      (((v) << 29) & 0x20000000)
#define BP_AUDIOIN_MICLINE_DIVIDE_LINE2         28
#define BM_AUDIOIN_MICLINE_DIVIDE_LINE2         0x10000000
#define BF_AUDIOIN_MICLINE_DIVIDE_LINE2(v)      (((v) << 28) & 0x10000000)
#define BP_AUDIOIN_MICLINE_RSRVD5               25
#define BM_AUDIOIN_MICLINE_RSRVD5               0xe000000
#define BF_AUDIOIN_MICLINE_RSRVD5(v)            (((v) << 25) & 0xe000000)
#define BP_AUDIOIN_MICLINE_MIC_SELECT           24
#define BM_AUDIOIN_MICLINE_MIC_SELECT           0x1000000
#define BF_AUDIOIN_MICLINE_MIC_SELECT(v)        (((v) << 24) & 0x1000000)
#define BP_AUDIOIN_MICLINE_RSRVD4               22
#define BM_AUDIOIN_MICLINE_RSRVD4               0xc00000
#define BF_AUDIOIN_MICLINE_RSRVD4(v)            (((v) << 22) & 0xc00000)
#define BP_AUDIOIN_MICLINE_MIC_RESISTOR         20
#define BM_AUDIOIN_MICLINE_MIC_RESISTOR         0x300000
#define BV_AUDIOIN_MICLINE_MIC_RESISTOR__Off    0x0
#define BV_AUDIOIN_MICLINE_MIC_RESISTOR__2KOhm  0x1
#define BV_AUDIOIN_MICLINE_MIC_RESISTOR__4KOhm  0x2
#define BV_AUDIOIN_MICLINE_MIC_RESISTOR__8KOhm  0x3
#define BF_AUDIOIN_MICLINE_MIC_RESISTOR(v)      (((v) << 20) & 0x300000)
#define BF_AUDIOIN_MICLINE_MIC_RESISTOR_V(v)    ((BV_AUDIOIN_MICLINE_MIC_RESISTOR__##v << 20) & 0x300000)
#define BP_AUDIOIN_MICLINE_RSRVD3               19
#define BM_AUDIOIN_MICLINE_RSRVD3               0x80000
#define BF_AUDIOIN_MICLINE_RSRVD3(v)            (((v) << 19) & 0x80000)
#define BP_AUDIOIN_MICLINE_MIC_BIAS             16
#define BM_AUDIOIN_MICLINE_MIC_BIAS             0x70000
#define BF_AUDIOIN_MICLINE_MIC_BIAS(v)          (((v) << 16) & 0x70000)
#define BP_AUDIOIN_MICLINE_RSRVD2               6
#define BM_AUDIOIN_MICLINE_RSRVD2               0xffc0
#define BF_AUDIOIN_MICLINE_RSRVD2(v)            (((v) << 6) & 0xffc0)
#define BP_AUDIOIN_MICLINE_MIC_CHOPCLK          4
#define BM_AUDIOIN_MICLINE_MIC_CHOPCLK          0x30
#define BF_AUDIOIN_MICLINE_MIC_CHOPCLK(v)       (((v) << 4) & 0x30)
#define BP_AUDIOIN_MICLINE_RSRVD1               2
#define BM_AUDIOIN_MICLINE_RSRVD1               0xc
#define BF_AUDIOIN_MICLINE_RSRVD1(v)            (((v) << 2) & 0xc)
#define BP_AUDIOIN_MICLINE_MIC_GAIN             0
#define BM_AUDIOIN_MICLINE_MIC_GAIN             0x3
#define BV_AUDIOIN_MICLINE_MIC_GAIN__0dB        0x0
#define BV_AUDIOIN_MICLINE_MIC_GAIN__20dB       0x1
#define BV_AUDIOIN_MICLINE_MIC_GAIN__30dB       0x2
#define BV_AUDIOIN_MICLINE_MIC_GAIN__40dB       0x3
#define BF_AUDIOIN_MICLINE_MIC_GAIN(v)          (((v) << 0) & 0x3)
#define BF_AUDIOIN_MICLINE_MIC_GAIN_V(v)        ((BV_AUDIOIN_MICLINE_MIC_GAIN__##v << 0) & 0x3)

/**
 * Register: HW_AUDIOIN_ANACLKCTRL
 * Address: 0x70
 * SCT: yes
*/
#define HW_AUDIOIN_ANACLKCTRL                   (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x70 + 0x0))
#define HW_AUDIOIN_ANACLKCTRL_SET               (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x70 + 0x4))
#define HW_AUDIOIN_ANACLKCTRL_CLR               (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x70 + 0x8))
#define HW_AUDIOIN_ANACLKCTRL_TOG               (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x70 + 0xc))
#define BP_AUDIOIN_ANACLKCTRL_CLKGATE           31
#define BM_AUDIOIN_ANACLKCTRL_CLKGATE           0x80000000
#define BF_AUDIOIN_ANACLKCTRL_CLKGATE(v)        (((v) << 31) & 0x80000000)
#define BP_AUDIOIN_ANACLKCTRL_RSRVD4            11
#define BM_AUDIOIN_ANACLKCTRL_RSRVD4            0x7ffff800
#define BF_AUDIOIN_ANACLKCTRL_RSRVD4(v)         (((v) << 11) & 0x7ffff800)
#define BP_AUDIOIN_ANACLKCTRL_DITHER_OFF        10
#define BM_AUDIOIN_ANACLKCTRL_DITHER_OFF        0x400
#define BF_AUDIOIN_ANACLKCTRL_DITHER_OFF(v)     (((v) << 10) & 0x400)
#define BP_AUDIOIN_ANACLKCTRL_SLOW_DITHER       9
#define BM_AUDIOIN_ANACLKCTRL_SLOW_DITHER       0x200
#define BF_AUDIOIN_ANACLKCTRL_SLOW_DITHER(v)    (((v) << 9) & 0x200)
#define BP_AUDIOIN_ANACLKCTRL_INVERT_ADCCLK     8
#define BM_AUDIOIN_ANACLKCTRL_INVERT_ADCCLK     0x100
#define BF_AUDIOIN_ANACLKCTRL_INVERT_ADCCLK(v)  (((v) << 8) & 0x100)
#define BP_AUDIOIN_ANACLKCTRL_RSRVD3            6
#define BM_AUDIOIN_ANACLKCTRL_RSRVD3            0xc0
#define BF_AUDIOIN_ANACLKCTRL_RSRVD3(v)         (((v) << 6) & 0xc0)
#define BP_AUDIOIN_ANACLKCTRL_ADCCLK_SHIFT      4
#define BM_AUDIOIN_ANACLKCTRL_ADCCLK_SHIFT      0x30
#define BF_AUDIOIN_ANACLKCTRL_ADCCLK_SHIFT(v)   (((v) << 4) & 0x30)
#define BP_AUDIOIN_ANACLKCTRL_RSRVD2            3
#define BM_AUDIOIN_ANACLKCTRL_RSRVD2            0x8
#define BF_AUDIOIN_ANACLKCTRL_RSRVD2(v)         (((v) << 3) & 0x8)
#define BP_AUDIOIN_ANACLKCTRL_ADCDIV            0
#define BM_AUDIOIN_ANACLKCTRL_ADCDIV            0x7
#define BF_AUDIOIN_ANACLKCTRL_ADCDIV(v)         (((v) << 0) & 0x7)

/**
 * Register: HW_AUDIOIN_DATA
 * Address: 0x80
 * SCT: yes
*/
#define HW_AUDIOIN_DATA         (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x80 + 0x0))
#define HW_AUDIOIN_DATA_SET     (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x80 + 0x4))
#define HW_AUDIOIN_DATA_CLR     (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x80 + 0x8))
#define HW_AUDIOIN_DATA_TOG     (*(volatile unsigned long *)(REGS_AUDIOIN_BASE + 0x80 + 0xc))
#define BP_AUDIOIN_DATA_HIGH    16
#define BM_AUDIOIN_DATA_HIGH    0xffff0000
#define BF_AUDIOIN_DATA_HIGH(v) (((v) << 16) & 0xffff0000)
#define BP_AUDIOIN_DATA_LOW     0
#define BM_AUDIOIN_DATA_LOW     0xffff
#define BF_AUDIOIN_DATA_LOW(v)  (((v) << 0) & 0xffff)

#endif /* __HEADERGEN__IMX233__AUDIOIN__H__ */

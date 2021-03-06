/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * This file was automatically generated by headergen, DO NOT EDIT it.
 * headergen version: 3.0.0
 * imx233 version: 2.4.0
 * imx233 authors: Amaury Pouly
 *
 * Copyright (C) 2015 by the authors
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
#ifndef __HEADERGEN_IMX233_APBX_H__
#define __HEADERGEN_IMX233_APBX_H__

#define HW_APBX_CTRL0               HW(APBX_CTRL0)
#define HWA_APBX_CTRL0              (0x80024000 + 0x0)
#define HWT_APBX_CTRL0              HWIO_32_RW
#define HWN_APBX_CTRL0              APBX_CTRL0
#define HWI_APBX_CTRL0              
#define HW_APBX_CTRL0_SET           HW(APBX_CTRL0_SET)
#define HWA_APBX_CTRL0_SET          (HWA_APBX_CTRL0 + 0x4)
#define HWT_APBX_CTRL0_SET          HWIO_32_WO
#define HWN_APBX_CTRL0_SET          APBX_CTRL0
#define HWI_APBX_CTRL0_SET          
#define HW_APBX_CTRL0_CLR           HW(APBX_CTRL0_CLR)
#define HWA_APBX_CTRL0_CLR          (HWA_APBX_CTRL0 + 0x8)
#define HWT_APBX_CTRL0_CLR          HWIO_32_WO
#define HWN_APBX_CTRL0_CLR          APBX_CTRL0
#define HWI_APBX_CTRL0_CLR          
#define HW_APBX_CTRL0_TOG           HW(APBX_CTRL0_TOG)
#define HWA_APBX_CTRL0_TOG          (HWA_APBX_CTRL0 + 0xc)
#define HWT_APBX_CTRL0_TOG          HWIO_32_WO
#define HWN_APBX_CTRL0_TOG          APBX_CTRL0
#define HWI_APBX_CTRL0_TOG          
#define BP_APBX_CTRL0_SFTRST        31
#define BM_APBX_CTRL0_SFTRST        0x80000000
#define BF_APBX_CTRL0_SFTRST(v)     (((v) & 0x1) << 31)
#define BFM_APBX_CTRL0_SFTRST(v)    BM_APBX_CTRL0_SFTRST
#define BF_APBX_CTRL0_SFTRST_V(e)   BF_APBX_CTRL0_SFTRST(BV_APBX_CTRL0_SFTRST__##e)
#define BFM_APBX_CTRL0_SFTRST_V(v)  BM_APBX_CTRL0_SFTRST
#define BP_APBX_CTRL0_CLKGATE       30
#define BM_APBX_CTRL0_CLKGATE       0x40000000
#define BF_APBX_CTRL0_CLKGATE(v)    (((v) & 0x1) << 30)
#define BFM_APBX_CTRL0_CLKGATE(v)   BM_APBX_CTRL0_CLKGATE
#define BF_APBX_CTRL0_CLKGATE_V(e)  BF_APBX_CTRL0_CLKGATE(BV_APBX_CTRL0_CLKGATE__##e)
#define BFM_APBX_CTRL0_CLKGATE_V(v) BM_APBX_CTRL0_CLKGATE
#define BP_APBX_CTRL0_RSVD0         0
#define BM_APBX_CTRL0_RSVD0         0x3fffffff
#define BF_APBX_CTRL0_RSVD0(v)      (((v) & 0x3fffffff) << 0)
#define BFM_APBX_CTRL0_RSVD0(v)     BM_APBX_CTRL0_RSVD0
#define BF_APBX_CTRL0_RSVD0_V(e)    BF_APBX_CTRL0_RSVD0(BV_APBX_CTRL0_RSVD0__##e)
#define BFM_APBX_CTRL0_RSVD0_V(v)   BM_APBX_CTRL0_RSVD0

#define HW_APBX_CTRL1                           HW(APBX_CTRL1)
#define HWA_APBX_CTRL1                          (0x80024000 + 0x10)
#define HWT_APBX_CTRL1                          HWIO_32_RW
#define HWN_APBX_CTRL1                          APBX_CTRL1
#define HWI_APBX_CTRL1                          
#define HW_APBX_CTRL1_SET                       HW(APBX_CTRL1_SET)
#define HWA_APBX_CTRL1_SET                      (HWA_APBX_CTRL1 + 0x4)
#define HWT_APBX_CTRL1_SET                      HWIO_32_WO
#define HWN_APBX_CTRL1_SET                      APBX_CTRL1
#define HWI_APBX_CTRL1_SET                      
#define HW_APBX_CTRL1_CLR                       HW(APBX_CTRL1_CLR)
#define HWA_APBX_CTRL1_CLR                      (HWA_APBX_CTRL1 + 0x8)
#define HWT_APBX_CTRL1_CLR                      HWIO_32_WO
#define HWN_APBX_CTRL1_CLR                      APBX_CTRL1
#define HWI_APBX_CTRL1_CLR                      
#define HW_APBX_CTRL1_TOG                       HW(APBX_CTRL1_TOG)
#define HWA_APBX_CTRL1_TOG                      (HWA_APBX_CTRL1 + 0xc)
#define HWT_APBX_CTRL1_TOG                      HWIO_32_WO
#define HWN_APBX_CTRL1_TOG                      APBX_CTRL1
#define HWI_APBX_CTRL1_TOG                      
#define BP_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN        16
#define BM_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN        0xffff0000
#define BF_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN(v)     (((v) & 0xffff) << 16)
#define BFM_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN(v)    BM_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN
#define BF_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN_V(e)   BF_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN(BV_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN__##e)
#define BFM_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN_V(v)  BM_APBX_CTRL1_CH_CMDCMPLT_IRQ_EN
#define BP_APBX_CTRL1_CH_CMDCMPLT_IRQ           0
#define BM_APBX_CTRL1_CH_CMDCMPLT_IRQ           0xffff
#define BF_APBX_CTRL1_CH_CMDCMPLT_IRQ(v)        (((v) & 0xffff) << 0)
#define BFM_APBX_CTRL1_CH_CMDCMPLT_IRQ(v)       BM_APBX_CTRL1_CH_CMDCMPLT_IRQ
#define BF_APBX_CTRL1_CH_CMDCMPLT_IRQ_V(e)      BF_APBX_CTRL1_CH_CMDCMPLT_IRQ(BV_APBX_CTRL1_CH_CMDCMPLT_IRQ__##e)
#define BFM_APBX_CTRL1_CH_CMDCMPLT_IRQ_V(v)     BM_APBX_CTRL1_CH_CMDCMPLT_IRQ

#define HW_APBX_CTRL2                       HW(APBX_CTRL2)
#define HWA_APBX_CTRL2                      (0x80024000 + 0x20)
#define HWT_APBX_CTRL2                      HWIO_32_RW
#define HWN_APBX_CTRL2                      APBX_CTRL2
#define HWI_APBX_CTRL2                      
#define HW_APBX_CTRL2_SET                   HW(APBX_CTRL2_SET)
#define HWA_APBX_CTRL2_SET                  (HWA_APBX_CTRL2 + 0x4)
#define HWT_APBX_CTRL2_SET                  HWIO_32_WO
#define HWN_APBX_CTRL2_SET                  APBX_CTRL2
#define HWI_APBX_CTRL2_SET                  
#define HW_APBX_CTRL2_CLR                   HW(APBX_CTRL2_CLR)
#define HWA_APBX_CTRL2_CLR                  (HWA_APBX_CTRL2 + 0x8)
#define HWT_APBX_CTRL2_CLR                  HWIO_32_WO
#define HWN_APBX_CTRL2_CLR                  APBX_CTRL2
#define HWI_APBX_CTRL2_CLR                  
#define HW_APBX_CTRL2_TOG                   HW(APBX_CTRL2_TOG)
#define HWA_APBX_CTRL2_TOG                  (HWA_APBX_CTRL2 + 0xc)
#define HWT_APBX_CTRL2_TOG                  HWIO_32_WO
#define HWN_APBX_CTRL2_TOG                  APBX_CTRL2
#define HWI_APBX_CTRL2_TOG                  
#define BP_APBX_CTRL2_CH_ERROR_STATUS       16
#define BM_APBX_CTRL2_CH_ERROR_STATUS       0xffff0000
#define BF_APBX_CTRL2_CH_ERROR_STATUS(v)    (((v) & 0xffff) << 16)
#define BFM_APBX_CTRL2_CH_ERROR_STATUS(v)   BM_APBX_CTRL2_CH_ERROR_STATUS
#define BF_APBX_CTRL2_CH_ERROR_STATUS_V(e)  BF_APBX_CTRL2_CH_ERROR_STATUS(BV_APBX_CTRL2_CH_ERROR_STATUS__##e)
#define BFM_APBX_CTRL2_CH_ERROR_STATUS_V(v) BM_APBX_CTRL2_CH_ERROR_STATUS
#define BP_APBX_CTRL2_CH_ERROR_IRQ          0
#define BM_APBX_CTRL2_CH_ERROR_IRQ          0xffff
#define BF_APBX_CTRL2_CH_ERROR_IRQ(v)       (((v) & 0xffff) << 0)
#define BFM_APBX_CTRL2_CH_ERROR_IRQ(v)      BM_APBX_CTRL2_CH_ERROR_IRQ
#define BF_APBX_CTRL2_CH_ERROR_IRQ_V(e)     BF_APBX_CTRL2_CH_ERROR_IRQ(BV_APBX_CTRL2_CH_ERROR_IRQ__##e)
#define BFM_APBX_CTRL2_CH_ERROR_IRQ_V(v)    BM_APBX_CTRL2_CH_ERROR_IRQ

#define HW_APBX_CHANNEL_CTRL                            HW(APBX_CHANNEL_CTRL)
#define HWA_APBX_CHANNEL_CTRL                           (0x80024000 + 0x30)
#define HWT_APBX_CHANNEL_CTRL                           HWIO_32_RW
#define HWN_APBX_CHANNEL_CTRL                           APBX_CHANNEL_CTRL
#define HWI_APBX_CHANNEL_CTRL                           
#define HW_APBX_CHANNEL_CTRL_SET                        HW(APBX_CHANNEL_CTRL_SET)
#define HWA_APBX_CHANNEL_CTRL_SET                       (HWA_APBX_CHANNEL_CTRL + 0x4)
#define HWT_APBX_CHANNEL_CTRL_SET                       HWIO_32_WO
#define HWN_APBX_CHANNEL_CTRL_SET                       APBX_CHANNEL_CTRL
#define HWI_APBX_CHANNEL_CTRL_SET                       
#define HW_APBX_CHANNEL_CTRL_CLR                        HW(APBX_CHANNEL_CTRL_CLR)
#define HWA_APBX_CHANNEL_CTRL_CLR                       (HWA_APBX_CHANNEL_CTRL + 0x8)
#define HWT_APBX_CHANNEL_CTRL_CLR                       HWIO_32_WO
#define HWN_APBX_CHANNEL_CTRL_CLR                       APBX_CHANNEL_CTRL
#define HWI_APBX_CHANNEL_CTRL_CLR                       
#define HW_APBX_CHANNEL_CTRL_TOG                        HW(APBX_CHANNEL_CTRL_TOG)
#define HWA_APBX_CHANNEL_CTRL_TOG                       (HWA_APBX_CHANNEL_CTRL + 0xc)
#define HWT_APBX_CHANNEL_CTRL_TOG                       HWIO_32_WO
#define HWN_APBX_CHANNEL_CTRL_TOG                       APBX_CHANNEL_CTRL
#define HWI_APBX_CHANNEL_CTRL_TOG                       
#define BP_APBX_CHANNEL_CTRL_RESET_CHANNEL              16
#define BM_APBX_CHANNEL_CTRL_RESET_CHANNEL              0xffff0000
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__AUDIOIN     0x1
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__AUDIOOUT    0x2
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__SPDIF_TX    0x4
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__I2C         0x8
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__SAIF1       0x10
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__DRI         0x20
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__IRDA_RX     0x40
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__UART0_RX    0x40
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__IRDA_TX     0x80
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__UART0_TX    0x80
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__UART1_RX    0x100
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__UART1_TX    0x200
#define BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__SAIF2       0x400
#define BF_APBX_CHANNEL_CTRL_RESET_CHANNEL(v)           (((v) & 0xffff) << 16)
#define BFM_APBX_CHANNEL_CTRL_RESET_CHANNEL(v)          BM_APBX_CHANNEL_CTRL_RESET_CHANNEL
#define BF_APBX_CHANNEL_CTRL_RESET_CHANNEL_V(e)         BF_APBX_CHANNEL_CTRL_RESET_CHANNEL(BV_APBX_CHANNEL_CTRL_RESET_CHANNEL__##e)
#define BFM_APBX_CHANNEL_CTRL_RESET_CHANNEL_V(v)        BM_APBX_CHANNEL_CTRL_RESET_CHANNEL
#define BP_APBX_CHANNEL_CTRL_FREEZE_CHANNEL             0
#define BM_APBX_CHANNEL_CTRL_FREEZE_CHANNEL             0xffff
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__AUDIOIN    0x1
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__AUDIOOUT   0x2
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__SPDIF_TX   0x4
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__I2C        0x8
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__SAIF1      0x10
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__DRI        0x20
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__IRDA_RX    0x40
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__UART0_RX   0x40
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__IRDA_TX    0x80
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__UART0_TX   0x80
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__UART1_RX   0x100
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__UART1_TX   0x200
#define BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__SAIF2      0x400
#define BF_APBX_CHANNEL_CTRL_FREEZE_CHANNEL(v)          (((v) & 0xffff) << 0)
#define BFM_APBX_CHANNEL_CTRL_FREEZE_CHANNEL(v)         BM_APBX_CHANNEL_CTRL_FREEZE_CHANNEL
#define BF_APBX_CHANNEL_CTRL_FREEZE_CHANNEL_V(e)        BF_APBX_CHANNEL_CTRL_FREEZE_CHANNEL(BV_APBX_CHANNEL_CTRL_FREEZE_CHANNEL__##e)
#define BFM_APBX_CHANNEL_CTRL_FREEZE_CHANNEL_V(v)       BM_APBX_CHANNEL_CTRL_FREEZE_CHANNEL

#define HW_APBX_DEVSEL                  HW(APBX_DEVSEL)
#define HWA_APBX_DEVSEL                 (0x80024000 + 0x40)
#define HWT_APBX_DEVSEL                 HWIO_32_RW
#define HWN_APBX_DEVSEL                 APBX_DEVSEL
#define HWI_APBX_DEVSEL                 
#define BP_APBX_DEVSEL_CH15             30
#define BM_APBX_DEVSEL_CH15             0xc0000000
#define BF_APBX_DEVSEL_CH15(v)          (((v) & 0x3) << 30)
#define BFM_APBX_DEVSEL_CH15(v)         BM_APBX_DEVSEL_CH15
#define BF_APBX_DEVSEL_CH15_V(e)        BF_APBX_DEVSEL_CH15(BV_APBX_DEVSEL_CH15__##e)
#define BFM_APBX_DEVSEL_CH15_V(v)       BM_APBX_DEVSEL_CH15
#define BP_APBX_DEVSEL_CH14             28
#define BM_APBX_DEVSEL_CH14             0x30000000
#define BF_APBX_DEVSEL_CH14(v)          (((v) & 0x3) << 28)
#define BFM_APBX_DEVSEL_CH14(v)         BM_APBX_DEVSEL_CH14
#define BF_APBX_DEVSEL_CH14_V(e)        BF_APBX_DEVSEL_CH14(BV_APBX_DEVSEL_CH14__##e)
#define BFM_APBX_DEVSEL_CH14_V(v)       BM_APBX_DEVSEL_CH14
#define BP_APBX_DEVSEL_CH13             26
#define BM_APBX_DEVSEL_CH13             0xc000000
#define BF_APBX_DEVSEL_CH13(v)          (((v) & 0x3) << 26)
#define BFM_APBX_DEVSEL_CH13(v)         BM_APBX_DEVSEL_CH13
#define BF_APBX_DEVSEL_CH13_V(e)        BF_APBX_DEVSEL_CH13(BV_APBX_DEVSEL_CH13__##e)
#define BFM_APBX_DEVSEL_CH13_V(v)       BM_APBX_DEVSEL_CH13
#define BP_APBX_DEVSEL_CH12             24
#define BM_APBX_DEVSEL_CH12             0x3000000
#define BF_APBX_DEVSEL_CH12(v)          (((v) & 0x3) << 24)
#define BFM_APBX_DEVSEL_CH12(v)         BM_APBX_DEVSEL_CH12
#define BF_APBX_DEVSEL_CH12_V(e)        BF_APBX_DEVSEL_CH12(BV_APBX_DEVSEL_CH12__##e)
#define BFM_APBX_DEVSEL_CH12_V(v)       BM_APBX_DEVSEL_CH12
#define BP_APBX_DEVSEL_CH11             22
#define BM_APBX_DEVSEL_CH11             0xc00000
#define BF_APBX_DEVSEL_CH11(v)          (((v) & 0x3) << 22)
#define BFM_APBX_DEVSEL_CH11(v)         BM_APBX_DEVSEL_CH11
#define BF_APBX_DEVSEL_CH11_V(e)        BF_APBX_DEVSEL_CH11(BV_APBX_DEVSEL_CH11__##e)
#define BFM_APBX_DEVSEL_CH11_V(v)       BM_APBX_DEVSEL_CH11
#define BP_APBX_DEVSEL_CH10             20
#define BM_APBX_DEVSEL_CH10             0x300000
#define BF_APBX_DEVSEL_CH10(v)          (((v) & 0x3) << 20)
#define BFM_APBX_DEVSEL_CH10(v)         BM_APBX_DEVSEL_CH10
#define BF_APBX_DEVSEL_CH10_V(e)        BF_APBX_DEVSEL_CH10(BV_APBX_DEVSEL_CH10__##e)
#define BFM_APBX_DEVSEL_CH10_V(v)       BM_APBX_DEVSEL_CH10
#define BP_APBX_DEVSEL_CH9              18
#define BM_APBX_DEVSEL_CH9              0xc0000
#define BF_APBX_DEVSEL_CH9(v)           (((v) & 0x3) << 18)
#define BFM_APBX_DEVSEL_CH9(v)          BM_APBX_DEVSEL_CH9
#define BF_APBX_DEVSEL_CH9_V(e)         BF_APBX_DEVSEL_CH9(BV_APBX_DEVSEL_CH9__##e)
#define BFM_APBX_DEVSEL_CH9_V(v)        BM_APBX_DEVSEL_CH9
#define BP_APBX_DEVSEL_CH8              16
#define BM_APBX_DEVSEL_CH8              0x30000
#define BF_APBX_DEVSEL_CH8(v)           (((v) & 0x3) << 16)
#define BFM_APBX_DEVSEL_CH8(v)          BM_APBX_DEVSEL_CH8
#define BF_APBX_DEVSEL_CH8_V(e)         BF_APBX_DEVSEL_CH8(BV_APBX_DEVSEL_CH8__##e)
#define BFM_APBX_DEVSEL_CH8_V(v)        BM_APBX_DEVSEL_CH8
#define BP_APBX_DEVSEL_CH7              14
#define BM_APBX_DEVSEL_CH7              0xc000
#define BV_APBX_DEVSEL_CH7__USE_I2C1    0x0
#define BV_APBX_DEVSEL_CH7__USE_IRDA    0x1
#define BF_APBX_DEVSEL_CH7(v)           (((v) & 0x3) << 14)
#define BFM_APBX_DEVSEL_CH7(v)          BM_APBX_DEVSEL_CH7
#define BF_APBX_DEVSEL_CH7_V(e)         BF_APBX_DEVSEL_CH7(BV_APBX_DEVSEL_CH7__##e)
#define BFM_APBX_DEVSEL_CH7_V(v)        BM_APBX_DEVSEL_CH7
#define BP_APBX_DEVSEL_CH6              12
#define BM_APBX_DEVSEL_CH6              0x3000
#define BV_APBX_DEVSEL_CH6__USE_SAIF1   0x0
#define BV_APBX_DEVSEL_CH6__USE_IRDA    0x1
#define BF_APBX_DEVSEL_CH6(v)           (((v) & 0x3) << 12)
#define BFM_APBX_DEVSEL_CH6(v)          BM_APBX_DEVSEL_CH6
#define BF_APBX_DEVSEL_CH6_V(e)         BF_APBX_DEVSEL_CH6(BV_APBX_DEVSEL_CH6__##e)
#define BFM_APBX_DEVSEL_CH6_V(v)        BM_APBX_DEVSEL_CH6
#define BP_APBX_DEVSEL_CH5              10
#define BM_APBX_DEVSEL_CH5              0xc00
#define BF_APBX_DEVSEL_CH5(v)           (((v) & 0x3) << 10)
#define BFM_APBX_DEVSEL_CH5(v)          BM_APBX_DEVSEL_CH5
#define BF_APBX_DEVSEL_CH5_V(e)         BF_APBX_DEVSEL_CH5(BV_APBX_DEVSEL_CH5__##e)
#define BFM_APBX_DEVSEL_CH5_V(v)        BM_APBX_DEVSEL_CH5
#define BP_APBX_DEVSEL_CH4              8
#define BM_APBX_DEVSEL_CH4              0x300
#define BF_APBX_DEVSEL_CH4(v)           (((v) & 0x3) << 8)
#define BFM_APBX_DEVSEL_CH4(v)          BM_APBX_DEVSEL_CH4
#define BF_APBX_DEVSEL_CH4_V(e)         BF_APBX_DEVSEL_CH4(BV_APBX_DEVSEL_CH4__##e)
#define BFM_APBX_DEVSEL_CH4_V(v)        BM_APBX_DEVSEL_CH4
#define BP_APBX_DEVSEL_CH3              6
#define BM_APBX_DEVSEL_CH3              0xc0
#define BF_APBX_DEVSEL_CH3(v)           (((v) & 0x3) << 6)
#define BFM_APBX_DEVSEL_CH3(v)          BM_APBX_DEVSEL_CH3
#define BF_APBX_DEVSEL_CH3_V(e)         BF_APBX_DEVSEL_CH3(BV_APBX_DEVSEL_CH3__##e)
#define BFM_APBX_DEVSEL_CH3_V(v)        BM_APBX_DEVSEL_CH3
#define BP_APBX_DEVSEL_CH2              4
#define BM_APBX_DEVSEL_CH2              0x30
#define BF_APBX_DEVSEL_CH2(v)           (((v) & 0x3) << 4)
#define BFM_APBX_DEVSEL_CH2(v)          BM_APBX_DEVSEL_CH2
#define BF_APBX_DEVSEL_CH2_V(e)         BF_APBX_DEVSEL_CH2(BV_APBX_DEVSEL_CH2__##e)
#define BFM_APBX_DEVSEL_CH2_V(v)        BM_APBX_DEVSEL_CH2
#define BP_APBX_DEVSEL_CH1              2
#define BM_APBX_DEVSEL_CH1              0xc
#define BF_APBX_DEVSEL_CH1(v)           (((v) & 0x3) << 2)
#define BFM_APBX_DEVSEL_CH1(v)          BM_APBX_DEVSEL_CH1
#define BF_APBX_DEVSEL_CH1_V(e)         BF_APBX_DEVSEL_CH1(BV_APBX_DEVSEL_CH1__##e)
#define BFM_APBX_DEVSEL_CH1_V(v)        BM_APBX_DEVSEL_CH1
#define BP_APBX_DEVSEL_CH0              0
#define BM_APBX_DEVSEL_CH0              0x3
#define BF_APBX_DEVSEL_CH0(v)           (((v) & 0x3) << 0)
#define BFM_APBX_DEVSEL_CH0(v)          BM_APBX_DEVSEL_CH0
#define BF_APBX_DEVSEL_CH0_V(e)         BF_APBX_DEVSEL_CH0(BV_APBX_DEVSEL_CH0__##e)
#define BFM_APBX_DEVSEL_CH0_V(v)        BM_APBX_DEVSEL_CH0

#define HW_APBX_CHn_CURCMDAR(_n1)           HW(APBX_CHn_CURCMDAR(_n1))
#define HWA_APBX_CHn_CURCMDAR(_n1)          (0x80024000 + 0x100 + (_n1) * 0x70)
#define HWT_APBX_CHn_CURCMDAR(_n1)          HWIO_32_RW
#define HWN_APBX_CHn_CURCMDAR(_n1)          APBX_CHn_CURCMDAR
#define HWI_APBX_CHn_CURCMDAR(_n1)          (_n1)
#define BP_APBX_CHn_CURCMDAR_CMD_ADDR       0
#define BM_APBX_CHn_CURCMDAR_CMD_ADDR       0xffffffff
#define BF_APBX_CHn_CURCMDAR_CMD_ADDR(v)    (((v) & 0xffffffff) << 0)
#define BFM_APBX_CHn_CURCMDAR_CMD_ADDR(v)   BM_APBX_CHn_CURCMDAR_CMD_ADDR
#define BF_APBX_CHn_CURCMDAR_CMD_ADDR_V(e)  BF_APBX_CHn_CURCMDAR_CMD_ADDR(BV_APBX_CHn_CURCMDAR_CMD_ADDR__##e)
#define BFM_APBX_CHn_CURCMDAR_CMD_ADDR_V(v) BM_APBX_CHn_CURCMDAR_CMD_ADDR

#define HW_APBX_CHn_NXTCMDAR(_n1)           HW(APBX_CHn_NXTCMDAR(_n1))
#define HWA_APBX_CHn_NXTCMDAR(_n1)          (0x80024000 + 0x110 + (_n1) * 0x70)
#define HWT_APBX_CHn_NXTCMDAR(_n1)          HWIO_32_RW
#define HWN_APBX_CHn_NXTCMDAR(_n1)          APBX_CHn_NXTCMDAR
#define HWI_APBX_CHn_NXTCMDAR(_n1)          (_n1)
#define BP_APBX_CHn_NXTCMDAR_CMD_ADDR       0
#define BM_APBX_CHn_NXTCMDAR_CMD_ADDR       0xffffffff
#define BF_APBX_CHn_NXTCMDAR_CMD_ADDR(v)    (((v) & 0xffffffff) << 0)
#define BFM_APBX_CHn_NXTCMDAR_CMD_ADDR(v)   BM_APBX_CHn_NXTCMDAR_CMD_ADDR
#define BF_APBX_CHn_NXTCMDAR_CMD_ADDR_V(e)  BF_APBX_CHn_NXTCMDAR_CMD_ADDR(BV_APBX_CHn_NXTCMDAR_CMD_ADDR__##e)
#define BFM_APBX_CHn_NXTCMDAR_CMD_ADDR_V(v) BM_APBX_CHn_NXTCMDAR_CMD_ADDR

#define HW_APBX_CHn_CMD(_n1)                    HW(APBX_CHn_CMD(_n1))
#define HWA_APBX_CHn_CMD(_n1)                   (0x80024000 + 0x120 + (_n1) * 0x70)
#define HWT_APBX_CHn_CMD(_n1)                   HWIO_32_RW
#define HWN_APBX_CHn_CMD(_n1)                   APBX_CHn_CMD
#define HWI_APBX_CHn_CMD(_n1)                   (_n1)
#define BP_APBX_CHn_CMD_XFER_COUNT              16
#define BM_APBX_CHn_CMD_XFER_COUNT              0xffff0000
#define BF_APBX_CHn_CMD_XFER_COUNT(v)           (((v) & 0xffff) << 16)
#define BFM_APBX_CHn_CMD_XFER_COUNT(v)          BM_APBX_CHn_CMD_XFER_COUNT
#define BF_APBX_CHn_CMD_XFER_COUNT_V(e)         BF_APBX_CHn_CMD_XFER_COUNT(BV_APBX_CHn_CMD_XFER_COUNT__##e)
#define BFM_APBX_CHn_CMD_XFER_COUNT_V(v)        BM_APBX_CHn_CMD_XFER_COUNT
#define BP_APBX_CHn_CMD_CMDWORDS                12
#define BM_APBX_CHn_CMD_CMDWORDS                0xf000
#define BF_APBX_CHn_CMD_CMDWORDS(v)             (((v) & 0xf) << 12)
#define BFM_APBX_CHn_CMD_CMDWORDS(v)            BM_APBX_CHn_CMD_CMDWORDS
#define BF_APBX_CHn_CMD_CMDWORDS_V(e)           BF_APBX_CHn_CMD_CMDWORDS(BV_APBX_CHn_CMD_CMDWORDS__##e)
#define BFM_APBX_CHn_CMD_CMDWORDS_V(v)          BM_APBX_CHn_CMD_CMDWORDS
#define BP_APBX_CHn_CMD_RSVD1                   9
#define BM_APBX_CHn_CMD_RSVD1                   0xe00
#define BF_APBX_CHn_CMD_RSVD1(v)                (((v) & 0x7) << 9)
#define BFM_APBX_CHn_CMD_RSVD1(v)               BM_APBX_CHn_CMD_RSVD1
#define BF_APBX_CHn_CMD_RSVD1_V(e)              BF_APBX_CHn_CMD_RSVD1(BV_APBX_CHn_CMD_RSVD1__##e)
#define BFM_APBX_CHn_CMD_RSVD1_V(v)             BM_APBX_CHn_CMD_RSVD1
#define BP_APBX_CHn_CMD_HALTONTERMINATE         8
#define BM_APBX_CHn_CMD_HALTONTERMINATE         0x100
#define BF_APBX_CHn_CMD_HALTONTERMINATE(v)      (((v) & 0x1) << 8)
#define BFM_APBX_CHn_CMD_HALTONTERMINATE(v)     BM_APBX_CHn_CMD_HALTONTERMINATE
#define BF_APBX_CHn_CMD_HALTONTERMINATE_V(e)    BF_APBX_CHn_CMD_HALTONTERMINATE(BV_APBX_CHn_CMD_HALTONTERMINATE__##e)
#define BFM_APBX_CHn_CMD_HALTONTERMINATE_V(v)   BM_APBX_CHn_CMD_HALTONTERMINATE
#define BP_APBX_CHn_CMD_WAIT4ENDCMD             7
#define BM_APBX_CHn_CMD_WAIT4ENDCMD             0x80
#define BF_APBX_CHn_CMD_WAIT4ENDCMD(v)          (((v) & 0x1) << 7)
#define BFM_APBX_CHn_CMD_WAIT4ENDCMD(v)         BM_APBX_CHn_CMD_WAIT4ENDCMD
#define BF_APBX_CHn_CMD_WAIT4ENDCMD_V(e)        BF_APBX_CHn_CMD_WAIT4ENDCMD(BV_APBX_CHn_CMD_WAIT4ENDCMD__##e)
#define BFM_APBX_CHn_CMD_WAIT4ENDCMD_V(v)       BM_APBX_CHn_CMD_WAIT4ENDCMD
#define BP_APBX_CHn_CMD_SEMAPHORE               6
#define BM_APBX_CHn_CMD_SEMAPHORE               0x40
#define BF_APBX_CHn_CMD_SEMAPHORE(v)            (((v) & 0x1) << 6)
#define BFM_APBX_CHn_CMD_SEMAPHORE(v)           BM_APBX_CHn_CMD_SEMAPHORE
#define BF_APBX_CHn_CMD_SEMAPHORE_V(e)          BF_APBX_CHn_CMD_SEMAPHORE(BV_APBX_CHn_CMD_SEMAPHORE__##e)
#define BFM_APBX_CHn_CMD_SEMAPHORE_V(v)         BM_APBX_CHn_CMD_SEMAPHORE
#define BP_APBX_CHn_CMD_RSVD0                   4
#define BM_APBX_CHn_CMD_RSVD0                   0x30
#define BF_APBX_CHn_CMD_RSVD0(v)                (((v) & 0x3) << 4)
#define BFM_APBX_CHn_CMD_RSVD0(v)               BM_APBX_CHn_CMD_RSVD0
#define BF_APBX_CHn_CMD_RSVD0_V(e)              BF_APBX_CHn_CMD_RSVD0(BV_APBX_CHn_CMD_RSVD0__##e)
#define BFM_APBX_CHn_CMD_RSVD0_V(v)             BM_APBX_CHn_CMD_RSVD0
#define BP_APBX_CHn_CMD_IRQONCMPLT              3
#define BM_APBX_CHn_CMD_IRQONCMPLT              0x8
#define BF_APBX_CHn_CMD_IRQONCMPLT(v)           (((v) & 0x1) << 3)
#define BFM_APBX_CHn_CMD_IRQONCMPLT(v)          BM_APBX_CHn_CMD_IRQONCMPLT
#define BF_APBX_CHn_CMD_IRQONCMPLT_V(e)         BF_APBX_CHn_CMD_IRQONCMPLT(BV_APBX_CHn_CMD_IRQONCMPLT__##e)
#define BFM_APBX_CHn_CMD_IRQONCMPLT_V(v)        BM_APBX_CHn_CMD_IRQONCMPLT
#define BP_APBX_CHn_CMD_CHAIN                   2
#define BM_APBX_CHn_CMD_CHAIN                   0x4
#define BF_APBX_CHn_CMD_CHAIN(v)                (((v) & 0x1) << 2)
#define BFM_APBX_CHn_CMD_CHAIN(v)               BM_APBX_CHn_CMD_CHAIN
#define BF_APBX_CHn_CMD_CHAIN_V(e)              BF_APBX_CHn_CMD_CHAIN(BV_APBX_CHn_CMD_CHAIN__##e)
#define BFM_APBX_CHn_CMD_CHAIN_V(v)             BM_APBX_CHn_CMD_CHAIN
#define BP_APBX_CHn_CMD_COMMAND                 0
#define BM_APBX_CHn_CMD_COMMAND                 0x3
#define BV_APBX_CHn_CMD_COMMAND__NO_DMA_XFER    0x0
#define BV_APBX_CHn_CMD_COMMAND__DMA_WRITE      0x1
#define BV_APBX_CHn_CMD_COMMAND__DMA_READ       0x2
#define BF_APBX_CHn_CMD_COMMAND(v)              (((v) & 0x3) << 0)
#define BFM_APBX_CHn_CMD_COMMAND(v)             BM_APBX_CHn_CMD_COMMAND
#define BF_APBX_CHn_CMD_COMMAND_V(e)            BF_APBX_CHn_CMD_COMMAND(BV_APBX_CHn_CMD_COMMAND__##e)
#define BFM_APBX_CHn_CMD_COMMAND_V(v)           BM_APBX_CHn_CMD_COMMAND

#define HW_APBX_CHn_BAR(_n1)            HW(APBX_CHn_BAR(_n1))
#define HWA_APBX_CHn_BAR(_n1)           (0x80024000 + 0x130 + (_n1) * 0x70)
#define HWT_APBX_CHn_BAR(_n1)           HWIO_32_RW
#define HWN_APBX_CHn_BAR(_n1)           APBX_CHn_BAR
#define HWI_APBX_CHn_BAR(_n1)           (_n1)
#define BP_APBX_CHn_BAR_ADDRESS         0
#define BM_APBX_CHn_BAR_ADDRESS         0xffffffff
#define BF_APBX_CHn_BAR_ADDRESS(v)      (((v) & 0xffffffff) << 0)
#define BFM_APBX_CHn_BAR_ADDRESS(v)     BM_APBX_CHn_BAR_ADDRESS
#define BF_APBX_CHn_BAR_ADDRESS_V(e)    BF_APBX_CHn_BAR_ADDRESS(BV_APBX_CHn_BAR_ADDRESS__##e)
#define BFM_APBX_CHn_BAR_ADDRESS_V(v)   BM_APBX_CHn_BAR_ADDRESS

#define HW_APBX_CHn_SEMA(_n1)                   HW(APBX_CHn_SEMA(_n1))
#define HWA_APBX_CHn_SEMA(_n1)                  (0x80024000 + 0x140 + (_n1) * 0x70)
#define HWT_APBX_CHn_SEMA(_n1)                  HWIO_32_RW
#define HWN_APBX_CHn_SEMA(_n1)                  APBX_CHn_SEMA
#define HWI_APBX_CHn_SEMA(_n1)                  (_n1)
#define BP_APBX_CHn_SEMA_RSVD2                  24
#define BM_APBX_CHn_SEMA_RSVD2                  0xff000000
#define BF_APBX_CHn_SEMA_RSVD2(v)               (((v) & 0xff) << 24)
#define BFM_APBX_CHn_SEMA_RSVD2(v)              BM_APBX_CHn_SEMA_RSVD2
#define BF_APBX_CHn_SEMA_RSVD2_V(e)             BF_APBX_CHn_SEMA_RSVD2(BV_APBX_CHn_SEMA_RSVD2__##e)
#define BFM_APBX_CHn_SEMA_RSVD2_V(v)            BM_APBX_CHn_SEMA_RSVD2
#define BP_APBX_CHn_SEMA_PHORE                  16
#define BM_APBX_CHn_SEMA_PHORE                  0xff0000
#define BF_APBX_CHn_SEMA_PHORE(v)               (((v) & 0xff) << 16)
#define BFM_APBX_CHn_SEMA_PHORE(v)              BM_APBX_CHn_SEMA_PHORE
#define BF_APBX_CHn_SEMA_PHORE_V(e)             BF_APBX_CHn_SEMA_PHORE(BV_APBX_CHn_SEMA_PHORE__##e)
#define BFM_APBX_CHn_SEMA_PHORE_V(v)            BM_APBX_CHn_SEMA_PHORE
#define BP_APBX_CHn_SEMA_RSVD1                  8
#define BM_APBX_CHn_SEMA_RSVD1                  0xff00
#define BF_APBX_CHn_SEMA_RSVD1(v)               (((v) & 0xff) << 8)
#define BFM_APBX_CHn_SEMA_RSVD1(v)              BM_APBX_CHn_SEMA_RSVD1
#define BF_APBX_CHn_SEMA_RSVD1_V(e)             BF_APBX_CHn_SEMA_RSVD1(BV_APBX_CHn_SEMA_RSVD1__##e)
#define BFM_APBX_CHn_SEMA_RSVD1_V(v)            BM_APBX_CHn_SEMA_RSVD1
#define BP_APBX_CHn_SEMA_INCREMENT_SEMA         0
#define BM_APBX_CHn_SEMA_INCREMENT_SEMA         0xff
#define BF_APBX_CHn_SEMA_INCREMENT_SEMA(v)      (((v) & 0xff) << 0)
#define BFM_APBX_CHn_SEMA_INCREMENT_SEMA(v)     BM_APBX_CHn_SEMA_INCREMENT_SEMA
#define BF_APBX_CHn_SEMA_INCREMENT_SEMA_V(e)    BF_APBX_CHn_SEMA_INCREMENT_SEMA(BV_APBX_CHn_SEMA_INCREMENT_SEMA__##e)
#define BFM_APBX_CHn_SEMA_INCREMENT_SEMA_V(v)   BM_APBX_CHn_SEMA_INCREMENT_SEMA

#define HW_APBX_CHn_DEBUG1(_n1)                         HW(APBX_CHn_DEBUG1(_n1))
#define HWA_APBX_CHn_DEBUG1(_n1)                        (0x80024000 + 0x150 + (_n1) * 0x70)
#define HWT_APBX_CHn_DEBUG1(_n1)                        HWIO_32_RW
#define HWN_APBX_CHn_DEBUG1(_n1)                        APBX_CHn_DEBUG1
#define HWI_APBX_CHn_DEBUG1(_n1)                        (_n1)
#define BP_APBX_CHn_DEBUG1_REQ                          31
#define BM_APBX_CHn_DEBUG1_REQ                          0x80000000
#define BF_APBX_CHn_DEBUG1_REQ(v)                       (((v) & 0x1) << 31)
#define BFM_APBX_CHn_DEBUG1_REQ(v)                      BM_APBX_CHn_DEBUG1_REQ
#define BF_APBX_CHn_DEBUG1_REQ_V(e)                     BF_APBX_CHn_DEBUG1_REQ(BV_APBX_CHn_DEBUG1_REQ__##e)
#define BFM_APBX_CHn_DEBUG1_REQ_V(v)                    BM_APBX_CHn_DEBUG1_REQ
#define BP_APBX_CHn_DEBUG1_BURST                        30
#define BM_APBX_CHn_DEBUG1_BURST                        0x40000000
#define BF_APBX_CHn_DEBUG1_BURST(v)                     (((v) & 0x1) << 30)
#define BFM_APBX_CHn_DEBUG1_BURST(v)                    BM_APBX_CHn_DEBUG1_BURST
#define BF_APBX_CHn_DEBUG1_BURST_V(e)                   BF_APBX_CHn_DEBUG1_BURST(BV_APBX_CHn_DEBUG1_BURST__##e)
#define BFM_APBX_CHn_DEBUG1_BURST_V(v)                  BM_APBX_CHn_DEBUG1_BURST
#define BP_APBX_CHn_DEBUG1_KICK                         29
#define BM_APBX_CHn_DEBUG1_KICK                         0x20000000
#define BF_APBX_CHn_DEBUG1_KICK(v)                      (((v) & 0x1) << 29)
#define BFM_APBX_CHn_DEBUG1_KICK(v)                     BM_APBX_CHn_DEBUG1_KICK
#define BF_APBX_CHn_DEBUG1_KICK_V(e)                    BF_APBX_CHn_DEBUG1_KICK(BV_APBX_CHn_DEBUG1_KICK__##e)
#define BFM_APBX_CHn_DEBUG1_KICK_V(v)                   BM_APBX_CHn_DEBUG1_KICK
#define BP_APBX_CHn_DEBUG1_END                          28
#define BM_APBX_CHn_DEBUG1_END                          0x10000000
#define BF_APBX_CHn_DEBUG1_END(v)                       (((v) & 0x1) << 28)
#define BFM_APBX_CHn_DEBUG1_END(v)                      BM_APBX_CHn_DEBUG1_END
#define BF_APBX_CHn_DEBUG1_END_V(e)                     BF_APBX_CHn_DEBUG1_END(BV_APBX_CHn_DEBUG1_END__##e)
#define BFM_APBX_CHn_DEBUG1_END_V(v)                    BM_APBX_CHn_DEBUG1_END
#define BP_APBX_CHn_DEBUG1_RSVD2                        25
#define BM_APBX_CHn_DEBUG1_RSVD2                        0xe000000
#define BF_APBX_CHn_DEBUG1_RSVD2(v)                     (((v) & 0x7) << 25)
#define BFM_APBX_CHn_DEBUG1_RSVD2(v)                    BM_APBX_CHn_DEBUG1_RSVD2
#define BF_APBX_CHn_DEBUG1_RSVD2_V(e)                   BF_APBX_CHn_DEBUG1_RSVD2(BV_APBX_CHn_DEBUG1_RSVD2__##e)
#define BFM_APBX_CHn_DEBUG1_RSVD2_V(v)                  BM_APBX_CHn_DEBUG1_RSVD2
#define BP_APBX_CHn_DEBUG1_NEXTCMDADDRVALID             24
#define BM_APBX_CHn_DEBUG1_NEXTCMDADDRVALID             0x1000000
#define BF_APBX_CHn_DEBUG1_NEXTCMDADDRVALID(v)          (((v) & 0x1) << 24)
#define BFM_APBX_CHn_DEBUG1_NEXTCMDADDRVALID(v)         BM_APBX_CHn_DEBUG1_NEXTCMDADDRVALID
#define BF_APBX_CHn_DEBUG1_NEXTCMDADDRVALID_V(e)        BF_APBX_CHn_DEBUG1_NEXTCMDADDRVALID(BV_APBX_CHn_DEBUG1_NEXTCMDADDRVALID__##e)
#define BFM_APBX_CHn_DEBUG1_NEXTCMDADDRVALID_V(v)       BM_APBX_CHn_DEBUG1_NEXTCMDADDRVALID
#define BP_APBX_CHn_DEBUG1_RD_FIFO_EMPTY                23
#define BM_APBX_CHn_DEBUG1_RD_FIFO_EMPTY                0x800000
#define BF_APBX_CHn_DEBUG1_RD_FIFO_EMPTY(v)             (((v) & 0x1) << 23)
#define BFM_APBX_CHn_DEBUG1_RD_FIFO_EMPTY(v)            BM_APBX_CHn_DEBUG1_RD_FIFO_EMPTY
#define BF_APBX_CHn_DEBUG1_RD_FIFO_EMPTY_V(e)           BF_APBX_CHn_DEBUG1_RD_FIFO_EMPTY(BV_APBX_CHn_DEBUG1_RD_FIFO_EMPTY__##e)
#define BFM_APBX_CHn_DEBUG1_RD_FIFO_EMPTY_V(v)          BM_APBX_CHn_DEBUG1_RD_FIFO_EMPTY
#define BP_APBX_CHn_DEBUG1_RD_FIFO_FULL                 22
#define BM_APBX_CHn_DEBUG1_RD_FIFO_FULL                 0x400000
#define BF_APBX_CHn_DEBUG1_RD_FIFO_FULL(v)              (((v) & 0x1) << 22)
#define BFM_APBX_CHn_DEBUG1_RD_FIFO_FULL(v)             BM_APBX_CHn_DEBUG1_RD_FIFO_FULL
#define BF_APBX_CHn_DEBUG1_RD_FIFO_FULL_V(e)            BF_APBX_CHn_DEBUG1_RD_FIFO_FULL(BV_APBX_CHn_DEBUG1_RD_FIFO_FULL__##e)
#define BFM_APBX_CHn_DEBUG1_RD_FIFO_FULL_V(v)           BM_APBX_CHn_DEBUG1_RD_FIFO_FULL
#define BP_APBX_CHn_DEBUG1_WR_FIFO_EMPTY                21
#define BM_APBX_CHn_DEBUG1_WR_FIFO_EMPTY                0x200000
#define BF_APBX_CHn_DEBUG1_WR_FIFO_EMPTY(v)             (((v) & 0x1) << 21)
#define BFM_APBX_CHn_DEBUG1_WR_FIFO_EMPTY(v)            BM_APBX_CHn_DEBUG1_WR_FIFO_EMPTY
#define BF_APBX_CHn_DEBUG1_WR_FIFO_EMPTY_V(e)           BF_APBX_CHn_DEBUG1_WR_FIFO_EMPTY(BV_APBX_CHn_DEBUG1_WR_FIFO_EMPTY__##e)
#define BFM_APBX_CHn_DEBUG1_WR_FIFO_EMPTY_V(v)          BM_APBX_CHn_DEBUG1_WR_FIFO_EMPTY
#define BP_APBX_CHn_DEBUG1_WR_FIFO_FULL                 20
#define BM_APBX_CHn_DEBUG1_WR_FIFO_FULL                 0x100000
#define BF_APBX_CHn_DEBUG1_WR_FIFO_FULL(v)              (((v) & 0x1) << 20)
#define BFM_APBX_CHn_DEBUG1_WR_FIFO_FULL(v)             BM_APBX_CHn_DEBUG1_WR_FIFO_FULL
#define BF_APBX_CHn_DEBUG1_WR_FIFO_FULL_V(e)            BF_APBX_CHn_DEBUG1_WR_FIFO_FULL(BV_APBX_CHn_DEBUG1_WR_FIFO_FULL__##e)
#define BFM_APBX_CHn_DEBUG1_WR_FIFO_FULL_V(v)           BM_APBX_CHn_DEBUG1_WR_FIFO_FULL
#define BP_APBX_CHn_DEBUG1_RSVD1                        5
#define BM_APBX_CHn_DEBUG1_RSVD1                        0xfffe0
#define BF_APBX_CHn_DEBUG1_RSVD1(v)                     (((v) & 0x7fff) << 5)
#define BFM_APBX_CHn_DEBUG1_RSVD1(v)                    BM_APBX_CHn_DEBUG1_RSVD1
#define BF_APBX_CHn_DEBUG1_RSVD1_V(e)                   BF_APBX_CHn_DEBUG1_RSVD1(BV_APBX_CHn_DEBUG1_RSVD1__##e)
#define BFM_APBX_CHn_DEBUG1_RSVD1_V(v)                  BM_APBX_CHn_DEBUG1_RSVD1
#define BP_APBX_CHn_DEBUG1_STATEMACHINE                 0
#define BM_APBX_CHn_DEBUG1_STATEMACHINE                 0x1f
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__IDLE           0x0
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__REQ_CMD1       0x1
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__REQ_CMD3       0x2
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__REQ_CMD2       0x3
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__XFER_DECODE    0x4
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__REQ_WAIT       0x5
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__REQ_CMD4       0x6
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__PIO_REQ        0x7
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__READ_FLUSH     0x8
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__READ_WAIT      0x9
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__WRITE          0xc
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__READ_REQ       0xd
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__CHECK_CHAIN    0xe
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__XFER_COMPLETE  0xf
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__WAIT_END       0x15
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__WRITE_WAIT     0x1c
#define BV_APBX_CHn_DEBUG1_STATEMACHINE__CHECK_WAIT     0x1e
#define BF_APBX_CHn_DEBUG1_STATEMACHINE(v)              (((v) & 0x1f) << 0)
#define BFM_APBX_CHn_DEBUG1_STATEMACHINE(v)             BM_APBX_CHn_DEBUG1_STATEMACHINE
#define BF_APBX_CHn_DEBUG1_STATEMACHINE_V(e)            BF_APBX_CHn_DEBUG1_STATEMACHINE(BV_APBX_CHn_DEBUG1_STATEMACHINE__##e)
#define BFM_APBX_CHn_DEBUG1_STATEMACHINE_V(v)           BM_APBX_CHn_DEBUG1_STATEMACHINE

#define HW_APBX_CHn_DEBUG2(_n1)             HW(APBX_CHn_DEBUG2(_n1))
#define HWA_APBX_CHn_DEBUG2(_n1)            (0x80024000 + 0x160 + (_n1) * 0x70)
#define HWT_APBX_CHn_DEBUG2(_n1)            HWIO_32_RW
#define HWN_APBX_CHn_DEBUG2(_n1)            APBX_CHn_DEBUG2
#define HWI_APBX_CHn_DEBUG2(_n1)            (_n1)
#define BP_APBX_CHn_DEBUG2_APB_BYTES        16
#define BM_APBX_CHn_DEBUG2_APB_BYTES        0xffff0000
#define BF_APBX_CHn_DEBUG2_APB_BYTES(v)     (((v) & 0xffff) << 16)
#define BFM_APBX_CHn_DEBUG2_APB_BYTES(v)    BM_APBX_CHn_DEBUG2_APB_BYTES
#define BF_APBX_CHn_DEBUG2_APB_BYTES_V(e)   BF_APBX_CHn_DEBUG2_APB_BYTES(BV_APBX_CHn_DEBUG2_APB_BYTES__##e)
#define BFM_APBX_CHn_DEBUG2_APB_BYTES_V(v)  BM_APBX_CHn_DEBUG2_APB_BYTES
#define BP_APBX_CHn_DEBUG2_AHB_BYTES        0
#define BM_APBX_CHn_DEBUG2_AHB_BYTES        0xffff
#define BF_APBX_CHn_DEBUG2_AHB_BYTES(v)     (((v) & 0xffff) << 0)
#define BFM_APBX_CHn_DEBUG2_AHB_BYTES(v)    BM_APBX_CHn_DEBUG2_AHB_BYTES
#define BF_APBX_CHn_DEBUG2_AHB_BYTES_V(e)   BF_APBX_CHn_DEBUG2_AHB_BYTES(BV_APBX_CHn_DEBUG2_AHB_BYTES__##e)
#define BFM_APBX_CHn_DEBUG2_AHB_BYTES_V(v)  BM_APBX_CHn_DEBUG2_AHB_BYTES

#define HW_APBX_VERSION             HW(APBX_VERSION)
#define HWA_APBX_VERSION            (0x80024000 + 0x800)
#define HWT_APBX_VERSION            HWIO_32_RW
#define HWN_APBX_VERSION            APBX_VERSION
#define HWI_APBX_VERSION            
#define BP_APBX_VERSION_MAJOR       24
#define BM_APBX_VERSION_MAJOR       0xff000000
#define BF_APBX_VERSION_MAJOR(v)    (((v) & 0xff) << 24)
#define BFM_APBX_VERSION_MAJOR(v)   BM_APBX_VERSION_MAJOR
#define BF_APBX_VERSION_MAJOR_V(e)  BF_APBX_VERSION_MAJOR(BV_APBX_VERSION_MAJOR__##e)
#define BFM_APBX_VERSION_MAJOR_V(v) BM_APBX_VERSION_MAJOR
#define BP_APBX_VERSION_MINOR       16
#define BM_APBX_VERSION_MINOR       0xff0000
#define BF_APBX_VERSION_MINOR(v)    (((v) & 0xff) << 16)
#define BFM_APBX_VERSION_MINOR(v)   BM_APBX_VERSION_MINOR
#define BF_APBX_VERSION_MINOR_V(e)  BF_APBX_VERSION_MINOR(BV_APBX_VERSION_MINOR__##e)
#define BFM_APBX_VERSION_MINOR_V(v) BM_APBX_VERSION_MINOR
#define BP_APBX_VERSION_STEP        0
#define BM_APBX_VERSION_STEP        0xffff
#define BF_APBX_VERSION_STEP(v)     (((v) & 0xffff) << 0)
#define BFM_APBX_VERSION_STEP(v)    BM_APBX_VERSION_STEP
#define BF_APBX_VERSION_STEP_V(e)   BF_APBX_VERSION_STEP(BV_APBX_VERSION_STEP__##e)
#define BFM_APBX_VERSION_STEP_V(v)  BM_APBX_VERSION_STEP

#endif /* __HEADERGEN_IMX233_APBX_H__*/

/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 * Copyright (C) 2013 by Marcin Bukat
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
#include "stddef.h"
#include "target.h"
#include "system.h"
#include "logf.h"
#include "rk27xx.h"

#define HZ  1000000

enum rk27xx_family_t
{
    UNKNOWN,
    REV_A,
    REV_B,
};

static enum rk27xx_family_t g_rk27xx_family = UNKNOWN;
static int g_atexit = HWSTUB_ATEXIT_OFF;

static void _enable_irq(void)
{
    asm volatile ("mrs r0, cpsr\n"
                  "bic r0, r0, #0x80\n"
                  "msr cpsr_c, r0\n"
                 );
}

static void power_off(void)
{
    GPIO_PCCON &= ~(1<<0);
    while(1);
}

static void rk27xx_reset(void)
{
    /* use Watchdog to reset */
    SCU_CLKCFG &= ~CLKCFG_WDT;
    WDTLR = 1;
    WDTCON = (1<<4) | (1<<3);

    /* Wait for reboot to kick in */
    while(1);
}

/* us may be at most 2^31/200 (~10 seconds) for 200MHz max cpu freq */
void target_udelay(int us)
{
    unsigned cycles_per_us;
    unsigned delay;

    cycles_per_us = (200000000 + 999999) / 1000000;

    delay = (us * cycles_per_us) / 5;

    asm volatile(
        "1: subs %0, %0, #1  \n"    /* 1 cycle  */
        "   nop              \n"    /* 1 cycle  */
        "   bne  1b          \n"    /* 3 cycles */
        : : "r"(delay)
    );
}

void target_mdelay(int ms)
{
    return target_udelay(ms * 1000);
}

void target_init(void)
{
    /* ungate all clocks */
    SCU_CLKCFG = 0;

    /* keep act line */
    GPIO_PCDR |= (1<<0);
    GPIO_PCCON |= (1<<0);

    /* disable watchdog */
    WDTCON &= ~(1<<3);

    /* enable UDC interrupt */
    INTC_IMR = (1<<16);
    INTC_IECR = (1<<16);

    EN_INT = EN_SUSP_INTR   |  /* Enable Suspend Interrupt */
             EN_RESUME_INTR |  /* Enable Resume Interrupt */
             EN_USBRST_INTR |  /* Enable USB Reset Interrupt */
             EN_OUT0_INTR   |  /* Enable OUT Token receive Interrupt EP0 */
             EN_IN0_INTR    |  /* Enable IN Token transmits Interrupt EP0 */
             EN_SETUP_INTR;    /* Enable SETUP Packet Receive Interrupt */

    /* 6. configure INTCON */
    INTCON = UDC_INTHIGH_ACT |  /* interrupt high active */
             UDC_INTEN;         /* enable EP0 interrupts */

    /* enable irq */
    _enable_irq();

    /* detect revision */
    uint32_t rk27xx_id = SCU_ID;

    if(rk27xx_id == 0xa1000604)
    {
        logf("identified rk27xx REV_A \n");
        g_rk27xx_family = REV_A;
    }
    else if(rk27xx_id == 0xa100027b)
    {
        logf("identified rk27xx REV_B \n");
        g_rk27xx_family = REV_B;
    }
    else
    {
        logf("unknown rk27xx revision \n");
    }
}

static struct usb_resp_info_target_t g_target =
{
    .id = HWSTUB_TARGET_RK27,
    .name = "Rockchip RK27XX"
};

int target_get_info(int info, void **buffer)
{
    if(info == HWSTUB_INFO_TARGET)
    {
        *buffer = &g_target;
        return sizeof(g_target);
    }
    else
        return -1;
}

int target_atexit(int method)
{
    g_atexit = method;
    return 0;
}

void target_exit(void)
{
    switch(g_atexit)
    {
        case HWSTUB_ATEXIT_OFF:
            power_off();
            // fallthrough in case of return
        case HWSTUB_ATEXIT_REBOOT:
            rk27xx_reset();
            // fallthrough in case of return
        case HWSTUB_ATEXIT_NOP:
        default:
            return;
    }
}

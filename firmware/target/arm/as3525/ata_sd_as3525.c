/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 Daniel Ankers
 * Copyright © 2008 Rafaël Carré
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

/* Driver for the ARM PL180 SD/MMC controller inside AS3525 SoC */

#include "config.h" /* for HAVE_MULTIVOLUME */
#include "fat.h"
#include "thread.h"
#include "hotswap.h"
#include "system.h"
#include "cpu.h"
#include <stdlib.h>
#include "as3525.h"
#include "pl180.h"
#include "panic.h"
#include "stdbool.h"
#include "ata_idle_notify.h"
#include "sd.h"

#ifdef HAVE_HOTSWAP
#include "disk.h"
#endif

/* command flags */
#define MMC_NO_FLAGS    (0<<0)
#define MMC_RESP        (1<<0)
#define MMC_LONG_RESP   (1<<1)
#define MMC_ARG         (1<<2)

/* ARM PL180 registers */
#define MMC_POWER(i)       (*(volatile unsigned char *) (pl180_base[i]+0x00))
#define MMC_CLOCK(i)       (*(volatile unsigned long *) (pl180_base[i]+0x04))
#define MMC_ARGUMENT(i)    (*(volatile unsigned long *) (pl180_base[i]+0x08))
#define MMC_COMMAND(i)     (*(volatile unsigned long *) (pl180_base[i]+0x0C))
#define MMC_RESPCMD(i)     (*(volatile unsigned long *) (pl180_base[i]+0x10))
#define MMC_RESP0(i)       (*(volatile unsigned long *) (pl180_base[i]+0x14))
#define MMC_RESP1(i)       (*(volatile unsigned long *) (pl180_base[i]+0x18))
#define MMC_RESP2(i)       (*(volatile unsigned long *) (pl180_base[i]+0x1C))
#define MMC_RESP3(i)       (*(volatile unsigned long *) (pl180_base[i]+0x20))
#define MMC_DATA_TIMER(i)  (*(volatile unsigned long *) (pl180_base[i]+0x24))
#define MMC_DATA_LENGTH(i) (*(volatile unsigned short*) (pl180_base[i]+0x28))
#define MMC_DATA_CTRL(i)   (*(volatile unsigned char *) (pl180_base[i]+0x2C))
#define MMC_DATA_CNT(i)    (*(volatile unsigned short*) (pl180_base[i]+0x30))
#define MMC_STATUS(i)      (*(volatile unsigned long *) (pl180_base[i]+0x34))
#define MMC_CLEAR(i)       (*(volatile unsigned long *) (pl180_base[i]+0x38))
#define MMC_MASK0(i)       (*(volatile unsigned long *) (pl180_base[i]+0x3C))
#define MMC_MASK1(i)       (*(volatile unsigned long *) (pl180_base[i]+0x40))
#define MMC_SELECT(i)      (*(volatile unsigned long *) (pl180_base[i]+0x44))
#define MMC_FIFO_CNT(i)    (*(volatile unsigned long *) (pl180_base[i]+0x48))

#define MMC_FIFO(i)        ((unsigned long *) (pl180_base[i]+0x80))
/* volumes */
#define     NAND_AS3525 0
#define     SD_AS3525   1

static const int pl180_base[NUM_VOLUMES] = {
            NAND_FLASH_BASE
#ifdef HAVE_MULTIVOLUME
            , SD_MCI_BASE
#endif
};

#define BLOCK_SIZE      512
#define SECTOR_SIZE     512

static tSDCardInfo card_info[NUM_VOLUMES];

/* for compatibility */
static long last_disk_activity = -1;

#define MIN_YIELD_PERIOD 1000
static long next_yield = 0;

/* Shoot for around 75% usage */
static long sd_stack [(DEFAULT_STACK_SIZE*2 + 0x1c0)/sizeof(long)];
static const char         sd_thread_name[] = "ata/sd";
static struct mutex       sd_mtx SHAREDBSS_ATTR;
static struct event_queue sd_queue;

static inline void mci_delay(void) { int i = 0xffff; while(i--) ; }

static void mci_set_clock_divider(const int drive, int divider)
{
    int clock = MMC_CLOCK(drive);

    if(divider > 1)
    {
        /* use divide logic */
        clock &= ~MCI_CLOCK_BYPASS;

        /* convert divider to MMC_CLOCK logic */
        divider = (divider/2) - 1;
        if(divider >= 256)
            divider = 255;
    }
    else
    {
        /* bypass dividing logic */
        clock |= MCI_CLOCK_BYPASS;
        divider = 0;
    }

    MMC_CLOCK(drive) = clock | divider;

    mci_delay();
}

static bool send_cmd(const int drive, const int cmd, const int arg,
                     const int flags, int *response)
{
    int val, status;

    while(MMC_STATUS(drive) & MCI_CMD_ACTIVE);

    if(MMC_COMMAND(drive) & MCI_COMMAND_ENABLE) /* clears existing command */
    {
        MMC_COMMAND(drive) = 0;
        mci_delay();
    }

    val = cmd | MCI_COMMAND_ENABLE;
    if(flags & MMC_RESP)
    {
        val |= MCI_COMMAND_RESPONSE;
        if(flags & MMC_LONG_RESP)
            val |= MCI_COMMAND_LONG_RESPONSE;
    }

    MMC_CLEAR(drive) = 0x7ff;

    MMC_ARGUMENT(drive) = (flags & MMC_ARG) ? arg : 0;
    MMC_COMMAND(drive) = val;

    while(MMC_STATUS(drive) & MCI_CMD_ACTIVE);  /* wait for cmd completion */

    MMC_COMMAND(drive) = 0;
    MMC_ARGUMENT(drive) = ~0;

    status = MMC_STATUS(drive);
    MMC_CLEAR(drive) = 0x7ff;

    if(flags & MMC_RESP)
    {
        if(status & MCI_CMD_TIMEOUT)
            return false;
        else if(status & (MCI_CMD_CRC_FAIL /* FIXME? */ | MCI_CMD_RESP_END))
        {   /* resp received */
            if(flags & MMC_LONG_RESP)
            {
                /* store the response in little endian order for the words */
                response[0] = MMC_RESP3(drive);
                response[1] = MMC_RESP2(drive);
                response[2] = MMC_RESP1(drive);
                response[3] = MMC_RESP0(drive);
            }
            else
                response[0] = MMC_RESP0(drive);
            return true;
        }
    }
    else if(status & MCI_CMD_SENT)
        return true;

    return false;
}

static int sd_init_card(const int drive)
{
    unsigned int  c_size;
    unsigned long c_mult;
    int response;
    int max_tries = 100; /* max acmd41 attemps */
    bool sdhc;

    if(!send_cmd(drive, SD_GO_IDLE_STATE, 0, MMC_NO_FLAGS, NULL))
        return -1;

    mci_delay();

    sdhc = false;
    if(send_cmd(drive, SD_SEND_IF_COND, 0x1AA, MMC_RESP|MMC_ARG, &response))
        if((response & 0xFFF) == 0x1AA)
            sdhc = true;

    do {
        mci_delay();

        /* app_cmd */
        if( !send_cmd(drive, SD_APP_CMD, 0, MMC_RESP|MMC_ARG, &response) ||
            !(response & (1<<5)) )
        {
            return -2;
        }

        /* acmd41 */
        if(!send_cmd(drive, SD_APP_OP_COND, (sdhc ? 0x40FF8000 : (1<<23)),
                        MMC_RESP|MMC_ARG, &card_info[drive].ocr))
            return -3;

    } while(!(card_info[drive].ocr & (1<<31)) && max_tries--);

    if(!max_tries)
        return -4;

    /* send CID */
    if(!send_cmd(drive, SD_ALL_SEND_CID, 0, MMC_RESP|MMC_LONG_RESP|MMC_ARG,
                            card_info[drive].cid))
        return -5;

    /* send RCA */
    if(!send_cmd(drive, SD_SEND_RELATIVE_ADDR, 0, MMC_RESP|MMC_ARG,
                &card_info[drive].rca))
        return -6;

    /* send CSD */
    if(!send_cmd(drive, SD_SEND_CSD, card_info[drive].rca,
                 MMC_RESP|MMC_LONG_RESP|MMC_ARG, card_info[drive].csd))
        return -7;

    /* These calculations come from the Sandisk SD card product manual */
    if( (card_info[drive].csd[3]>>30) == 0)
    {
        /* CSD version 1.0 */
        c_size = ((card_info[drive].csd[2] & 0x3ff) << 2) + (card_info[drive].csd[1]>>30) + 1;
        c_mult = 4 << ((card_info[drive].csd[1] >> 15) & 7);
        card_info[drive].max_read_bl_len = 1 << ((card_info[drive].csd[2] >> 16) & 15);
        card_info[drive].block_size = BLOCK_SIZE;     /* Always use 512 byte blocks */
        card_info[drive].numblocks = c_size * c_mult * (card_info[drive].max_read_bl_len/512);
        card_info[drive].capacity = card_info[drive].numblocks * card_info[drive].block_size;
    }
#ifdef HAVE_MULTIVOLUME
    else if( (card_info[drive].csd[3]>>30) == 1)
    {
        /* CSD version 2.0 */
        c_size = ((card_info[drive].csd[2] & 0x3f) << 16) + (card_info[drive].csd[1]>>16) + 1;
        card_info[drive].max_read_bl_len = 1 << ((card_info[drive].csd[2] >> 16) & 0xf);
        card_info[drive].block_size = BLOCK_SIZE;     /* Always use 512 byte blocks */
        card_info[drive].numblocks = c_size << 10;
        card_info[drive].capacity = card_info[drive].numblocks * card_info[drive].block_size;
    }
#endif

    if(!send_cmd(drive, SD_SELECT_CARD, card_info[drive].rca, MMC_ARG, NULL))
        return -9;

    if(!send_cmd(drive, SD_APP_CMD, card_info[drive].rca, MMC_ARG, NULL))
        return -10;

    if(!send_cmd(drive, SD_SET_BUS_WIDTH, card_info[drive].rca | 2, MMC_ARG, NULL))
        return -11;

    if(!send_cmd(drive, SD_SET_BLOCKLEN, card_info[drive].block_size, MMC_ARG,
                 NULL))
        return -12;

    card_info[drive].initialized = 1;

    mci_set_clock_divider(drive, 1); /* full speed */

    return 0;
}

static void sd_thread(void) __attribute__((noreturn));
static void sd_thread(void)
{
    struct queue_event ev;
    bool idle_notified = false;

    while (1)
    {
        queue_wait_w_tmo(&sd_queue, &ev, HZ);

        switch ( ev.id )
        {
#ifdef HAVE_HOTSWAP
        case SYS_HOTSWAP_INSERTED:
        case SYS_HOTSWAP_EXTRACTED:
            fat_lock();          /* lock-out FAT activity first -
                                    prevent deadlocking via disk_mount that
                                    would cause a reverse-order attempt with
                                    another thread */
            mutex_lock(&sd_mtx); /* lock-out card activity - direct calls
                                    into driver that bypass the fat cache */

            /* We now have exclusive control of fat cache and ata */

            disk_unmount(1);     /* release "by force", ensure file
                                    descriptors aren't leaked and any busy
                                    ones are invalid if mounting */

            /* Force card init for new card, re-init for re-inserted one or
             * clear if the last attempt to init failed with an error. */
            card_info[1].initialized = 0;

            if (ev.id == SYS_HOTSWAP_INSERTED)
                disk_mount(1);

            queue_broadcast(SYS_FS_CHANGED, 0);

            /* Access is now safe */
            mutex_unlock(&sd_mtx);
            fat_unlock();
            break;
#endif
        case SYS_TIMEOUT:
            if (TIME_BEFORE(current_tick, last_disk_activity+(3*HZ)))
            {
                idle_notified = false;
            }
            else
            {
                /* never let a timer wrap confuse us */
                next_yield = current_tick;

                if (!idle_notified)
                {
                    call_storage_idle_notifys(false);
                    idle_notified = true;
                }
            }
            break;
#if 0
        case SYS_USB_CONNECTED:
            usb_acknowledge(SYS_USB_CONNECTED_ACK);
            /* Wait until the USB cable is extracted again */
            usb_wait_for_disconnect(&sd_queue);

            break;
        case SYS_USB_DISCONNECTED:
            usb_acknowledge(SYS_USB_DISCONNECTED_ACK);
            break;
#endif
        }
    }
}
static void init_pl180_controller(const int drive)
{
#ifdef BOOTLOADER
    MMC_COMMAND(drive) = MMC_DATA_CTRL(drive) = 0;
    MMC_CLEAR(drive) = 0x7ff;

    MMC_MASK0(drive) = MMC_MASK1(drive) = 0;  /* disable all interrupts */

    MMC_POWER(drive) = MCI_POWER_UP|(10 /*voltage*/ << 2); /* use OF voltage */
    mci_delay();

    MMC_POWER(drive) |= MCI_POWER_ON;
    mci_delay();

    MMC_SELECT(drive) = 0;

    MMC_CLOCK(drive) = MCI_CLOCK_ENABLE;
    MMC_CLOCK(drive) &= ~MCI_CLOCK_POWERSAVE;
#endif /* BOOTLOADER */

    /* set MCLK divider */
    mci_set_clock_divider(drive, 200);
}

int sd_init(void)
{
    int ret;

#ifdef BOOTLOADER /* No need to do twice the same thing */
    CGU_IDE =   (1<<7)  /* AHB interface enable */  |
                (1<<6)  /* interface enable */      |
                (2<<2)  /* clock didiver = 2+1 */   |
                1       /* clock source = PLLA */;

    CGU_PERI |= CGU_NAF_CLOCK_ENABLE;
#ifdef HAVE_MULTIVOLUME
    CGU_PERI |= CGU_MCI_CLOCK_ENABLE;
#endif

    CCU_IO &= ~8;           /* bits 3:2 = 01, xpd is SD interface */
    CCU_IO |= 4;

#endif
    init_pl180_controller(NAND_AS3525);
    ret = sd_init_card(NAND_AS3525);
    if(ret < 0)
        return ret;

#ifdef HAVE_MULTIVOLUME
    init_pl180_controller(SD_AS3525);
    ret = sd_init_card(SD_AS3525);
    if(ret < 0)
        return ret;
#endif

    queue_init(&sd_queue, true);
    create_thread(sd_thread, sd_stack, sizeof(sd_stack), 0,
            sd_thread_name IF_PRIO(, PRIORITY_USER_INTERFACE) IF_COP(, CPU));

    return 0;
}

#ifdef STORAGE_GET_INFO
void sd_get_info(IF_MV2(int drive,) struct storage_info *info)
{
#ifndef HAVE_MULTIVOLUME
    const int drive=0;
#endif
    info->sector_size=card_info[drive].block_size;
    info->num_sectors=card_info[drive].numblocks;
    info->vendor="Rockbox";
    info->product = (drive == 0) ?  "Internal Storage" : "SD Card Slot";
    info->revision="0.00";
}
#endif

#ifdef HAVE_HOTSWAP
bool sd_removable(IF_MV_NONVOID(int drive))
{
#ifndef HAVE_MULTIVOLUME
    const int drive=0;
#endif
    return (drive==1);
}

bool sd_present(IF_MV_NONVOID(int drive))
{
#ifndef HAVE_MULTIVOLUME
    const int drive=0;
#endif
    return (card_info[drive].initialized && card_info[drive].numblocks > 0);
}
#endif

int sd_write_sectors(IF_MV2(int drive,) unsigned long start, int count, const void* buf)
{
    (void)start;
    (void)count;
    (void)buf;
    return 0; /* TODO */
}

static bool sd_poll_status(const int drive, unsigned int trigger, long timeout)
{
    long t = current_tick;

    while ((MMC_STATUS(drive) & trigger) == 0)
    {
        long time = current_tick;

        if (TIME_AFTER(time, next_yield))
        {
            long ty = current_tick;
            yield();
            timeout += current_tick - ty;
            next_yield = ty + MIN_YIELD_PERIOD;
        }

        if (TIME_AFTER(time, t + timeout))
            return false;
    }

    return true;
}

static int sd_wait_for_state(const int drive, unsigned int state)
{
    unsigned int response = 0;
    unsigned int timeout = 0x80000;

    long t = current_tick;

    while (1)
    {
        long us;

        if(!send_cmd(drive, SD_SEND_STATUS, card_info[drive].rca,
                    MMC_RESP|MMC_ARG, &response))
            return -1;

        if (((response >> 9) & 0xf) == state)
            return 0;

        if(TIME_AFTER(current_tick, t + timeout))
            return -1;

        us = current_tick;
        if (TIME_AFTER(us, next_yield))
        {
            yield();
            timeout += current_tick - us;
            next_yield = us + MIN_YIELD_PERIOD;
        }
    }
}

int sd_read_sectors(IF_MV2(int drive,) unsigned long start, int incount,
                     void* inbuf)
{
#ifndef HAVE_MULTIVOLUME
    const int drive = 0;
#endif
    int ret;
    unsigned char *buf_end, *buf = inbuf;
    int remaining = incount;
    const unsigned long *fifo_base = MMC_FIFO(drive);

    start += 20480; /* skip SanDisk OF */

    /* TODO: Add DMA support. */

    mutex_lock(&sd_mtx);

#ifdef HAVE_MULTIVOLUME
    if (drive != 0 && !card_detect_target())
    {
        /* no external sd-card inserted */
        ret = -88;
        goto sd_read_error;
    }
#endif

    if (card_info[drive].initialized < 0)
    {
        ret = card_info[drive].initialized;
        goto sd_read_error;
    }

    last_disk_activity = current_tick;

    ret = sd_wait_for_state(drive, SD_TRAN);
    if (ret < 0)
        goto sd_read_error;

    while(remaining)
    {
        /* 128 * 512 = 2^16, and doesn't fit in the 16 bits of DATA_LENGTH
         * register, so we have to transfer maximum 127 sectors at a time. */
        int transfer = (remaining >= 128) ? 127 : remaining; /* sectors */

        if(card_info[drive].ocr & (1<<30) ) /* SDHC */
            ret = send_cmd(drive, SD_READ_MULTIPLE_BLOCK, start, MMC_ARG, NULL);
        else
            ret = send_cmd(drive, SD_READ_MULTIPLE_BLOCK, start * BLOCK_SIZE,
                    MMC_ARG, NULL);

        if (ret < 0)
            goto sd_read_error;

        /* TODO: Don't assume BLOCK_SIZE == SECTOR_SIZE */


        MMC_DATA_TIMER(drive) = 0x1000000; /* FIXME: arbitrary */
        MMC_DATA_LENGTH(drive) = transfer * card_info[drive].block_size;
        MMC_DATA_CTRL(drive) =  (1<<0) /* enable */ |
                                (1<<1) /* from card to controller */ |
                                (9<<4) /* 2^9 = 512 */ ;

        buf_end = buf + transfer * card_info[drive].block_size;

        while(buf < buf_end)
        {
            /* Wait for the FIFO to be half full */
            if (!sd_poll_status(drive, ((1<<15)), 100))
            {
                ret = -42;
                goto sd_read_error;
            }

            asm volatile(
                "ldmia %2,  {r0-r7} \n" /* load  8 * 4 bytes */
                "stmia %1!, {r0-r7} \n" /* store 8 * 4 bytes */
                :"=r"(buf)                /* output */
                :"r"(buf), "r"(fifo_base) /* input */
                :"r0","r1","r2","r3","r4","r5","r6","r7","r8" /* clobbers */
            );
        }

        remaining -= transfer;
        start += transfer;
        last_disk_activity = current_tick;

        if(!send_cmd(drive, SD_STOP_TRANSMISSION, 0, MMC_NO_FLAGS, NULL))
        {
            ret = -666;
            goto sd_read_error;
        }

        ret = sd_wait_for_state(drive, SD_TRAN);
        if (ret < 0)
            goto sd_read_error;

    }
    while (1)
    {
        mutex_unlock(&sd_mtx);

        return ret;

sd_read_error:
        card_info[drive].initialized = 0;
    }
}

void sd_sleep(void)
{
}

void sd_spin(void)
{
}

void sd_spindown(int seconds)
{
    (void)seconds;
}

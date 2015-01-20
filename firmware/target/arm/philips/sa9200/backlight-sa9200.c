/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2006 by Barry Wardell
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
#include "config.h"
#include "backlight-target.h"
#include "system.h"
#include "backlight.h"
#include "ascodec.h"
#include "as3514.h"
#include "synaptics-mep.h"
#include "lcd.h"

void backlight_hw_brightness(int brightness)
{
    ascodec_write(AS3514_DCDC15, brightness);
}

void backlight_hw_on(void)
{
#ifdef HAVE_LCD_ENABLE
    lcd_enable(true); /* power on lcd + visible display */
#endif
#if (CONFIG_BACKLIGHT_FADING != BACKLIGHT_FADING_SW_SETTING) /* in bootloader/sim */
    /* if we set the brightness to the settings value, then fading up
     * is glitchy */
    backlight_hw_brightness(backlight_brightness);
#endif
}

void backlight_hw_off(void)
{
    backlight_hw_brightness(0);
#ifdef HAVE_LCD_ENABLE
    lcd_enable(false); /* power off visible display */
#endif
}

#ifdef HAVE_BUTTON_LIGHT

#define BUTTONLIGHT_MASK 0x7f
#define BUTTONLIGHT_MAX  0x0f
static unsigned short buttonlight_status = 0;

void buttonlight_hw_on(void)
{
    if (!buttonlight_status)
    {
        touchpad_set_buttonlights(BUTTONLIGHT_MASK, BUTTONLIGHT_MAX);
        GPIOD_OUTPUT_VAL &= ~(0x40 | 0x20 | 0x04); /* REW/FFWD/MENU */
        buttonlight_status = 1;
    }
}

void buttonlight_hw_off(void)
{
    if (buttonlight_status)
    {
        touchpad_set_buttonlights(BUTTONLIGHT_MASK, 0);
        GPIOD_OUTPUT_VAL |= (0x40 | 0x20 | 0x04); /* REW/FFWD/MENU */
        buttonlight_status = 0;
    }
}
#endif

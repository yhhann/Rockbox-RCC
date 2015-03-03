/***************************************************************************
 *             __________               __   ___
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 * Copyright (C) 2014 by Ilia Sergachev: Initial Rockbox port to iBasso DX50
 * Copyright (C) 2014 by Mario Basister: iBasso DX90 port
 * Copyright (C) 2014 by Simon Rothen: Initial Rockbox repository submission, additional features
 * Copyright (C) 2014 by Udo Schläpfer: Code clean up, additional features
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


#ifndef _USB_DX50_H_
#define _USB_DX50_H_


/* Supported usb modes. */
enum ibasso_usb_mode
{
    /*
        USB mass storage mode. On USB connection, Rockbox will terminate and the internel and
        external storage gets exported to the connected client.
    */
    USB_MODE_MASS_STORAGE = 0,

    /*
        Actually the same, since we to not have proper USB detection.
        Starts the adb server and enables adb connection over USB. Rockbox will continue to run.
    */
    USB_MODE_CHARGE,
    USB_MODE_ADB
};


/*
    Set the usb mode.
    mode: ibasso_usb_mode
*/
void ibasso_set_usb_mode(int mode);


#endif

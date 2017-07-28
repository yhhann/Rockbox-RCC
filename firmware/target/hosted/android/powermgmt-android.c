/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2010 by Thomas Martitz
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


#include <jni.h>
#include <stdbool.h>
#include "kernel.h"
#include "button.h"
#include "config.h"

extern JNIEnv *env_ptr;
extern jclass  RockboxService_class;
extern jobject RockboxService_instance;
extern void jni_call(void(*fn)(void));

static jfieldID __battery_level;
static jobject BatteryMonitor_instance;

static void new_battery_monitor(void)
{
    JNIEnv e = *env_ptr;
    jclass class = e->FindClass(env_ptr, "com/gaana/monitors/BatteryMonitor");
    jmethodID constructor = e->GetMethodID(env_ptr, class,
                                            "<init>",
                                            "(Landroid/content/Context;)V");
    BatteryMonitor_instance = e->NewObject(env_ptr, class,
                                            constructor,
                                            RockboxService_instance);

    /* cache the battery level field id */
    __battery_level = (*env_ptr)->GetFieldID(env_ptr,
                                            class,
                                            "mBattLevel", "I");
}

int _battery_level(void)
{
    if (!BatteryMonitor_instance)
    {
        jni_call(new_battery_monitor);
        return -1;
    }
    else
    {
        return (*env_ptr)->GetIntField(env_ptr, BatteryMonitor_instance, __battery_level);
    }
}


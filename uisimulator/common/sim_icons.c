/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Mats Lidell <matsl@contactor.se>
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#ifdef HAVE_LCD_CHARCELLS

#include "sim_icons.h"

#include <lcd.h>
#include <kernel.h>
#include <sprintf.h>
#include <string.h>
#include <debug.h>

extern void lcd_print_icon(int x, int icon_line, bool enable, char **icon);

static char* icon_battery_3[]=
  {
 "***************  ",
 "*             *  ",
 "* **  **  **  *  ",
 "* **  **  **  ***",
 "* **  **  **  ***",
 "* **  **  **  ***",
 "* **  **  **  *  ",
 "*             *  ",
 "***************  ",NULL
 };
static char* icon_battery_2[]=
  {
 "***************  ",
 "*             *  ",
 "* **  **      *  ",
 "* **  **      ***",
 "* **  **      ***",
 "* **  **      ***",
 "* **  **      *  ",
 "*             *  ",
 "***************  ",NULL
 };
static char* icon_battery_1[]=
  {
 "***************  ",
 "*             *  ",
 "* **          *  ",
 "* **          ***",
 "* **          ***",
 "* **          ***",
 "* **          *  ",
 "*             *  ",
 "***************  ",NULL
 };
static char* icon_battery[]=
  {
 "***************  ",
 "*             *  ",
 "*             *  ",
 "*             ***",
 "*             ***",
 "*             ***",
 "*             *  ",
 "*             *  ",
 "***************  ",NULL
 };

static char* icon_volume[]=
  {
 "*   *    *       ",
 "*   *    *       ",
 "*   *    *       ",
 " * *     *       ",
 " * *     *       ",
 " * *  *  *       ",
 "  *  * * *       ",
 "  *  * * *       ",
 "  *   *  *       ",NULL
 };
static char* icon_volume_1[]={NULL};
static char* icon_volume_2[]={NULL};
static char* icon_volume_3[]={NULL};
static char* icon_volume_4[]={NULL};
static char* icon_volume_5[]={NULL};

static char* icon_pause[]={NULL};
static char* icon_play[]={NULL};
static char* icon_record[]={NULL};
static char* icon_usb[]={NULL};
static char* icon_audio[]={NULL};
static char* icon_param[]={NULL};
static char* icon_repeat[]={NULL};
static char* icon_repeat2[]={NULL};

struct icon_info
{
    char** bitmap;
    int xpos;
    int row;
};

static struct icon_info icons [] = 
{
  {icon_battery, 0, 0},
  {icon_battery_1, 0, 0},
  {icon_battery_2, 0, 0},
  {icon_battery_3, 0, 0},
  {icon_usb, 0, 1},
  {icon_play, 0, 1},
  {icon_record, 0, 1},
  {icon_pause, 0, 1},
  {icon_audio, 0, 1},
  {icon_repeat, 0, 1},
  {icon_repeat2, 0, 1},
  {icon_volume, 108, 0},
  {icon_volume_1, 108, 0},
  {icon_volume_2, 108, 0},
  {icon_volume_3, 108, 0},
  {icon_volume_4, 108, 0},
  {icon_volume_5, 108, 0},
  {icon_param, 0, 1}
};

void
lcd_icon(int icon, bool enable)
{
  lcd_print_icon(icons[icon].xpos, icons[icon].row, enable,
		 icons[icon].bitmap);
}

#endif /* HAVE_LCD_CHARCELLS */

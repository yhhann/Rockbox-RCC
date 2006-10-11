/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 by wavey@wavey.org
 * RTC config saving code (C) 2002 by hessu@hes.iki.fi
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include "inttypes.h"
#include "config.h"
#include "kernel.h"
#include "thread.h"
#include "action.h"
#include "settings.h"
#include "disk.h"
#include "panic.h"
#include "debug.h"
#include "usb.h"
#include "backlight.h"
#include "lcd.h"
#include "audio.h"
#include "mp3_playback.h"
#include "mpeg.h"
#include "talk.h"
#include "string.h"
#include "ata.h"
#include "fat.h"
#include "power.h"
#include "powermgmt.h"
#include "status.h"
#include "atoi.h"
#include "screens.h"
#include "ctype.h"
#include "file.h"
#include "errno.h"
#include "system.h"
#include "misc.h"
#include "timefuncs.h"
#ifdef HAVE_LCD_BITMAP
#include "icons.h"
#include "font.h"
#include "peakmeter.h"
#include "hwcompat.h"
#endif
#include "lang.h"
#include "language.h"
#include "gwps.h"
#include "powermgmt.h"
#include "bookmark.h"
#include "sprintf.h"
#include "keyboard.h"
#include "version.h"
#include "rtc.h"
#include "sound.h"
#include "rbunicode.h"
#include "dircache.h"
#include "statusbar.h"
#include "splash.h"
#include "list.h"
#ifdef HAVE_LCD_COLOR
#include "backdrop.h"
#endif

#ifdef CONFIG_TUNER
#include "radio.h"
#endif

#if CONFIG_CODEC == MAS3507D
void dac_line_in(bool enable);
#endif
struct user_settings global_settings;
#ifdef HAVE_RECORDING
const char rec_base_directory[] = REC_BASE_DIR;
#endif
#if CONFIG_CODEC == SWCODEC
#include "pcmbuf.h"
#include "pcm_playback.h"
#include "dsp.h"
#endif

#ifdef HAVE_WM8758
#include "eq_menu.h"
#endif

#define CONFIG_BLOCK_VERSION 54
#define CONFIG_BLOCK_SIZE 512
#define RTC_BLOCK_SIZE 44

#ifdef HAVE_LCD_BITMAP
#define MAX_LINES 10
#else
#define MAX_LINES 2
#endif

#ifdef HAVE_REMOTE_LCD
#include "lcd-remote.h"
#endif

long lasttime = 0;
static long config_sector = 0;  /* mark uninitialized */
static unsigned char config_block[CONFIG_BLOCK_SIZE];


/* descriptor for a configuration value */
/* (watch the struct packing and member sizes to keep this small) */
struct bit_entry
{
    /* how many bits within the bitfield (1-32), MSB set if value is signed */
    unsigned char bit_size; /* min 6+1 bit */
    /* how many bytes in the global_settings struct (1,2,4) */
    unsigned char byte_size; /* min 3 bits */
    /* store position in global_settings struct */
    short settings_offset; /* min 9 bit, better 10 */
    /* default value */
    int default_val; /* min 15 bit */
    /* variable name in a .cfg file, NULL if not to be saved */
    const char* cfg_name;
    /* set of values, "rgb" for a color, or NULL for a numerical value */
    const char* cfg_val;
};

/********************************************

Config block as saved on the battery-packed RTC user RAM memory block
of 44 bytes, starting at offset 0x14 of the RTC memory space.

offset  abs
0x00    0x14    "Roc"   header signature: 0x52 0x6f 0x63
0x03    0x17    <version byte: 0x0>
0x04    0x18    start of bit-table
...
0x28,0x29 unused, not reachable by set_bits() without disturbing the next 2
0x2A,0x2B <checksum 2 bytes: xor of 0x00-0x29>

Config memory is reset to 0xff and initialized with 'factory defaults' if
a valid header & checksum is not found. Config version number is only
increased when information is _relocated_ or space is _reused_ so that old
versions can read and modify configuration changed by new versions.
Memory locations not used by a given version should not be
modified unless the header & checksum test fails.

Rest of config block, only saved to disk:
0x2C  start of 2nd bit-table
...
0xA4  (char[20]) FMR Preset file
0xB8  (char[20]) WPS file
0xCC  (char[20]) Lang file
0xE0  (char[20]) Font file
...   (char[20]) RWPS file (on targets supporting a Remote WPS)
...   (char[20]) Main backdrop file (on color LCD targets)

... to 0x200  <unused>

*************************************/

/* The persistence of the global_settings members is now controlled by
   the two tables below, rtc_bits and hd_bits.
   New values can just be added to the end, it will be backwards
   compatible. If you however change order, bitsize, etc. of existing
   entries, you need to bump CONFIG_BLOCK_VERSION to break compatibility.
*/


/* convenience macro for both size and offset of global_settings member */
#define S_O(val) sizeof(global_settings.val), offsetof(struct user_settings, val)
#define SIGNED 0x80 /* for bitsize value with signed attribute */

/* some sets of values which are used more than once, to save memory */
static const char off_on[] = "off,on";
static const char off_on_ask[] = "off,on,ask";
static const char off_number_spell_hover[] = "off,number,spell,hover";
#ifdef HAVE_LCD_BITMAP
static const char graphic_numeric[] = "graphic,numeric";
#endif

#ifdef HAVE_RECORDING
/* keep synchronous to trig_durations and
   trigger_times in settings_apply_trigger */
static const char trig_durations_conf [] =
                  "0s,1s,2s,5s,10s,15s,20s,25s,30s,1min,2min,5min,10min";
#endif

#if defined(CONFIG_BACKLIGHT)
static const char backlight_times_conf [] =
                  "off,on,1,2,3,4,5,6,7,8,9,10,15,20,25,30,45,60,90";
#endif

/* the part of the settings which ends up in the RTC RAM, where available
   (those we either need early, save frequently, or without spinup) */
static const struct bit_entry rtc_bits[] =
{
    /* placeholder, containing the size information */
    {9, 0, 0, 0, NULL, NULL }, /* 9 bit to tell how far this is populated */

    /* # of bits, offset+size, default, .cfg name, .cfg values */
    /* sound */
#if CONFIG_CODEC == MAS3507D
    {8 | SIGNED, S_O(volume), -18, "volume", NULL }, /* -78...+18 */
#else
    {8 | SIGNED, S_O(volume), -25, "volume", NULL }, /* -100...+12 / -84...0 */
#endif
    {8 | SIGNED, S_O(balance), 0, "balance", NULL }, /* -100...100 */
#if CONFIG_CODEC != SWCODEC /* any MAS */
    {5 | SIGNED, S_O(bass), 0, "bass", NULL }, /* -15..+15 / -12..+12 */
    {5 | SIGNED, S_O(treble), 0, "treble", NULL }, /* -15..+15 / -12..+12 */
#elif defined HAVE_UDA1380
    {5, S_O(bass), 0, "bass", NULL }, /* 0..+24 */
    {3, S_O(treble), 0, "treble", NULL }, /* 0..+6 */
#elif defined(HAVE_WM8975) || defined(HAVE_WM8758) \
   || defined(HAVE_WM8731) || defined(HAVE_WM8721)
    {5 | SIGNED, S_O(bass), 0, "bass", NULL }, /* -6..+9 */
    {5 | SIGNED, S_O(treble), 0, "treble", NULL }, /* -6..+9 */
#endif
#if (CONFIG_CODEC == MAS3587F) || (CONFIG_CODEC == MAS3539F)
    {5, S_O(loudness), 0, "loudness", NULL }, /* 0...17 */
    {3, S_O(avc), 0, "auto volume", "off,20ms,2,4,8" },
    {1, S_O(superbass), false, "superbass", off_on },
#endif
    {3, S_O(channel_config), 0, "channels",
        "stereo,mono,custom,mono left,mono right,karaoke" },
    {8, S_O(stereo_width), 100, "stereo width", NULL},
    /* playback */
    {1, S_O(resume), false, "resume", off_on },
    {1, S_O(playlist_shuffle), false, "shuffle", off_on },
    {16 | SIGNED, S_O(resume_index), -1, NULL, NULL },
    {16 | SIGNED, S_O(resume_first_index), 0, NULL, NULL },
    {32 | SIGNED, S_O(resume_offset), -1, NULL, NULL },
    {32 | SIGNED, S_O(resume_seed), -1, NULL, NULL },
    {3, S_O(repeat_mode), REPEAT_ALL, "repeat", "off,all,one,shuffle,ab" },
    /* LCD */
#ifdef HAVE_LCD_CONTRAST
    {6, S_O(contrast), DEFAULT_CONTRAST_SETTING, "contrast", NULL },
#endif
#ifdef CONFIG_BACKLIGHT
    {5, S_O(backlight_timeout), 6, "backlight timeout", backlight_times_conf },
#ifdef CONFIG_CHARGING
    {5, S_O(backlight_timeout_plugged), 11, "backlight timeout plugged",
        backlight_times_conf },
#endif
#endif /* CONFIG_BACKLIGHT */
#ifdef HAVE_LCD_BITMAP
    {1, S_O(invert), false, "invert", off_on },
    {1, S_O(flip_display), false, "flip display", off_on },
    /* display */
    {1, S_O(invert_cursor), true, "invert cursor", off_on },
    {1, S_O(statusbar), true, "statusbar", off_on },
    {1, S_O(scrollbar), true, "scrollbar", off_on },
#if CONFIG_KEYPAD == RECORDER_PAD
    {1, S_O(buttonbar), true, "buttonbar", off_on },
#endif
    {1, S_O(volume_type), 0, "volume display", graphic_numeric },
    {1, S_O(battery_display), 0, "battery display", graphic_numeric },
    {1, S_O(timeformat), 0, "time format", "24hour,12hour" },
#endif /* HAVE_LCD_BITMAP */
    {1, S_O(show_icons), true, "show icons", off_on },
    /* system */
    {4, S_O(poweroff), 10,
        "idle poweroff", "off,1,2,3,4,5,6,7,8,9,10,15,30,45,60" },
    {18, S_O(runtime), 0, NULL, NULL },
    {18, S_O(topruntime), 0, NULL, NULL },
#if MEM > 1
    {15, S_O(max_files_in_playlist), 10000,
        "max files in playlist", NULL }, /* 1000...20000 */
    {14, S_O(max_files_in_dir), 400,
        "max files in dir", NULL }, /* 50...10000 */
#else
    {15, S_O(max_files_in_playlist), 1000,
        "max files in playlist", NULL }, /* 1000...20000 */
    {14, S_O(max_files_in_dir), 200,
        "max files in dir", NULL }, /* 50...10000 */
#endif
    /* battery */
    {12, S_O(battery_capacity), BATTERY_CAPACITY_DEFAULT, "battery capacity",
         NULL }, /* 1500...3200 for NiMH, 2200...3200 for LiIon,
                     500...1500 for Alkaline */
#ifdef CONFIG_CHARGING
    {1, S_O(car_adapter_mode), false, "car adapter mode", off_on },
#endif
    /* tuner */
#ifdef CONFIG_TUNER
    {1, S_O(fm_force_mono), false, "force fm mono", off_on },
    {9, S_O(last_frequency), 0, NULL, NULL }, /* Default: MIN_FREQ */
#endif

#if BATTERY_TYPES_COUNT > 1
    {1, S_O(battery_type), 0, "battery type", "alkaline,nimh" },
#endif

#ifdef HAVE_REMOTE_LCD
    /* remote lcd */
    {6, S_O(remote_contrast), DEFAULT_REMOTE_CONTRAST_SETTING,
        "remote contrast", NULL },
    {1, S_O(remote_invert), false, "remote invert", off_on },
    {1, S_O(remote_flip_display), false, "remote flip display", off_on },
    {5, S_O(remote_backlight_timeout), 6, "remote backlight timeout",
        backlight_times_conf },
#ifdef CONFIG_CHARGING
    {5, S_O(remote_backlight_timeout_plugged), 11,
        "remote backlight timeout plugged", backlight_times_conf },
#endif
#ifdef HAVE_REMOTE_LCD_TICKING
    {1, S_O(remote_reduce_ticking), false, "remote reduce ticking", off_on },
#endif
#endif

#ifdef CONFIG_BACKLIGHT
    {1, S_O(bl_filter_first_keypress), false,
            "backlight filters first keypress", off_on },
#ifdef HAVE_REMOTE_LCD
    {1, S_O(remote_bl_filter_first_keypress), false,
            "backlight filters first remote keypress", off_on },
#endif
#endif /* CONFIG_BACKLIGHT */

    /* new stuff to be added here */
    /* If values are just added to the end, no need to bump the version. */

    /* Current sum of bits: 277 (worst case, but w/o remote lcd) */
    /* Sum of all bit sizes must not grow beyond 288! */
};


/* the part of the settings which ends up in HD sector only */
static const struct bit_entry hd_bits[] =
{
    /* This table starts after the 44 RTC bytes = 352 bits. */
    /* Here we need 11 bits to tell how far this is populated. */

    /* placeholder, containing the size information */
    {11, 0, 0, 0, NULL, NULL }, /* 11 bit to tell how far this is populated */

    /* # of bits, offset+size, default, .cfg name, .cfg values */
    /* more display */
#ifdef CONFIG_BACKLIGHT
    {1, S_O(caption_backlight), false, "caption backlight", off_on },
#endif
#ifdef HAVE_REMOTE_LCD
    {1, S_O(remote_caption_backlight), false,
        "remote caption backlight", off_on },
#endif
#ifdef HAVE_BACKLIGHT_BRIGHTNESS
    {4, S_O(brightness), DEFAULT_BRIGHTNESS_SETTING, "brightness", NULL },
#endif
#ifdef HAVE_BACKLIGHT_PWM_FADING
    /* backlight fading */
    {2, S_O(backlight_fade_in), 1, "backlight fade in", "off,500ms,1s,2s"},
    {3, S_O(backlight_fade_out), 3, "backlight fade out",
        "off,500ms,1s,2s,3s,4s,5s,10s"},
#endif

    {4, S_O(scroll_speed), 9, "scroll speed", NULL }, /* 0...15 */
    {8, S_O(scroll_delay), 100, "scroll delay", NULL }, /* 0...250 */
    {8, S_O(bidir_limit), 50, "bidir limit", NULL }, /* 0...200 */
    
#ifdef HAVE_REMOTE_LCD
    {4, S_O(remote_scroll_speed), 9, "remote scroll speed", NULL }, /* 0...15 */
    {8, S_O(remote_scroll_step), 6, "remote scroll step", NULL }, /* 1...160 */
    {8, S_O(remote_scroll_delay), 100, "remote scroll delay", NULL }, /* 0...250 */
    {8, S_O(remote_bidir_limit), 50, "remote bidir limit", NULL }, /* 0...200 */
#endif

#ifdef HAVE_LCD_BITMAP
    {1, S_O(offset_out_of_view), false, "Screen Scrolls Out Of View", off_on },
#if LCD_WIDTH > 255
    {9, S_O(scroll_step), 6, "scroll step", NULL },
    {9, S_O(screen_scroll_step), 16, "screen scroll step", NULL },
#elif LCD_WIDTH > 127
    {8, S_O(scroll_step), 6, "scroll step", NULL },
    {8, S_O(screen_scroll_step), 16, "screen scroll step", NULL },
#else
    {7, S_O(scroll_step), 6, "scroll step", NULL },
    {7, S_O(screen_scroll_step), 16, "screen scroll step", NULL },
#endif
#endif /* HAVE_LCD_BITMAP */
#ifdef HAVE_LCD_CHARCELLS
    {3, S_O(jump_scroll), 0, "jump scroll", NULL }, /* 0...5 */
    {8, S_O(jump_scroll_delay), 50, "jump scroll delay", NULL }, /* 0...250 */
#endif
    {1, S_O(scroll_paginated), false, "scroll paginated", off_on },

#ifdef HAVE_LCD_COLOR
    {LCD_DEPTH,S_O(fg_color),LCD_DEFAULT_FG,"foreground color","rgb"},
    {LCD_DEPTH,S_O(bg_color),LCD_DEFAULT_BG,"background color","rgb"},
#endif

    /* more playback */
    {1, S_O(play_selected), true, "play selected", off_on },
    {1, S_O(fade_on_stop), true, "volume fade", off_on },
    {4, S_O(ff_rewind_min_step), FF_REWIND_1000,
        "scan min step", "1,2,3,4,5,6,8,10,15,20,25,30,45,60" },
    {4, S_O(ff_rewind_accel), 3, "scan accel", NULL },
#if CONFIG_CODEC == SWCODEC
    {3, S_O(buffer_margin), 0, "antiskip",
        "5s,15s,30s,1min,2min,3min,5min,10min" },
#else
    {3, S_O(buffer_margin), 0, "antiskip", NULL },
#endif
    /* disk */
#ifndef HAVE_MMC
#ifdef HAVE_ATA_POWER_OFF
    {1, S_O(disk_poweroff), false, "disk poweroff", off_on },
#endif
    {8, S_O(disk_spindown), 5, "disk spindown", NULL },
#endif /* HAVE_MMC */

    /* browser */
    {3, S_O(dirfilter), SHOW_SUPPORTED,
        "show files", "all,supported,music,playlists,id3 database" },
    {1, S_O(sort_case), false, "sort case", off_on },
    {1, S_O(browse_current), false, "follow playlist", off_on },
    /* playlist */
    {1, S_O(playlist_viewer_icons), true, "playlist viewer icons", off_on },
    {1, S_O(playlist_viewer_indices), true,
        "playlist viewer indices", off_on },
    {1, S_O(playlist_viewer_track_display), 0,
        "playlist viewer track display", "track name,full path" },
    {2, S_O(recursive_dir_insert), RECURSE_OFF,
        "recursive directory insert", off_on_ask },
    /* bookmarks */
    {3, S_O(autocreatebookmark), BOOKMARK_NO, "autocreate bookmarks",
        "off,on,ask,recent only - on,recent only - ask" },
    {2, S_O(autoloadbookmark), BOOKMARK_NO,
        "autoload bookmarks", off_on_ask },
    {2, S_O(usemrb), BOOKMARK_NO,
        "use most-recent-bookmarks", "off,on,unique only" },
#ifdef HAVE_LCD_BITMAP
    /* peak meter */
    {5, S_O(peak_meter_clip_hold), 16, "peak meter clip hold", /* 0...25 */
        "on,1,2,3,4,5,6,7,8,9,10,15,20,25,30,45,60,90,2min,3min,5min,10min,20min,45min,90min" },
    {5, S_O(peak_meter_hold), 3, "peak meter hold",
        "off,200ms,300ms,500ms,1,2,3,4,5,6,7,8,9,10,15,20,30,1min" },
    {7, S_O(peak_meter_release), 8, "peak meter release", NULL }, /* 0...126 */
    {1, S_O(peak_meter_dbfs), true, "peak meter dbfs", off_on },
    {7, S_O(peak_meter_min), 60, "peak meter min", NULL }, /* 0...100 */
    {7, S_O(peak_meter_max), 0, "peak meter max", NULL }, /* 0...100 */
#endif
#if (CONFIG_CODEC == MAS3587F) || (CONFIG_CODEC == MAS3539F)
    {7, S_O(mdb_strength), 0, "mdb strength", NULL},
    {7, S_O(mdb_harmonics), 0, "mdb harmonics", NULL},
    {9, S_O(mdb_center), 0, "mdb center", NULL},
    {9, S_O(mdb_shape), 0, "mdb shape", NULL},
    {1, S_O(mdb_enable), 0, "mdb enable", off_on},
#endif
#if CONFIG_CODEC == MAS3507D
    {1, S_O(line_in), false, "line in", off_on },
#endif
    /* voice */
    {2, S_O(talk_dir), 0, "talk dir", off_number_spell_hover },
    {2, S_O(talk_file), 0, "talk file", off_number_spell_hover },
    {1, S_O(talk_menu), true, "talk menu", off_on },

    {2, S_O(sort_file), 0, "sort files", "alpha,oldest,newest,type" },
    {2, S_O(sort_dir), 0, "sort dirs", "alpha,oldest,newest" },
    {1, S_O(id3_v1_first), 0, "id3 tag priority", "v2-v1,v1-v2"},

#ifdef HAVE_RECORDING
    /* recording */
    {1, S_O(recscreen_on), false, "recscreen on", off_on },
    {1, S_O(rec_startup), false, "rec screen on startup", off_on },
    {4, S_O(rec_timesplit), 0, "rec timesplit", /* 0...15 */
        "off,00:05,00:10,00:15,00:30,01:00,01:14,01:20,02:00,04:00,06:00,08:00,10:00,12:00,18:00,24:00" },
    {4, S_O(rec_sizesplit), 0, "rec sizesplit", /* 0...15 */
        "off,5MB,10MB,15MB,32MB,64MB,75MB,100MB,128MB,256MB,512MB,650MB,700MB,1GB,1.5GB,1.75GB" },
    {1, S_O(rec_channels), 0, "rec channels", "stereo,mono" },
    {1, S_O(rec_split_type), 0, "rec split type", "Split, Stop" },
    {1, S_O(rec_split_method), 0, "rec split method", "Time,Filesize" },

    {
#if defined(HAVE_SPDIF_IN) || defined(HAVE_FMRADIO_IN)
        2,
#else
        1,
#endif
        S_O(rec_source), 0 /* 0=mic */, "rec source",
        "mic,line"
#ifdef HAVE_SPDIF_IN
        ",spdif"
#endif
#ifdef HAVE_FMRADIO_IN
        ",fmradio"
#endif
    },
    {5, S_O(rec_prerecord_time), 0, "prerecording time", NULL }, /* 0...30 */
    {1, S_O(rec_directory), 0, /* rec_base_directory */
        "rec directory", REC_BASE_DIR ",current" },
#ifdef CONFIG_BACKLIGHT
    {2, S_O(cliplight), 0, "cliplight", "off,main,both,remote" },
#endif 
#if CONFIG_CODEC == MAS3587F
    {4, S_O(rec_mic_gain), 8, "rec mic gain", NULL },
    {4, S_O(rec_left_gain), 2 /* 0dB */, "rec left gain", NULL }, /* 0...15 */
    {4, S_O(rec_right_gain), 2 /* 0dB */, "rec right gain", NULL }, /* 0...15 */
    {3, S_O(rec_frequency), 0, /* 0=44.1kHz */
        "rec frequency", "44,48,32,22,24,16" },
    {3, S_O(rec_quality), 5 /* 192 kBit/s max */, "rec quality", NULL },
    {1, S_O(rec_editable), false, "editable recordings", off_on },
#endif /* CONFIG_CODEC == MAS3587F */

#if CONFIG_CODEC == SWCODEC && defined(HAVE_RECORDING)
#ifdef HAVE_UDA1380
    {8|SIGNED, S_O(rec_mic_gain), 16 /* 8 dB */, "rec mic gain", NULL }, /* -128...+108 */
#endif
#ifdef HAVE_TLV320
    /* TLV320 only has no mic boost or 20db mic boost */
    {1, S_O(rec_mic_gain), 0 /* 0 dB */, "rec mic gain", NULL }, /* 0db or 20db */
#endif
    {8|SIGNED, S_O(rec_left_gain), 0, "rec left gain", NULL }, /* -128...+96 */
    {8|SIGNED, S_O(rec_right_gain), 0, "rec right gain", NULL }, /* -128...+96 */
#if 0 /* Till samplerates are added for SWCODEC */
    {3, S_O(rec_frequency), 0, /* 0=44.1kHz */
        "rec frequency", "44,48,32,22,24,16" },
#else
    {3, S_O(rec_frequency), 0, /* 0=44.1kHz */
        "rec frequency", "44" },
#endif

    {4, S_O(rec_quality), 4 /* MP3 L3 192 kBit/s */, "rec quality", NULL },
#endif /* CONFIG_CODEC == SWCODEC && defined(HAVE_RECORDING) */

    /* values for the trigger */
    {8 | SIGNED, S_O(rec_start_thres), -35, "trigger start threshold", NULL},
    {8 | SIGNED, S_O(rec_stop_thres), -45, "trigger stop threshold", NULL},
    {4, S_O(rec_start_duration), 0, "trigger start duration", trig_durations_conf},
    {4, S_O(rec_stop_postrec), 2, "trigger stop postrec", trig_durations_conf},
    {4, S_O(rec_stop_gap), 1, "trigger min gap", trig_durations_conf},
    {4, S_O(rec_trigger_mode ), 0, "trigger mode", "off,once,repeat"},
#endif /* HAVE_RECORDING */

#ifdef HAVE_SPDIF_POWER
    {1, S_O(spdif_enable), false, "spdif enable", off_on},
#endif

    {2, S_O(next_folder), false, "folder navigation", "off,on,random" },
    {1, S_O(runtimedb), false, "gather runtime data", off_on },

#if CONFIG_CODEC == SWCODEC
    {1, S_O(replaygain), false, "replaygain", off_on },
    {2, S_O(replaygain_type), REPLAYGAIN_ALBUM, "replaygain type",
        "track,album,track shuffle" },
    {1, S_O(replaygain_noclip), false, "replaygain noclip", off_on },
    {8 | SIGNED, S_O(replaygain_preamp), 0, "replaygain preamp", NULL },
    {2, S_O(beep), 0, "beep", "off,weak,moderate,strong" },
    {2, S_O(crossfade), 0, "crossfade", "off,shuffle,track skip,always"},
    {3, S_O(crossfade_fade_in_delay), 0, "crossfade fade in delay", NULL},
    {3, S_O(crossfade_fade_out_delay), 0, "crossfade fade out delay", NULL},
    {4, S_O(crossfade_fade_in_duration), 0, "crossfade fade in duration", NULL},
    {4, S_O(crossfade_fade_out_duration), 0, "crossfade fade out duration", NULL},
    {1, S_O(crossfade_fade_out_mixmode), 0, "crossfade fade out mode", "crossfade,mix"},
    {1, S_O(crossfeed), false, "crossfeed", off_on },
    {6, S_O(crossfeed_direct_gain), 15, "crossfeed direct gain", NULL },
    {7, S_O(crossfeed_cross_gain), 60, "crossfeed cross gain", NULL },
    {8, S_O(crossfeed_hf_attenuation), 160, "crossfeed hf attenuation", NULL },
    {11, S_O(crossfeed_hf_cutoff), 700, "crossfeed hf cutoff", NULL },

    /* equalizer */
    {1, S_O(eq_enabled), false, "eq enabled", off_on },
    {8, S_O(eq_precut), 0, "eq precut", NULL },
    /* 0..32768 Hz */
    {15, S_O(eq_band0_cutoff), 60, "eq band 0 cutoff", NULL },
    {15, S_O(eq_band1_cutoff), 200, "eq band 1 cutoff", NULL },
    {15, S_O(eq_band2_cutoff), 800, "eq band 2 cutoff", NULL },
    {15, S_O(eq_band3_cutoff), 4000, "eq band 3 cutoff", NULL },
    {15, S_O(eq_band4_cutoff), 12000, "eq band 4 cutoff", NULL },
    /* 0..64 (or 0.0 to 6.4) */
    {6, S_O(eq_band0_q), 7, "eq band 0 q", NULL },
    {6, S_O(eq_band1_q), 10, "eq band 1 q", NULL },
    {6, S_O(eq_band2_q), 10, "eq band 2 q", NULL },
    {6, S_O(eq_band3_q), 10, "eq band 3 q", NULL },
    {6, S_O(eq_band4_q), 7, "eq band 4 q", NULL },
    /* -240..240 (or -24db to +24db) */
    {9|SIGNED, S_O(eq_band0_gain), 0, "eq band 0 gain", NULL },
    {9|SIGNED, S_O(eq_band1_gain), 0, "eq band 1 gain", NULL },
    {9|SIGNED, S_O(eq_band2_gain), 0, "eq band 2 gain", NULL },
    {9|SIGNED, S_O(eq_band3_gain), 0, "eq band 3 gain", NULL },
    {9|SIGNED, S_O(eq_band4_gain), 0, "eq band 4 gain", NULL },
#endif

#ifdef HAVE_DIRCACHE
    {1, S_O(dircache), false, "dircache", off_on },
    {22, S_O(dircache_size), 0, NULL, NULL },
#endif
#ifdef HAVE_TC_RAMCACHE
    {1, S_O(tagcache_ram), 0, "tagcache_ram", off_on },
#endif
    {1, S_O(tagcache_autoupdate), 0, "tagcache_autoupdate", off_on },

    {4, S_O(default_codepage), 0, "default codepage",
        "iso8859-1,iso8859-7,iso8859-8,cp1251,iso8859-11,cp1256,iso8859-9,iso8859-2,sjis,gb2312,ksx1001,big5,utf-8,cp1256" },

    {1, S_O(warnon_erase_dynplaylist), false,
        "warn when erasing dynamic playlist", off_on },

#ifdef CONFIG_BACKLIGHT
#ifdef HAS_BUTTON_HOLD
    {2, S_O(backlight_on_button_hold), 0, "backlight on button hold",
        "normal,off,on" },
#endif

#ifdef HAVE_LCD_SLEEP
    {4, S_O(lcd_sleep_after_backlight_off), 3,
        "lcd sleep after backlight off",
        "always,never,5,10,15,20,30,45,60,90" },
#endif
#endif /* CONFIG_BACKLIGHT */

#ifdef HAVE_WM8758
    {1, S_O(eq_hw_enabled), false, "eq hardware enabled", off_on },
    
    {2, S_O(eq_hw_band0_cutoff), 1, "eq hardware band 0 cutoff", "80Hz,105Hz,135Hz,175Hz" },
    {5|SIGNED, S_O(eq_hw_band0_gain), 0, "eq hardware band 0 gain", NULL },

    {2, S_O(eq_hw_band1_center), 1, "eq hardware band 1 center", "230Hz,300Hz,385Hz,500Hz" },
    {1, S_O(eq_hw_band1_bandwidth), 0, "eq hardware band 1 bandwidth", "narrow,wide" },
    {5|SIGNED, S_O(eq_hw_band1_gain), 0, "eq hardware band 1 gain", NULL },

    {2, S_O(eq_hw_band2_center), 1, "eq hardware band 2 center", "650Hz,850Hz,1.1kHz,1.4kHz" },
    {1, S_O(eq_hw_band2_bandwidth), 0, "eq hardware band 2 bandwidth", "narrow,wide" },
    {5|SIGNED, S_O(eq_hw_band2_gain), 0, "eq hardware band 2 gain", NULL },

    {2, S_O(eq_hw_band3_center), 1, "eq hardware band 3 center", "1.8kHz,2.4kHz,3.2kHz,4.1kHz" },
    {1, S_O(eq_hw_band3_bandwidth), 0, "eq hardware band 3 bandwidth", "narrow,wide" },
    {5|SIGNED, S_O(eq_hw_band3_gain), 0, "eq hardware band 3 gain", NULL },

    {2, S_O(eq_hw_band4_cutoff), 1, "eq hardware band 4 cutoff", "5.3kHz,6.9kHz,9kHz,11.7kHz" },
    {5|SIGNED, S_O(eq_hw_band4_gain), 0, "eq hardware band 4 gain", NULL },
#endif
    {1, S_O(hold_lr_for_scroll_in_list), true, "hold_lr_for_scroll_in_list", off_on },

    {2, S_O(show_path_in_browser), 0, "show path in browser", "off,current directory,full path" },
#ifdef HAVE_AGC
    {4, S_O(rec_agc_preset_mic), 1, "agc mic preset", NULL}, /* 0...5 */
    {4, S_O(rec_agc_preset_line), 1, "agc line preset", NULL}, /* 0...5 */
    {8|SIGNED, S_O(rec_agc_maxgain_mic), 104, "agc maximum mic gain", NULL},
    {8|SIGNED, S_O(rec_agc_maxgain_line), 96, "agc maximum line gain", NULL},
    {3, S_O(rec_agc_cliptime), 1, "agc cliptime", "0.2s,0.4s,0.6s,0.8,1s"},
#endif

#ifdef HAVE_REMOTE_LCD
#ifdef HAS_REMOTE_BUTTON_HOLD
    {2, S_O(remote_backlight_on_button_hold), 0, "remote backlight on button hold",
        "normal,off,on" },
#endif
#endif

#ifdef HAVE_HEADPHONE_DETECTION
    {2, S_O(unplug_mode), 0, "pause on headphone unplug", NULL},
    {4, S_O(unplug_rw), 0, "rewind duration on pause", NULL},
    {1, S_O(unplug_autoresume), 0, "disable autoresume if phones not present", off_on },
#endif
#ifdef CONFIG_TUNER
    {2, S_O(fm_region), 0, "fm_region", "eu,us,jp,kr" },
#endif

    /* If values are just added to the end, no need to bump the version. */
    /* new stuff to be added at the end */

    /* Sum of all bit sizes must not grow beyond 0xB8*8 = 1472 */
};

/* helper function to extract n (<=32) bits from an arbitrary position
 * counting from LSB to MSB */
static uint32_t get_bits(
    const uint32_t *p, /* the start of the bitfield array */
    unsigned int from, /* bit no. to start reading from */
    unsigned int size) /* how many bits to read */
{
    unsigned int long_index = from / 32;
    unsigned int bit_index = from % 32;
    uint32_t result;

    result = p[long_index] >> bit_index;

    if (bit_index + size > 32)     /* crossing longword boundary */
        result |= p[long_index+1] << (32 - bit_index);

    result &= 0xFFFFFFFF >> (32 - size);

    return result;
}

/* helper function to set n (<=32) bits to an arbitrary position,
 * counting from LSB to MSB */
static void set_bits(
    uint32_t *p,       /* the start of the bitfield array */
    unsigned int from, /* bit no. to start writing into */
    unsigned int size, /* how many bits to change */
    uint32_t value)    /* content (LSBs will be taken) */
{
    unsigned int long_index = from / 32;
    unsigned int bit_index = from % 32;
    uint32_t mask;

    mask = 0xFFFFFFFF >> (32 - size);
    value &= mask;
    mask <<= bit_index;

    if (bit_index + size > 32)
        p[long_index+1] =
            (p[long_index+1] & (0xFFFFFFFF << (bit_index + size - 32)))
            | (value >> (32 - bit_index));

    p[long_index] = (p[long_index] & ~mask) | (value << bit_index);
}

#ifdef HAVE_LCD_COLOR
/*
 * Helper function to convert a string of 6 hex digits to a native colour
 */

#define hex2dec(c) (((c) >= '0' && ((c) <= '9')) ? (toupper(c)) - '0' : \
                                                   (toupper(c)) - 'A' + 10)

int hex_to_rgb(const char* hex)
{   int ok = 1;
    int i;
    int red, green, blue;

    if (strlen(hex) == 6) {
        for (i=0; i < 6; i++ ) {
           if (!isxdigit(hex[i])) {
              ok=0;
              break;
           }
        }

        if (ok) {
            red = (hex2dec(hex[0]) << 4) | hex2dec(hex[1]);
            green = (hex2dec(hex[2]) << 4) | hex2dec(hex[3]);
            blue = (hex2dec(hex[4]) << 4) | hex2dec(hex[5]);
            return LCD_RGBPACK(red,green,blue);
        }
    }

    return 0;
}
#endif

/*
 * Calculates the checksum for the config block and returns it
 */

static unsigned short calculate_config_checksum(const unsigned char* buf)
{
    unsigned int i;
    unsigned char cksum[2];
    cksum[0] = cksum[1] = 0;

    for (i=0; i < RTC_BLOCK_SIZE - 2; i+=2 ) {
        cksum[0] ^= buf[i];
        cksum[1] ^= buf[i+1];
    }

    return (cksum[0] << 8) | cksum[1];
}

/*
 * initialize the config block buffer
 */
static void init_config_buffer( void )
{
    DEBUGF( "init_config_buffer()\n" );

    /* reset to 0 - all unused */
    memset(config_block, 0, CONFIG_BLOCK_SIZE);
    /* insert header */
    config_block[0] = 'R';
    config_block[1] = 'o';
    config_block[2] = 'c';
    config_block[3] = CONFIG_BLOCK_VERSION;
}

/*
 * save the config block buffer to disk or RTC RAM
 */
static int save_config_buffer( void )
{
    unsigned short chksum;
#ifdef HAVE_RTC_RAM
    unsigned int i;
#endif

    /* update the checksum in the end of the block before saving */
    chksum = calculate_config_checksum(config_block);
    config_block[ RTC_BLOCK_SIZE - 2 ] = chksum >> 8;
    config_block[ RTC_BLOCK_SIZE - 1 ] = chksum & 0xff;

#ifdef HAVE_RTC_RAM
    /* FIXME: okay, it _would_ be cleaner and faster to implement rtc_write so
       that it would write a number of bytes at a time since the RTC chip
       supports that, but this will have to do for now 8-) */
    for (i=0; i < RTC_BLOCK_SIZE; i++ ) {
        int r = rtc_write(0x14+i, config_block[i]);
        if (r) {
            DEBUGF( "save_config_buffer: rtc_write failed at addr 0x%02x: %d\n",
                    14+i, r );
            return r;
        }
    }

#endif

    if (config_sector != 0)
        ata_delayed_write( config_sector, config_block);
    else
        return -1;

    return 0;
}

/*
 * load the config block buffer from disk or RTC RAM
 */
static int load_config_buffer(int which)
{
    unsigned short chksum;
    bool correct = false;


    DEBUGF( "load_config_buffer()\n" );

    if (which & SETTINGS_HD)
    {
        if (config_sector != 0) {
            ata_read_sectors(IF_MV2(0,) config_sector, 1,  config_block);
            /* calculate the checksum, check it and the header */
            chksum = calculate_config_checksum(config_block);

            if (config_block[0] == 'R' &&
                config_block[1] == 'o' &&
                config_block[2] == 'c' &&
                config_block[3] == CONFIG_BLOCK_VERSION &&
                (chksum >> 8) == config_block[RTC_BLOCK_SIZE - 2] &&
                (chksum & 0xff) == config_block[RTC_BLOCK_SIZE - 1])
            {
                DEBUGF( "load_config_buffer: header & checksum test ok\n" );
                correct = true;
            }
        }
    }

#ifdef HAVE_RTC_RAM
    if(!correct)
    {
        /* If the disk sector was incorrect, reinit the buffer */
        memset(config_block, 0, CONFIG_BLOCK_SIZE);
    }

    if (which & SETTINGS_RTC)
    {
        unsigned int i;
        unsigned char rtc_block[RTC_BLOCK_SIZE];

        /* read rtc block */
        for (i=0; i < RTC_BLOCK_SIZE; i++ )
            rtc_block[i] = rtc_read(0x14+i);

        chksum = calculate_config_checksum(rtc_block);

        /* if rtc block is ok, use that */
        if (rtc_block[0] == 'R' &&
            rtc_block[1] == 'o' &&
            rtc_block[2] == 'c' &&
            rtc_block[3] == CONFIG_BLOCK_VERSION &&
            (chksum >> 8) == rtc_block[RTC_BLOCK_SIZE - 2] &&
            (chksum & 0xff) == rtc_block[RTC_BLOCK_SIZE - 1])
        {
            memcpy(config_block, rtc_block, RTC_BLOCK_SIZE);
            correct = true;
        }
    }
#endif

    if ( !correct ) {
        /* if checksum is not valid, clear the config buffer */
        DEBUGF( "load_config_buffer: header & checksum test failed\n" );
        init_config_buffer();
        return -1;
    }

    return 0;
}


/* helper to save content of global_settings into a bitfield,
   as described per table */
static void save_bit_table(const struct bit_entry* p_table, int count, int bitstart)
{
    uint32_t *p_bitfield = (uint32_t *)config_block; /* 32 bit addr. */
    uint32_t value; /* 32 bit content */
    int i;
    const struct bit_entry* p_run = p_table; /* start after the size info */
    int curr_bit = bitstart + p_table->bit_size;
    count--; /* first is excluded from loop */

    for (i=0; i<count; i++)
    {
        p_run++;
        /* could do a memcpy, but that would be endian-dependent */
        switch(p_run->byte_size)
        {
        case 1:
            value = ((uint8_t *)&global_settings)[p_run->settings_offset];
            break;
        case 2:
            value = ((uint16_t *)&global_settings)[p_run->settings_offset/2];
            break;
        case 4:
            value = ((uint32_t *)&global_settings)[p_run->settings_offset/4];
            break;
        default:
            DEBUGF( "save_bit_table: illegal size!\n" );
            continue;
        }
        set_bits(p_bitfield, curr_bit, p_run->bit_size & 0x3F, value);
        curr_bit += p_run->bit_size & 0x3F;
    }
    set_bits(p_bitfield, bitstart, p_table->bit_size, /* write size */
        curr_bit); /* = position after last element */
}

/*
 * figure out the config sector from the partition table and the
 * mounted file system
 */
void settings_calc_config_sector(void)
{
#ifdef SIMULATOR
    config_sector = 61;
#else
    int i;
    long partition_start;
    long sector = 0;

    if (fat_startsector(IF_MV(0)) != 0)    /* There is a partition table */
    {
        sector = 61;
        for (i = 0; i < 4; i++)
        {
            partition_start = disk_partinfo(i)->start;
            if (partition_start != 0 && (partition_start - 2) < sector)
                sector = partition_start - 2;
        }
        if (sector < 0)
            sector = 0;
    }

    config_sector = sector;
#endif
}

/*
 * persist all runtime user settings to disk or RTC RAM
 */
int settings_save( void )
{
    int i;

    {
        int elapsed_secs;

        elapsed_secs = (current_tick - lasttime) / HZ;
        global_settings.runtime += elapsed_secs;
        lasttime += (elapsed_secs * HZ);

        if ( global_settings.runtime > global_settings.topruntime )
            global_settings.topruntime = global_settings.runtime;
    }

    /* serialize scalar values into RTC and HD sector, specified via table */
    save_bit_table(rtc_bits, sizeof(rtc_bits)/sizeof(rtc_bits[0]), 4*8);
    save_bit_table(hd_bits, sizeof(hd_bits)/sizeof(hd_bits[0]), RTC_BLOCK_SIZE*8);

    i = 0xb8;
    strncpy((char *)&config_block[i], (char *)global_settings.wps_file,
            MAX_FILENAME);
    i+= MAX_FILENAME;
    strncpy((char *)&config_block[i], (char *)global_settings.lang_file,
            MAX_FILENAME);
    i+= MAX_FILENAME;
    strncpy((char *)&config_block[i], (char *)global_settings.font_file,
            MAX_FILENAME);
    i+= MAX_FILENAME;
#ifdef HAVE_REMOTE_LCD
    strncpy((char *)&config_block[i], (char *)global_settings.rwps_file,
            MAX_FILENAME);
    i+= MAX_FILENAME;
#endif

#ifdef CONFIG_TUNER
    strncpy((char *)&config_block[i], (char *)global_settings.fmr_file,
            MAX_FILENAME);
    i+= MAX_FILENAME;
#endif

#ifdef HAVE_LCD_COLOR
    strncpy((char *)&config_block[i], (char *)global_settings.backdrop_file,
            MAX_FILENAME);
    i+= MAX_FILENAME;
#endif
#ifdef HAVE_LCD_BITMAP
    strncpy((char *)&config_block[i], (char *)global_settings.kbd_file,
            MAX_FILENAME);
    i+= MAX_FILENAME;
#endif

    if(save_config_buffer())
    {
        lcd_clear_display();
#ifdef HAVE_REMOTE_LCD
        lcd_remote_clear_display();
#endif
#ifdef HAVE_LCD_CHARCELLS
        lcd_puts(0, 0, str(LANG_SETTINGS_SAVE_PLAYER));
        lcd_puts(0, 1, str(LANG_SETTINGS_BATTERY_PLAYER));
#else
        lcd_puts(4, 2, str(LANG_SETTINGS_SAVE_RECORDER));
        lcd_puts(2, 4, str(LANG_SETTINGS_BATTERY_RECORDER));
        lcd_update();
#ifdef HAVE_REMOTE_LCD
        lcd_remote_puts(4, 2, str(LANG_SETTINGS_SAVE_RECORDER));
        lcd_remote_puts(2, 4, str(LANG_SETTINGS_BATTERY_RECORDER));
        lcd_remote_update();
#endif
#endif
        sleep(HZ*2);
        return -1;
    }
    return 0;
}

#ifdef HAVE_LCD_BITMAP
/**
 * Applies the range infos stored in global_settings to
 * the peak meter.
 */
void settings_apply_pm_range(void)
{
    int pm_min, pm_max;

    /* depending on the scale mode (dBfs or percent) the values
       of global_settings.peak_meter_dbfs have different meanings */
    if (global_settings.peak_meter_dbfs)
    {
        /* convert to dBfs * 100          */
        pm_min = -(((int)global_settings.peak_meter_min) * 100);
        pm_max = -(((int)global_settings.peak_meter_max) * 100);
    }
    else
    {
        /* percent is stored directly -> no conversion */
        pm_min = global_settings.peak_meter_min;
        pm_max = global_settings.peak_meter_max;
    }

    /* apply the range */
    peak_meter_init_range(global_settings.peak_meter_dbfs, pm_min, pm_max);
}
#endif /* HAVE_LCD_BITMAP */

void sound_settings_apply(void)
{
    sound_set(SOUND_BASS, global_settings.bass);
    sound_set(SOUND_TREBLE, global_settings.treble);
    sound_set(SOUND_BALANCE, global_settings.balance);
    sound_set(SOUND_VOLUME, global_settings.volume);
#if CONFIG_CODEC == SWCODEC
    channels_set(global_settings.channel_config);
    stereo_width_set(global_settings.stereo_width);
#else
    sound_set(SOUND_CHANNELS, global_settings.channel_config);
    sound_set(SOUND_STEREO_WIDTH, global_settings.stereo_width);
#endif
#if (CONFIG_CODEC == MAS3587F) || (CONFIG_CODEC == MAS3539F)
    sound_set(SOUND_LOUDNESS, global_settings.loudness);
    sound_set(SOUND_AVC, global_settings.avc);
    sound_set(SOUND_MDB_STRENGTH, global_settings.mdb_strength);
    sound_set(SOUND_MDB_HARMONICS, global_settings.mdb_harmonics);
    sound_set(SOUND_MDB_CENTER, global_settings.mdb_center);
    sound_set(SOUND_MDB_SHAPE, global_settings.mdb_shape);
    sound_set(SOUND_MDB_ENABLE, global_settings.mdb_enable);
    sound_set(SOUND_SUPERBASS, global_settings.superbass);
#endif
}

void settings_apply(void)
{
    char buf[64];
#if CONFIG_CODEC == SWCODEC
    int i;
#endif

    sound_settings_apply();

    audio_set_buffer_margin(global_settings.buffer_margin);

#ifdef HAVE_LCD_CONTRAST
    lcd_set_contrast(global_settings.contrast);
#endif
    lcd_scroll_speed(global_settings.scroll_speed);
#ifdef HAVE_REMOTE_LCD
    lcd_remote_set_contrast(global_settings.remote_contrast);
    lcd_remote_set_invert_display(global_settings.remote_invert);
    lcd_remote_set_flip(global_settings.remote_flip_display);
    lcd_remote_scroll_speed(global_settings.remote_scroll_speed);
    lcd_remote_scroll_step(global_settings.remote_scroll_step);
    lcd_remote_scroll_delay(global_settings.remote_scroll_delay * (HZ/10));
    lcd_remote_bidir_scroll(global_settings.remote_bidir_limit);
#ifdef HAVE_REMOTE_LCD_TICKING
    lcd_remote_emireduce(global_settings.remote_reduce_ticking);
#endif
    remote_backlight_set_timeout(global_settings.remote_backlight_timeout);
#ifdef CONFIG_CHARGING
    remote_backlight_set_timeout_plugged(global_settings.remote_backlight_timeout_plugged);
#endif
#ifdef HAS_REMOTE_BUTTON_HOLD
    remote_backlight_set_on_button_hold(global_settings.remote_backlight_on_button_hold);
#endif
#endif /* HAVE_REMOTE_LCD */
#ifdef CONFIG_BACKLIGHT
    backlight_set_timeout(global_settings.backlight_timeout);
#ifdef CONFIG_CHARGING
    backlight_set_timeout_plugged(global_settings.backlight_timeout_plugged);
#endif
#if defined(HAVE_BACKLIGHT_PWM_FADING) && !defined(SIMULATOR)
    backlight_set_fade_in(global_settings.backlight_fade_in);
    backlight_set_fade_out(global_settings.backlight_fade_out);
#endif
#endif
#ifdef HAVE_BACKLIGHT_BRIGHTNESS
    backlight_set_brightness(global_settings.brightness);
#endif
    ata_spindown(global_settings.disk_spindown);
#if (CONFIG_CODEC == MAS3507D) && !defined(SIMULATOR)
    dac_line_in(global_settings.line_in);
#endif
    mpeg_id3_options(global_settings.id3_v1_first);
#ifdef HAVE_ATA_POWER_OFF
    ata_poweroff(global_settings.disk_poweroff);
#endif

    set_poweroff_timeout(global_settings.poweroff);

    set_battery_capacity(global_settings.battery_capacity);
#if BATTERY_TYPES_COUNT > 1
    set_battery_type(global_settings.battery_type);
#endif

#ifdef HAVE_LCD_BITMAP
    lcd_set_invert_display(global_settings.invert);
    lcd_set_flip(global_settings.flip_display);
    button_set_flip(global_settings.flip_display);
    lcd_update(); /* refresh after flipping the screen */
    settings_apply_pm_range();
    peak_meter_init_times(
        global_settings.peak_meter_release, global_settings.peak_meter_hold,
        global_settings.peak_meter_clip_hold);
#endif

#ifdef HAVE_LCD_COLOR
    unload_wps_backdrop();
#endif
    if ( global_settings.wps_file[0] &&
         global_settings.wps_file[0] != 0xff ) {
        snprintf(buf, sizeof buf, WPS_DIR "/%s.wps",
                 global_settings.wps_file);
        wps_data_load(gui_wps[0].data, buf, true);
    }
    else
    {
        wps_data_init(gui_wps[0].data);
    }

#ifdef HAVE_LCD_COLOR
    if ( global_settings.backdrop_file[0] &&
         global_settings.backdrop_file[0] != 0xff ) {
        snprintf(buf, sizeof buf, BACKDROP_DIR "/%s.bmp",
                 global_settings.backdrop_file);
        load_main_backdrop(buf);
    } else {
        unload_main_backdrop();
    }
    show_main_backdrop();

    screens[SCREEN_MAIN].set_foreground(global_settings.fg_color);
    screens[SCREEN_MAIN].set_background(global_settings.bg_color);
#endif

#if defined(HAVE_REMOTE_LCD) && (NB_SCREENS > 1)
    if ( global_settings.rwps_file[0] &&
         global_settings.rwps_file[0] != 0xff ) {
        snprintf(buf, sizeof buf, WPS_DIR "/%s.rwps",
                 global_settings.rwps_file);
        wps_data_load(gui_wps[1].data, buf, true);
    }
    else
        wps_data_init(gui_wps[1].data);
#endif

#ifdef HAVE_LCD_BITMAP
    if ( global_settings.font_file[0] &&
         global_settings.font_file[0] != 0xff ) {
        snprintf(buf, sizeof buf, FONT_DIR "/%s.fnt",
                 global_settings.font_file);
        font_load(buf);
    }
    else
        font_reset();

    if ( global_settings.kbd_file[0] &&
         global_settings.kbd_file[0] != 0xff ) {
        snprintf(buf, sizeof buf, ROCKBOX_DIR "/%s.kbd",
                 global_settings.kbd_file);
        load_kbd(buf);
    }
    else
        load_kbd(NULL);

    lcd_scroll_step(global_settings.scroll_step);
    gui_list_screen_scroll_step(global_settings.screen_scroll_step);
    gui_list_screen_scroll_out_of_view(global_settings.offset_out_of_view);
#else
    lcd_jump_scroll(global_settings.jump_scroll);
    lcd_jump_scroll_delay(global_settings.jump_scroll_delay * (HZ/10));
#endif
    lcd_bidir_scroll(global_settings.bidir_limit);
    lcd_scroll_delay(global_settings.scroll_delay * (HZ/10));

    if ( global_settings.lang_file[0] &&
         global_settings.lang_file[0] != 0xff ) {
        snprintf(buf, sizeof buf, LANG_DIR "/%s.lng",
                 global_settings.lang_file);
        lang_load(buf);
        talk_init(); /* use voice of same language */
    }

    set_codepage(global_settings.default_codepage);

#if CONFIG_CODEC == SWCODEC
    audio_set_crossfade(global_settings.crossfade);
    dsp_set_replaygain(true);
    dsp_set_crossfeed(global_settings.crossfeed);
    dsp_set_crossfeed_direct_gain(global_settings.crossfeed_direct_gain);
    dsp_set_crossfeed_cross_params(global_settings.crossfeed_cross_gain,
                                   global_settings.crossfeed_cross_gain
                                   + global_settings.crossfeed_hf_attenuation,
                                   global_settings.crossfeed_hf_cutoff);

    dsp_set_eq(global_settings.eq_enabled);
    dsp_set_eq_precut(global_settings.eq_precut);
    /* Update all EQ bands */
    for(i = 0; i < 5; i++) {
        dsp_set_eq_coefs(i);
    }
#endif

#ifdef HAVE_WM8758
    eq_hw_enable(global_settings.eq_hw_enabled);
#endif

#ifdef HAVE_SPDIF_POWER
    spdif_power_enable(global_settings.spdif_enable);
#endif

#ifdef CONFIG_BACKLIGHT
    set_backlight_filter_keypress(global_settings.bl_filter_first_keypress);
#ifdef HAVE_REMOTE_LCD
    set_remote_backlight_filter_keypress(global_settings.remote_bl_filter_first_keypress);
#endif
#ifdef HAS_BUTTON_HOLD
    backlight_set_on_button_hold(global_settings.backlight_on_button_hold);
#endif
#ifdef HAVE_LCD_SLEEP
    lcd_set_sleep_after_backlight_off(global_settings.lcd_sleep_after_backlight_off);
#endif
#endif /* CONFIG_BACKLIGHT */
}


/* helper to load global_settings from a bitfield, as described per table */
static void load_bit_table(const struct bit_entry* p_table, int count, int bitstart)
{
    uint32_t *p_bitfield = (uint32_t *)config_block; /* 32 bit addr. */
    uint32_t value; /* 32 bit content */
    int i;
    int maxbit; /* how many bits are valid in the saved part */
    const struct bit_entry* p_run = p_table; /* start after the size info */
    count--; /* first is excluded from loop */
    maxbit = get_bits(p_bitfield, bitstart, p_table->bit_size);
    bitstart += p_table->bit_size;

    for (i=0; i<count; i++)
    {
        int size;
        p_run++;

        size = p_run->bit_size & 0x3F; /* mask off abused bits */
        if (bitstart + size > maxbit)
            break; /* exit if this is not valid any more in bitfield */

        value = get_bits(p_bitfield, bitstart, size);
        bitstart += size;
        if (p_run->bit_size & SIGNED)
        {   // sign extend the read value
            unsigned long mask = 0xFFFFFFFF << (size - 1);
            if (value & mask) /* true if MSB of value is set */
                value |= mask;
        }

        /* could do a memcpy, but that would be endian-dependent */
        switch(p_run->byte_size)
        {
        case 1:
            ((uint8_t *)&global_settings)[p_run->settings_offset] =
                (unsigned char)value;
            break;
        case 2:
            ((uint16_t *)&global_settings)[p_run->settings_offset/2] =
                (unsigned short)value;
            break;
        case 4:
            ((uint32_t *)&global_settings)[p_run->settings_offset/4] =
                (unsigned int)value;
            break;
        default:
            DEBUGF( "load_bit_table: illegal size!\n" );
            continue;
        }
    }
}


/*
 * load settings from disk or RTC RAM
 */
void settings_load(int which)
{
    int i;
    DEBUGF( "reload_all_settings()\n" );

    /* load the buffer from the RTC (resets it to all-unused if the block
       is invalid) and decode the settings which are set in the block */
    if (!load_config_buffer(which))
    {
        /* load scalar values from RTC and HD sector, specified via table */
        if (which & SETTINGS_RTC)
        {
            load_bit_table(rtc_bits, sizeof(rtc_bits)/sizeof(rtc_bits[0]), 4*8);
        }
        if (which & SETTINGS_HD)
        {
            load_bit_table(hd_bits, sizeof(hd_bits)/sizeof(hd_bits[0]),
                RTC_BLOCK_SIZE*8);
        }

#ifdef HAVE_RECORDING
    global_settings.recscreen_on = false;
#endif

#ifdef HAVE_LCD_CONTRAST
        if ( global_settings.contrast < MIN_CONTRAST_SETTING ||
             global_settings.contrast > MAX_CONTRAST_SETTING )
            global_settings.contrast = lcd_default_contrast();
#endif

#ifdef HAVE_LCD_REMOTE
        if (global_settings.remote_contrast < MIN_REMOTE_CONTRAST_SETTING ||
            global_settings.remote_contrast > MAX_REMOTE_CONTRAST_SETTING )
            global_settings.remote_contrast = lcd_remote_default_contrast();
#endif
        i = 0xb8;
        strncpy((char *)global_settings.wps_file, (char *)&config_block[i],
                MAX_FILENAME);
        i+= MAX_FILENAME;
        strncpy((char *)global_settings.lang_file, (char *)&config_block[i],
                MAX_FILENAME);
        i+= MAX_FILENAME;
        strncpy((char *)global_settings.font_file, (char *)&config_block[i],
                MAX_FILENAME);
        i+= MAX_FILENAME;
#ifdef HAVE_REMOTE_LCD
        strncpy((char *)global_settings.rwps_file, (char *)&config_block[i],
                MAX_FILENAME);
        i+= MAX_FILENAME;
#endif

#ifdef CONFIG_TUNER
        strncpy((char *)global_settings.fmr_file, (char *)&config_block[i],
                MAX_FILENAME);
        i+= MAX_FILENAME;
#endif

#ifdef HAVE_LCD_COLOR
        strncpy((char *)global_settings.backdrop_file, (char *)&config_block[i],
                MAX_FILENAME);
        i+= MAX_FILENAME;
#endif
#ifdef HAVE_LCD_BITMAP
        strncpy((char *)global_settings.kbd_file, (char *)&config_block[i],
                MAX_FILENAME);
        i+= MAX_FILENAME;
#endif
    }
}

void set_file(char* filename, char* setting, int maxlen)
{
    char* fptr = strrchr(filename,'/');
    int len;
    int extlen = 0;
    char* ptr;

    if (!fptr)
        return;

    *fptr = 0;
    fptr++;

    len = strlen(fptr);
    ptr = fptr + len;
    while ((*ptr != '.') && (ptr != fptr)) {
        extlen++;
        ptr--;
    }
    if(ptr == fptr) extlen = 0;

    if (strncasecmp(ROCKBOX_DIR, filename ,strlen(ROCKBOX_DIR)) ||
        (len-extlen > maxlen))
        return;

    strncpy(setting, fptr, len-extlen);
    setting[len-extlen]=0;

    settings_save();
}

/* helper to sort a .cfg file entry into a global_settings member,
   as described per table. Returns the position if found, else 0. */
static int load_cfg_table(
    const struct bit_entry* p_table, /* the table which describes the entries */
    int count, /* number of entries in the table, including the first */
    const char* name, /* the item to be searched */
    const char* value, /* the value which got loaded for that item */
    int hint) /* position to start looking */
{
    int i = hint;

    do
    {
        if (p_table[i].cfg_name != NULL && !strcasecmp(name, p_table[i].cfg_name))
        { /* found */
            int val = 0;
            if (p_table[i].cfg_val == NULL)
            {   /* numerical value, just convert the string */
                val = atoi(value);
            }
#if HAVE_LCD_COLOR
            else if (!strncasecmp(p_table[i].cfg_val,"rgb",4))
            {
                val = hex_to_rgb(value);
            }
#endif
            else
            {   /* set of string values, find the index */
                const char* item;
                const char* run;
                int len = strlen(value);

                item = run = p_table[i].cfg_val;

                while(1)
                {
                    /* count the length of the field */
                    while (*run != ',' && *run != '\0')
                        run++;

                    if (!strncasecmp(value, item, MAX(run-item, len)))
                        break; /* match, exit the search */

                    if (*run == '\0') /* reached the end of the choices */
                        return i; /* return the position, but don't update */

                    val++; /* count the item up */
                    run++; /* behind the ',' */
                    item = run;
                }
            }

            /* could do a memcpy, but that would be endian-dependent */
            switch(p_table[i].byte_size)
            {
            case 1:
                ((unsigned char*)&global_settings)[p_table[i].settings_offset] =
                    (unsigned char)val;
                break;
            case 2:
                ((unsigned short*)&global_settings)[p_table[i].settings_offset/2] =
                    (unsigned short)val;
                break;
            case 4:
                ((unsigned int*)&global_settings)[p_table[i].settings_offset/4] =
                    (unsigned int)val;
                break;
            default:
                DEBUGF( "illegal size!" );
                continue;
            }

            return i; /* return the position */
        }

        i++;
        if (i==count)
            i=1; /* wraparound */
    } while (i != hint); /* back where we started, all searched */

    return 0; /* indicate not found */
}


bool settings_load_config(const char* file)
{
    int fd;
    char line[128];

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return false;

    while (read_line(fd, line, sizeof line) > 0)
    {
        char* name;
        char* value;
        const struct bit_entry* table[2] = { rtc_bits, hd_bits };
        const int ta_size[2] = {
            sizeof(rtc_bits)/sizeof(rtc_bits[0]),
            sizeof(hd_bits)/sizeof(hd_bits[0])
        };
        int last_table = 0; /* which table was used last round */
        int last_pos = 1; /* at which position did we succeed */
        int pos; /* currently returned position */

        if (!settings_parseline(line, &name, &value))
            continue;

        /* check for the string values */
        if (!strcasecmp(name, "wps")) {
#ifdef HAVE_LCD_COLOR
            unload_wps_backdrop();
#endif
	    int fd2;
            if ((fd2 = open(value, O_RDONLY)) >= 0) {
		close(fd2);
                set_file(value, (char *)global_settings.wps_file, MAX_FILENAME);
	    }
        }
#if defined(HAVE_REMOTE_LCD) && (NB_SCREENS > 1)
        else if (!strcasecmp(name, "rwps")) {
	    int fd2;
            if ((fd2 = open(value, O_RDONLY)) >= 0) {
		close(fd2);
                set_file(value, (char *)global_settings.rwps_file, MAX_FILENAME);
	    }
        }
#endif
        else if (!strcasecmp(name, "lang")) {
            if (!lang_load(value))
            {
                set_file(value, (char *)global_settings.lang_file, MAX_FILENAME);
                talk_init(); /* use voice of same language */
            }
        }
#ifdef CONFIG_TUNER
        else if (!strcasecmp(name, "fmr")) {
            set_file(value, global_settings.fmr_file, MAX_FILENAME);
        }
#endif
#ifdef HAVE_LCD_BITMAP
        else if (!strcasecmp(name, "font")) {
            if (font_load(value))
                set_file(value, (char *)global_settings.font_file, MAX_FILENAME);
        }
#endif
#ifdef HAVE_LCD_COLOR
        else if (!strcasecmp(name, "backdrop")) {
            if (load_main_backdrop(value)) {
                set_file(value, (char *)global_settings.backdrop_file, MAX_FILENAME);
                show_main_backdrop();
            }
        }
#endif
#ifdef HAVE_LCD_BITMAP
        else if (!strcasecmp(name, "keyboard")) {
            if (!load_kbd(value))
                set_file(value, (char *)global_settings.kbd_file, MAX_FILENAME);
        }
#endif


        /* check for scalar values, using the two tables */
        pos = load_cfg_table(table[last_table], ta_size[last_table],
            name, value, last_pos);
        if (pos) /* success */
        {
            last_pos = pos; /* remember as a position hint for next round */
            continue;
        }

        last_table = 1-last_table; /* try other table */
        last_pos = 1; /* search from start */
        pos = load_cfg_table(table[last_table], ta_size[last_table],
            name, value, last_pos);
        if (pos) /* success */
        {
            last_pos = pos; /* remember as a position hint for next round */
            continue;
        }
    }

    close(fd);
    settings_apply();
    settings_save();
    return true;
}


/* helper to save content of global_settings into a file,
   as described per table */
static void save_cfg_table(const struct bit_entry* p_table, int count, int fd)
{
    long value; /* 32 bit content */
    int i;
    const struct bit_entry* p_run = p_table; /* start after the size info */
    count--; /* first is excluded from loop */

    for (i=0; i<count; i++)
    {
        p_run++;

        if (p_run->cfg_name == NULL)
            continue; /* this value is not to be saved */

        /* could do a memcpy, but that would be endian-dependent */
        switch(p_run->byte_size)
        {
        case 1:
            if (p_run->bit_size & SIGNED) /* signed? */
                value = ((char*)&global_settings)[p_run->settings_offset];
            else
                value = ((unsigned char*)&global_settings)[p_run->settings_offset];
            break;
        case 2:
            if (p_run->bit_size & SIGNED) /* signed? */
                value = ((short*)&global_settings)[p_run->settings_offset/2];
            else
                value = ((unsigned short*)&global_settings)[p_run->settings_offset/2];
            break;
        case 4:
            value = ((unsigned int*)&global_settings)[p_run->settings_offset/4];
            break;
        default:
            DEBUGF( "illegal size!" );
            continue;
        }

        if (p_run->cfg_val == NULL) /* write as number */
        {
            fdprintf(fd, "%s: %ld\r\n", p_run->cfg_name, value);
        }
#ifdef HAVE_LCD_COLOR
        else if (!strcasecmp(p_run->cfg_val, "rgb"))
        {
            fdprintf(fd, "%s: %02x%02x%02x\r\n", p_run->cfg_name,
                                                 (int)RGB_UNPACK_RED(value),
                                                 (int)RGB_UNPACK_GREEN(value),
                                                 (int)RGB_UNPACK_BLUE(value));
        }
#endif
        else /* write as item */
        {
            const char* p = p_run->cfg_val;

            fdprintf(fd, "%s: ", p_run->cfg_name);

            while(value >= 0)
            {
                char c = *p++; /* currently processed char */
                if (c == ',') /* separator */
                    value--;
                else if (c == '\0') /* end of string */
                    break; /* not found */
                else if (value == 0) /* the right place */
                    write(fd, &c, 1); /* char by char, this is lame, OK */
            }

            fdprintf(fd, "\r\n");
            if (p_run->cfg_val != off_on) /* explaination for non-bool */
                fdprintf(fd, "# (possible values: %s)\r\n", p_run->cfg_val);
        }
    }
}


bool settings_save_config(void)
{
    int fd;
    char filename[MAX_PATH];

    create_numbered_filename(filename, ROCKBOX_DIR, "config", ".cfg", 2);

    /* allow user to modify filename */
    while (true) {
        if (!kbd_input(filename, sizeof filename)) {
            fd = creat(filename, O_WRONLY);
            if (fd < 0)
                gui_syncsplash(HZ, true, str(LANG_FAILED));
            else
                break;
        }
        else {
            gui_syncsplash(HZ, true, str(LANG_MENU_SETTING_CANCEL));
            return false;
        }
    }

    fdprintf(fd, "# .cfg file created by rockbox %s - "
                 "http://www.rockbox.org\r\n#\r\n#\r\n# wps / rwps / language"
                 " / font / fmpreset / backdrop \r\n#\r\n", appsversion);

    if (global_settings.wps_file[0] != 0)
        fdprintf(fd, "wps: %s/%s.wps\r\n", WPS_DIR,
                 global_settings.wps_file);

#ifdef HAVE_REMOTE_LCD
    if (global_settings.rwps_file[0] != 0)
        fdprintf(fd, "rwps: %s/%s.rwps\r\n", WPS_DIR,
                 global_settings.rwps_file);
#endif

    if (global_settings.lang_file[0] != 0)
        fdprintf(fd, "lang: %s/%s.lng\r\n", LANG_DIR,
                 global_settings.lang_file);

#ifdef HAVE_LCD_BITMAP
    if (global_settings.font_file[0] != 0)
        fdprintf(fd, "font: %s/%s.fnt\r\n", FONT_DIR,
                 global_settings.font_file);
#endif

#ifdef HAVE_LCD_COLOR
    if (global_settings.backdrop_file[0] != 0)
        fdprintf(fd, "backdrop: %s/%s.bmp\r\n", BACKDROP_DIR,
                 global_settings.backdrop_file);
#endif

#ifdef CONFIG_TUNER
    if (global_settings.fmr_file[0] != 0)
        fdprintf(fd, "fmr: %s/%s.fmr\r\n", FMPRESET_PATH,
                 global_settings.fmr_file);
#endif

#ifdef HAVE_LCD_BITMAP
    if (global_settings.kbd_file[0] != 0)
        fdprintf(fd, "keyboard: %s/%s.kbd\r\n", ROCKBOX_DIR,
                 global_settings.kbd_file);
#endif

    /* here's the action: write values to file, specified via table */
    save_cfg_table(rtc_bits, sizeof(rtc_bits)/sizeof(rtc_bits[0]), fd);
    save_cfg_table(hd_bits, sizeof(hd_bits)/sizeof(hd_bits[0]), fd);

    close(fd);

    gui_syncsplash(HZ, true, str(LANG_SETTINGS_SAVED));
    return true;
}


/* helper to load defaults from table into global_settings members */
static void default_table(const struct bit_entry* p_table, int count)
{
    int i;

    for (i=1; i<count; i++) /* exclude the first, the size placeholder */
    {
        /* could do a memcpy, but that would be endian-dependent */
        switch(p_table[i].byte_size)
        {
        case 1:
            ((unsigned char*)&global_settings)[p_table[i].settings_offset] =
                (unsigned char)p_table[i].default_val;
            break;
        case 2:
            ((unsigned short*)&global_settings)[p_table[i].settings_offset/2] =
                (unsigned short)p_table[i].default_val;
            break;
        case 4:
            ((unsigned int*)&global_settings)[p_table[i].settings_offset/4] =
                (unsigned int)p_table[i].default_val;
            break;
        default:
            DEBUGF( "illegal size!" );
            continue;
        }
    }
}


/*
 * reset all settings to their default value
 */
void settings_reset(void) {

    DEBUGF( "settings_reset()\n" );

    /* read defaults from table(s) into global_settings */
    default_table(rtc_bits, sizeof(rtc_bits)/sizeof(rtc_bits[0]));
    default_table(hd_bits, sizeof(hd_bits)/sizeof(hd_bits[0]));

    /* do some special cases not covered by table */
    global_settings.volume      = sound_default(SOUND_VOLUME);
    global_settings.balance     = sound_default(SOUND_BALANCE);
    global_settings.bass        = sound_default(SOUND_BASS);
    global_settings.treble      = sound_default(SOUND_TREBLE);
    global_settings.channel_config = sound_default(SOUND_CHANNELS);
    global_settings.stereo_width = sound_default(SOUND_STEREO_WIDTH);
#if (CONFIG_CODEC == MAS3587F) || (CONFIG_CODEC == MAS3539F)
    global_settings.loudness    = sound_default(SOUND_LOUDNESS);
    global_settings.avc         = sound_default(SOUND_AVC);
    global_settings.mdb_strength = sound_default(SOUND_MDB_STRENGTH);
    global_settings.mdb_harmonics = sound_default(SOUND_MDB_HARMONICS);
    global_settings.mdb_center = sound_default(SOUND_MDB_CENTER);
    global_settings.mdb_shape = sound_default(SOUND_MDB_SHAPE);
    global_settings.mdb_enable = sound_default(SOUND_MDB_ENABLE);
    global_settings.superbass = sound_default(SOUND_SUPERBASS);
#endif
#ifdef HAVE_LCD_CONTRAST 
    global_settings.contrast = lcd_default_contrast();
#endif
#ifdef HAVE_LCD_REMOTE
    global_settings.remote_contrast = lcd_remote_default_contrast();
#endif

#ifdef CONFIG_TUNER
    global_settings.fmr_file[0] = '\0';
#endif
    global_settings.wps_file[0] = '\0';
#ifdef HAVE_REMOTE_LCD
    global_settings.rwps_file[0] = '\0';
#endif
    global_settings.font_file[0] = '\0';
    global_settings.lang_file[0] = '\0';
#ifdef HAVE_LCD_COLOR
    global_settings.backdrop_file[0] = '\0';

    global_settings.fg_color = LCD_DEFAULT_FG;
    global_settings.bg_color = LCD_DEFAULT_BG;
#endif
#ifdef HAVE_LCD_BITMAP
    global_settings.kbd_file[0] = '\0';
#endif
    global_settings.hold_lr_for_scroll_in_list = true;
}

bool set_bool(const char* string, bool* variable )
{
    return set_bool_options(string, variable,
                            (char *)STR(LANG_SET_BOOL_YES),
                            (char *)STR(LANG_SET_BOOL_NO),
                            NULL);
}

/* wrapper to convert from int param to bool param in set_option */
static void (*boolfunction)(bool);
void bool_funcwrapper(int value)
{
    if (value)
        boolfunction(true);
    else
        boolfunction(false);
}

bool set_bool_options(const char* string, bool* variable,
                      const char* yes_str, int yes_voice,
                      const char* no_str, int no_voice,
                      void (*function)(bool))
{
    struct opt_items names[] = {
        {(unsigned char *)no_str, no_voice},
        {(unsigned char *)yes_str, yes_voice}
    };
    bool result;

    boolfunction = function;
    result = set_option(string, variable, BOOL, names, 2,
                        function ? bool_funcwrapper : NULL);
    return result;
}

void talk_unit(int unit, int value)
{
    if (global_settings.talk_menu)
    {
        if (unit < UNIT_LAST)
        {   /* use the available unit definition */
            talk_value(value, unit, false);
        }
        else
        {   /* say the number, followed by an arbitrary voice ID */
            talk_number(value, false);
            talk_id(unit, true);
        }
    }
}

struct value_setting_data {
    enum optiontype type;
    /* used for "value" settings.. */
    int max;
    int step;
    int voice_unit;
    const char * unit;
    void (*formatter)(char* dest, int dest_length,
                          int variable, const char* unit);
    /* used for BOOL and "choice" settings */
    struct opt_items* options;    
};
    
char * value_setting_get_name_cb(int selected_item,void * data, char *buffer)
{
    struct value_setting_data* cb_data = 
            (struct value_setting_data*)data;
    if (cb_data->type == INT && !cb_data->options)
    {
        int item = cb_data->max -(selected_item*cb_data->step);
        if (cb_data->formatter)
            cb_data->formatter(buffer, MAX_PATH,item,cb_data->unit);
        else
            snprintf(buffer, MAX_PATH,"%d %s",item,cb_data->unit);
    }
    else strcpy(buffer,P2STR(cb_data->options[selected_item].string));
    return buffer;
} 
#define type_fromvoidptr(type, value) \
    (type == INT)? \
        (int)(*(int*)(value)) \
    : \
        (bool)(*(bool*)(value))
bool do_set_setting(const unsigned char* string, void *variable,
                    int nb_items,int selected, 
                    struct value_setting_data *cb_data,
                    void (*function)(int))
{
    int action;
    bool done = false;
    struct gui_synclist lists;
    int oldvalue;
    
    if (cb_data->type == INT)
        oldvalue = *(int*)variable;
    else oldvalue = *(bool*)variable;
    
    gui_synclist_init(&lists,value_setting_get_name_cb,(void*)cb_data,false,1);
    gui_synclist_set_title(&lists, (char*)string, NOICON);
    gui_synclist_set_icon_callback(&lists,NULL);
    gui_synclist_set_nb_items(&lists,nb_items);
    gui_synclist_limit_scroll(&lists,true);
    gui_synclist_select_item(&lists, selected);
    
    if (global_settings.talk_menu)
    {
        if (cb_data->type == INT && !cb_data->options)
            talk_unit(cb_data->voice_unit, *(int*)variable);
        else talk_id(cb_data->options[selected].voice_id, false);
    }
    
    gui_synclist_draw(&lists);
    while (!done)
    {
        
        action = get_action(CONTEXT_LIST,TIMEOUT_BLOCK); 
        if (action == ACTION_NONE)
            continue;
        
        if (gui_synclist_do_button(&lists,action))
        {
            if (global_settings.talk_menu)
            {
                int value;
                if (cb_data->type == INT && !cb_data->options)
                {
                    value = cb_data->max - 
                            gui_synclist_get_sel_pos(&lists)*cb_data->step;
                    talk_unit(cb_data->voice_unit, value);
                }
                else 
                {
                    value = gui_synclist_get_sel_pos(&lists);
                    talk_id(cb_data->options[value].voice_id, false);
                }
            }
            if (cb_data->type == INT && !cb_data->options)
                *(int*)variable = cb_data->max - 
                        gui_synclist_get_sel_pos(&lists)*cb_data->step;
            else if (cb_data->type == BOOL)
                *(bool*)variable = gui_synclist_get_sel_pos(&lists) ? true : false;
            else *(int*)variable = gui_synclist_get_sel_pos(&lists);
        }  
        else if (action == ACTION_STD_CANCEL)
        {
            gui_syncsplash(HZ/2,true,str(LANG_MENU_SETTING_CANCEL));
            if (cb_data->type == INT)
                *(int*)variable = oldvalue;
            else *(bool*)variable = (bool)oldvalue;
            done = true;
        }
        else if (action == ACTION_STD_OK)
        {
            done = true;
        }
        else if(default_event_handler(action) == SYS_USB_CONNECTED)
            return true;
        gui_syncstatusbar_draw(&statusbars, false);
        if ( function )
            function(type_fromvoidptr(cb_data->type,variable));
    }
    return false;
}
bool set_int(const unsigned char* string,
             const char* unit,
             int voice_unit,
             int* variable,
             void (*function)(int),
             int step,
             int min,
             int max,
             void (*formatter)(char*, int, int, const char*) )
{
    struct value_setting_data data = {
        INT,max, step, voice_unit,unit,formatter,NULL };
    return do_set_setting(string,variable,(max-min)/step + 1,
                          (max-*variable)/step, &data,function);
}

/* NOTE: the 'type' parameter specifies the actual type of the variable
   that 'variable' points to. not the value within. Only variables with
   type 'bool' should use parameter BOOL.

   The type separation is necessary since int and bool are fundamentally
   different and bit-incompatible types and can not share the same access
   code. */ 
bool set_option(const char* string, void* variable, enum optiontype type,
                const struct opt_items* options, int numoptions, void (*function)(int))
{
    struct value_setting_data data = {
        type,0, 0, 0,NULL,NULL,(struct opt_items*)options };
    int selected;
    if (type == BOOL)
        selected = *(bool*)variable ? 1 : 0;
    else selected = *(int*)variable;
    return do_set_setting(string,variable,numoptions,
                          selected, &data,function);
}

#ifdef HAVE_RECORDING
/* This array holds the record timer interval lengths, in seconds */
static const unsigned long rec_timer_seconds[] =
{
    0,        /* 0 means OFF */
    5*60,     /* 00:05 */
    10*60,    /* 00:10 */
    15*60,    /* 00:15 */
    30*60,    /* 00:30 */
    60*60,    /* 01:00 */
    74*60,    /* 74:00 */
    80*60,    /* 80:00 */
    2*60*60,  /* 02:00 */
    4*60*60,  /* 04:00 */
    6*60*60,  /* 06:00 */
    8*60*60,  /* 08:00 */
    10L*60*60, /* 10:00 */
    12L*60*60, /* 12:00 */
    18L*60*60, /* 18:00 */
    24L*60*60  /* 24:00 */
};

unsigned int rec_timesplit_seconds(void)
{
    return rec_timer_seconds[global_settings.rec_timesplit];
}

/* This array holds the record size interval lengths, in bytes */
static const unsigned long rec_size_bytes[] =
{
    0,               /* 0 means OFF */
    5*1024*1024,     /* 5MB */
    10*1024*1024,    /* 10MB */
    15*1024*1024,    /* 15MB */
    32*1024*1024,    /* 32MB */
    64*1024*1024,    /* 64MB */
    75*1024*1024,    /* 75MB */
    100*1024*1024,   /* 100MB */
    128*1024*1024,   /* 128MB */
    256*1024*1024,   /* 256MB */
    512*1024*1024,   /* 512MB */
    650*1024*1024,   /* 650MB */
    700*1024*1024,   /* 700MB */
    1024*1024*1024,  /* 1GB */
    1536*1024*1024,  /* 1.5GB */
    1792*1024*1024,  /* 1.75GB  */
};

unsigned long rec_sizesplit_bytes(void)
{
    return rec_size_bytes[global_settings.rec_sizesplit];
}
/*
 * Time strings used for the trigger durations.
 * Keep synchronous to trigger_times in settings_apply_trigger
 */
const char * const trig_durations[TRIG_DURATION_COUNT] =
{
    "0s", "1s", "2s", "5s",
    "10s", "15s", "20s", "25s", "30s",
    "1min", "2min", "5min", "10min"
};

void settings_apply_trigger(void)
{
    /* Keep synchronous to trig_durations and trig_durations_conf*/
    static const long trigger_times[TRIG_DURATION_COUNT] = {
        0, HZ, 2*HZ, 5*HZ,
        10*HZ, 15*HZ, 20*HZ, 25*HZ, 30*HZ,
        60*HZ, 2*60*HZ, 5*60*HZ, 10*60*HZ
    };

    peak_meter_define_trigger(
        global_settings.rec_start_thres,
        trigger_times[global_settings.rec_start_duration],
        MIN(trigger_times[global_settings.rec_start_duration] / 2, 2*HZ),
        global_settings.rec_stop_thres,
        trigger_times[global_settings.rec_stop_postrec],
        trigger_times[global_settings.rec_stop_gap]
    );
}
#endif

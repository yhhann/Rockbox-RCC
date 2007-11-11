/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2007 by Dave Chapman
 *
 * Based on merge0.cpp by James Espinoza, but completely rewritten.
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

/*

Description of nk.bin from 

http://www.xs4all.nl/~itsme/projects/xda/wince-flashfile-formats.html

these files contain most information, several non-contigouos blocks
may be present and an entrypoint in the code.

   1. a 7 character signature "B000FF\n" ( that is with 3 zeroes, and
      ending in a linefeed )
   2. DWORD for image start
   3. DWORD for image length
   4. followd by several records of this format:
         1. DWORD with address where this block is to be flashed to
         2. DWORD with the length of this block
         3. DWORD with the 32 bit checksum of this block, in perl:
            unpack("%32C*", $data);
         4. followed by <length> bytes of data 
   5. the last record has address ZERO, in the length the entrypoint
      into the rom, and ZERO as checksum.


NOTE: The Gigabeat-S nk.bin contains 171 records, plus the EOF record.

mknkboot.c appends two images:

1) A "Disable" image which overwrites a word in the EBoot image
2) Our bootloader image, which has the same load address as nk.exe

*/

/* win32 compatibility */

#ifndef O_BINARY
#define O_BINARY 0
#endif


#define DISABLE_ADDR    0x88065A10 /* in EBoot */
#define DISABLE_INSN    0xe3a00001
#define DISABLE_SUM     (0xe3+0xa0+0x00+0x01)  

static void put_uint32le(uint32_t x, unsigned char* p)
{
    p[0] = x & 0xff;
    p[1] = (x >> 8) & 0xff;
    p[2] = (x >> 16) & 0xff;
    p[3] = (x >> 24) & 0xff;
}

static void usage(void)
{
    printf("Usage: mknkboot <firmware file> <boot file> <output file>\n");

    exit(1);
}

static off_t filesize(int fd) {
    struct stat buf;

    if (fstat(fd,&buf) < 0) {
        perror("[ERR]  Checking filesize of input file");
        return -1;
    } else {
        return(buf.st_size);
    }
}


int main(int argc, char *argv[])
{
    char *infile, *bootfile, *outfile;
    int fdin, fdboot,fdout;
    int i,n;
    int inlength,bootlength,newlength;
    unsigned char* buf;
    unsigned char* boot;
    unsigned char* disable;
    uint32_t sum;
    
    if(argc < 3) {
        usage();
    }

    infile = argv[1];
    bootfile = argv[2];
    outfile = argv[3];

    fdin = open(infile, O_RDONLY|O_BINARY);
    if (fdin < 0)
    {
        perror(infile);
    }

    fdboot = open(bootfile, O_RDONLY|O_BINARY);
    if (fdboot < 0)
    {
        perror(bootfile);
    }

    inlength = filesize(fdin);

    bootlength = filesize(fdboot);

    /* Create buffer for original nk.bin, plus our bootloader (with 12
       byte header), plus the 16-byte "disable record" */

    newlength = inlength + (bootlength + 12) + 16;
    buf = malloc(newlength);

    if (buf==NULL)
    {
        printf("[ERR]  Could not allocate memory, aborting\n");
        return 1;
    }

    /****** STEP 1 - Read original nk.bin into buffer */

    n = read(fdin, buf, inlength);
    if (n != inlength)
    {
        printf("[ERR] Could not read from %s\n",infile);
        return 2;
    }

    /****** STEP 2 - Move EOF record to the new EOF */
    memcpy(buf + newlength - 12, buf + inlength - 12, 12);


    /****** STEP 3 - Create a record to disable the firmware signature
                check in EBoot */
    disable = buf + inlength - 12;

    put_uint32le(DISABLE_ADDR, disable);
    put_uint32le(4,            disable + 4);
    put_uint32le(DISABLE_SUM,  disable + 8);
    put_uint32le(DISABLE_INSN, disable + 12);

    /****** STEP 4 - Read the bootloader binary */
    boot = disable + 16;
    n = read(fdboot, boot + 12, bootlength);
    if (n != bootlength)
    {
        printf("[ERR] Could not read from %s\n",bootfile);
        return 3;
    }

    /****** STEP 5 - Create header for bootloader record */

    /* Calculate simple checksum */
    sum = 0;
    for (i = 0; i < bootlength; i++) {
        sum += boot[12 + i];
    }
    
    put_uint32le(0x88201000, boot); /* nk.exe start address */
    put_uint32le(bootlength, boot + 4);
    put_uint32le(sum, boot + 8);

    /****** STEP 6 -  Now write the output file */

    fdout = open(outfile, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0644);
    if (fdout < 0)
    {
        perror(outfile);
    }

    n = write(fdout, buf, newlength);
    if (n != newlength)
    {
        printf("[ERR] Could not write output file %s\n",outfile);
        return 3;
    }

    close(fdin);
    close(fdboot);
    close(fdout);
    
    return 0;
}

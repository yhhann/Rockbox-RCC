/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 *   Copyright (C) 2007 by Dominik Wenger
 *   $Id$
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "autodetection.h"

#if defined(Q_OS_LINUX)
#include <stdio.h>
#include <mntent.h>
#endif

Autodetection::Autodetection(QObject* parent): QObject(parent)
{

}

bool Autodetection::detect()
{
    m_device = "";
    m_mountpoint = "";

    // Try detection via rockbox.info / rbutil.log
    QStringList mountpoints = getMountpoints();

    for(int i=0; i< mountpoints.size();i++)
    {
        // do the file checking
        QDir dir(mountpoints.at(i));
        if(dir.exists())
        {
            // check logfile first.
            if(QFile(mountpoints.at(i) + "/.rockbox/rbutil.log").exists()) {
                QSettings log(mountpoints.at(i) + "/.rockbox/rbutil.log",
                              QSettings::IniFormat, this);
                if(!log.value("platform").toString().isEmpty()) {
                    m_device = log.value("platform").toString();
                    m_mountpoint = mountpoints.at(i);
                    qDebug() << "rbutil.log detected:" << m_device << m_mountpoint;
                    return true;
                }
            }

            // check rockbox-info.txt afterwards.
            QFile file(mountpoints.at(i) + "/.rockbox/rockbox-info.txt");
            if(file.exists())
            {
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QString line = file.readLine();
                if(line.startsWith("Target: "))
                {
                    line.remove("Target: ");
                    m_device = line.trimmed(); // trim whitespaces
                    m_mountpoint = mountpoints.at(i);
                    qDebug() << "rockbox-info.txt detected:" << m_device << m_mountpoint;
                    return true;
                }
            }
            // check for some specific files in root folder
            QDir root(mountpoints.at(i));
            QStringList rootentries = root.entryList(QDir::Files);
            if(rootentries.contains("archos.mod", Qt::CaseInsensitive))
            {
                // archos.mod in root folder -> Archos Player
                m_device = "player";
                m_mountpoint = mountpoints.at(i);
                return true;
            }
            if(rootentries.contains("ONDIOST.BIN"), Qt::CaseInsensitive)
            {
                // ONDIOST.BIN in root -> Ondio FM
                m_device = "ondiofm";
                m_mountpoint = mountpoints.at(i);
                return true;
            }
            if(rootentries.contains("ONDIOSP.BIN"), Qt::CaseInsensitive)
            {
                // ONDIOSP.BIN in root -> Ondio SP
                m_device = "ondiosp";
                m_mountpoint = mountpoints.at(i);
                return true;
            }
            if(rootentries.contains("ajbrec.ajz"), Qt::CaseInsensitive)
            {
                qDebug() << "it's an archos. further detection needed";
            }
            // detection based on player specific folders
            QStringList rootfolders = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            if(rootfolders.contains("GBSYSTEM"), Qt::CaseInsensitive)
            {
                // GBSYSTEM folder -> Gigabeat
                m_device = "gigabeatf";
                m_mountpoint = mountpoints.at(i);
                return true;
            }
            qDebug() << rootfolders;
        }

    }

    int n;
    //try ipodpatcher
    struct ipod_t ipod;
    n = ipod_scan(&ipod);
    if(n == 1) {
        qDebug() << "Ipod found:" << ipod.modelstr << "at" << ipod.diskname;
        m_device = ipod.targetname;
        m_mountpoint = resolveMountPoint(ipod.diskname);
        return true;
    }

    //try sansapatcher
    struct sansa_t sansa;
    n = sansa_scan(&sansa);
    if(n == 1) {
        qDebug() << "Sansa found:" << "sansae200" << "at" << sansa.diskname;
        m_device = "sansae200";
        m_mountpoint = resolveMountPoint(sansa.diskname);
        return true;
    }

    return false;
}


QStringList Autodetection::getMountpoints()
{
#if defined(Q_OS_WIN32)
    QStringList tempList;
    QFileInfoList list = QDir::drives();
    for(int i=0; i<list.size();i++)
    {
        tempList << list.at(i).absolutePath();
    }
    return tempList;
    
#elif defined(Q_OS_MACX)
    QDir dir("/Volumes");
    return dir.entryList(); 
#elif defined(Q_OS_LINUX)
    QStringList tempList;

    FILE *mn = setmntent("/etc/mtab", "r");
    if(!mn)
        return QStringList("");
    
    struct mntent *ent;
    while((ent = getmntent(mn)))
        tempList << QString(ent->mnt_dir);
    endmntent(mn);
    
    return tempList;
#else
#error Unknown Plattform
#endif
}

QString Autodetection::resolveMountPoint(QString device)
{
    qDebug() << "Autodetection::resolveMountPoint(QString)" << device;

#if defined(Q_OS_LINUX)
    FILE *mn = setmntent("/etc/mtab", "r");
    if(!mn)
        return QString("");
    
    struct mntent *ent;
    while((ent = getmntent(mn))) {
        if(QString(ent->mnt_fsname).startsWith(device)
           && QString(ent->mnt_type).contains("vfat", Qt::CaseInsensitive)) {
            endmntent(mn);
            return QString(ent->mnt_dir);
        }
    }
    endmntent(mn);

#endif
    return QString("");

}

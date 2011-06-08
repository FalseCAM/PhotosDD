/* (C) 2011 FalseCAM
    This file is part of PhotosDD.
    
    PhotosDD is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
    
    PhotosDD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    
    You should have received a copy of the GNU Lesser General Public License
    along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PHOTOSDD_H
#define PHOTOSDD_H
#include <QString>
#include <QWidget>
#include <QCoreApplication>
#include "PhotosDD_global.h"
#include "pluginsdialog.h"
#include "photosddplugin.h"

class PHOTOSDDSHARED_EXPORT PhotosDD {
private:

public:
    PhotosDD();
    ~PhotosDD();
    // Library Constants
    static QString applicationName(){ return QString("PhotosDD");}
    static QString applicationDomain(){ return QString("http://photosdd.sourceforge.net/");}
    static QString applicationVersion(){ return QString("0.2");}
    static QIcon applicationIcon(){ return QIcon(":/icon/photosdd");}
    static QString organizationName(){ return QString("FalseCAM");}
    static QString organizationDomain(){ return QString("www.falsecam.net");}
    static QString author(){ return QString("FalseCAM");}
    
    void configPlugins(QWidget* parent);
    QList<PhotosDDPlugin*> getPlugins();
    static bool download(QString url, QString dir);
    static bool removeDir(const QString &dirName);
    static void about(QWidget* parent);
};

#endif // PHOTOSDD_H

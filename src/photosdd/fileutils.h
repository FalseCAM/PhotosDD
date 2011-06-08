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
#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QObject>
#include <QString>

class FileUtils : public QObject
{
    Q_OBJECT
public:
    explicit FileUtils(QObject *parent = 0);

    static bool removeDir(const QString &dirName);

signals:

public slots:

};

#endif // FILEUTILS_H

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
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QStringList>
#include <QUrl>

class Downloader : public QObject
{
    Q_OBJECT
private:
public:
    explicit Downloader(QObject *parent = 0);

signals:

public slots:
static void download(QStringList list, QString folder);
static bool download(QString url, QString folder);
private slots:
static bool download(QUrl url, QString folder);
static bool downloadFile(QString url, QString folder);
static bool downloadHttp(QUrl url, QString folder);
};

#endif // DOWNLOADER_H

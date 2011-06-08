/* (C) 2011 FalseCAM
    This file is part of PhotosDD plugin File.

    File is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    File is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef File_H
#define File_H

#include <QDomDocument>
#include <QUrl>
#include <QDropEvent>
#include "../../photosdd/photosddplugin.h"

namespace Ui {
    class File;
}
/*
 This class is a plugin for the library PhotosDD.
 It downloads Photos included in a File feed.
 */
class File : public PhotosDDPlugin
{
    Q_OBJECT
    Q_INTERFACES(PhotosDDPlugin)

public:
            File(QWidget *parent = 0);
    ~File();
    QString getName();
    QString getTitle();
    QString getVersion();
    QString getAuthor();
    QString getDescription();
    QIcon getIcon();
    void download();
    QStringList getFiles();

protected:
    // Drag n Drop
    void dragEnterEvent( QDragEnterEvent* event );
    void dropEvent( QDropEvent* event );

private:
    Ui::File *ui;
    // List to store url of images
    QStringList imagesList;
    void addImages(QStringList files);
    void addDir(QString dirString);
    void addRDir(QString dirString);
    bool fileAlreadyAdded(QString file);

    // QString to store last opened files dir
    QString lastOpenFileDir;
signals:
    void progress(int);
    void image_list_Changed(int);

private slots:
    void on_clearListButton_clicked();
    void imageListChanged(int index);
    void refreshList();
    void on_addDirButton_clicked();
    void on_addFilesButton_clicked();
};

#endif // File_H

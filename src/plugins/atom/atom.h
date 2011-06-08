/* (C) 2011 FalseCAM
    This file is part of PhotosDD plugin Atom.

    Atom is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    Atom is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ATOM_H
#define ATOM_H

#include <QDomDocument>
#include <QUrl>
#include "../../photosdd/photosddplugin.h"

namespace Ui {
    class Atom;
}
/*
 This class is a plugin for the library PhotosDD.
 It downloads Photos included in a Atom feed.
 */
class Atom : public PhotosDDPlugin
{
    Q_OBJECT
    Q_INTERFACES(PhotosDDPlugin)

public:
            Atom(QWidget *parent = 0);
    ~Atom();
    QString getName();
    QString getTitle();
    QString getVersion();
    QString getAuthor();
    QString getDescription();
    QIcon getIcon();
    void download();
    QStringList getFiles();

private:
    Ui::Atom *ui;
    QString tempDir;
    QStringList files;

    QStringList parse(QDomDocument document);
    QString parseEntry(const QDomElement &entry);
    QDomDocument get(const QUrl &url);
    QStringList getImagesFromAtom(QString rssUrl);
signals:
    void progress(int);
};

#endif // Atom_H

/* (C) 2011 FalseCAM
 This file is part of PhotosDD plugin Flickr.

 Flickr is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 Flickr is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef FLICKR_H
#define FLICKR_H

#include <QtXml/QDomDocument>
#include <QtCore/QUrl>
#include "photosdd/photosddplugin.h"

namespace Ui {
class Flickr;
}
/*
 This class is a plugin for the library PhotosDD.
 It downloads Photos included in a Flickr feed.
 */
class Flickr: public PhotosDDPlugin {
Q_OBJECT
Q_INTERFACES(PhotosDDPlugin)

public:
	Flickr(QWidget *parent = 0);
	~Flickr();
	QString getName();
	QString getTitle();
	QString getVersion();
	QString getAuthor();
	QString getDescription();
	QIcon getIcon();
	void download();
	QStringList getFiles();

private:
	Ui::Flickr *ui;
	QString tempDir;
	QStringList files;

	QString getOriginalImageURL(QString url);
private slots:
	void loadUrl(QUrl url);
	void loadUrl();signals:
	void process(int);
};

#endif // FLICKR_H

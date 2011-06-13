/* (C) 2011 FalseCAM
 This file is part of PhotosDD plugin Picasa.

 Picasa is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 Picasa is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PICASA_H
#define PICASA_H

#include <QDomDocument>
#include <QUrl>
#include "photosdd/photosddplugin.h"

namespace Ui {
class Picasa;
}
/*
 This class is a plugin for the library PhotosDD.
 It downloads Photos included in a Picasa feed.
 */
class Picasa: public PhotosDDPlugin {
Q_OBJECT
Q_INTERFACES(PhotosDDPlugin)

public:
	Picasa(QWidget *parent = 0);
	~Picasa();
	QString getName();
	QString getTitle();
	QString getVersion();
	QString getAuthor();
	QString getDescription();
	QIcon getIcon();
	void download();
	QStringList getFiles();

private:
	Ui::Picasa *ui;
	QString tempDir;
	QStringList files;

	QString getOriginalImageURL(QString url);
private slots:
	void loadUrl(QUrl url);
	void loadUrl();
	void loadProgress(int progress_);signals:
	void progress(int);
};

#endif // PICASA_H

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
#ifndef PHOTOSDDPLUGIN_H
#define PHOTOSDDPLUGIN_H

#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtCore/QString>

class PhotosDDPlugin: virtual public QWidget {
public:
	virtual ~PhotosDDPlugin() {
	}
	virtual QString getName() = 0;
	virtual QString getTitle() = 0;
	virtual QString getVersion() = 0;
	virtual QString getAuthor() = 0;
	virtual QString getDescription() = 0;
	virtual QIcon getIcon() = 0;
	virtual void download() = 0;
	virtual QStringList getFiles() = 0;
	// signals: void process(int);
};

Q_DECLARE_INTERFACE(PhotosDDPlugin,
		"PhotosDD.PhotosDDPlugin/0.1")
;

#endif // PLUGIN_H

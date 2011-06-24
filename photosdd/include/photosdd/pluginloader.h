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
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QObject>
#include <QList>
#include "photosdd/photosddplugin.h"

/*
 PluginLoader class loads and manages plugins to use in PhotosDD.
 This class is implemented as Singleton.
 */

class PluginLoader: public QObject {
Q_OBJECT
private:
	static PluginLoader* instance;
	QList<PhotosDDPlugin *> pluginList;
	QList<PhotosDDPlugin *> activatedPluginList;
	PluginLoader(const PluginLoader&) {
	}
	PluginLoader(QObject *parent);

public:
	//explicit PluginLoader(QObject *parent = 0){}
	~PluginLoader();

signals:
	void pluginStateChanged();

public slots:

	static PluginLoader& getInstance();
	void addPlugin(PhotosDDPlugin *plugin);
	QList<PhotosDDPlugin *> getPlugins();
	QList<PhotosDDPlugin *> getActivatedPlugins();
	void clearActivated();
	void activatePlugin(QString name);
	void activatePlugins(QStringList list);
	void deactivatePlugin(QString name);
	static void destroy();
	void loadPlugins();
	void loadPlugins(QString dir);
	void unloadPlugins();
	void loadConfig();
	void saveConfig();

};

#endif // PLUGINLOADER_H

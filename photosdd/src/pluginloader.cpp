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
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QSettings>
#include <QMessageBox>
#include "photosdd/pluginloader.h"
#include "photosdd/photosddplugin.h"
#include "photosdd/photosdd.h"

PluginLoader* PluginLoader::instance = 0;

PluginLoader::PluginLoader(QObject *parent = 0) {
	loadPlugins();
	loadConfig();
}

PluginLoader::~PluginLoader() {
	saveConfig();
	unloadPlugins();
}

PluginLoader& PluginLoader::getInstance() {
	if (!instance)
		instance = new PluginLoader(0);
	return *instance;
}

void PluginLoader::destroy() {
	if (instance)
		delete instance;
	instance = 0;
}

void PluginLoader::addPlugin(PhotosDDPlugin *plugin) {
	if (plugin) {
		pluginList.append(plugin);
	}
}

void PluginLoader::clearActivated() {
	activatedPluginList.clear();
}

/*
 Activates plugin by name and version.
 Activated plugins are used.
 */
void PluginLoader::activatePlugin(QString name) {
	qDebug("[PluginLoader] activates plugin: %s", qPrintable(name));
	foreach(PhotosDDPlugin * plugin, pluginList)
		{
			if (!activatedPluginList.contains(plugin)) {
				if (QString(plugin->getName() + " " + plugin->getVersion())
						== name || QString(plugin->getName()) == name) {
					activatedPluginList.append(plugin);
					emit(pluginStateChanged());
				}
			}
		}
}

/*
 Deactivates plugin by name and version.
 */
void PluginLoader::deactivatePlugin(QString name) {
	foreach(PhotosDDPlugin * plugin, activatedPluginList)
		{
			if (QString(plugin->getName() + " " + plugin->getVersion()) == name
					|| QString(plugin->getName()) == name) {
				activatedPluginList.removeOne(plugin);
				emit(pluginStateChanged());
			}
		}
}

void PluginLoader::activatePlugins(QStringList list) {
	for (int i = 0; i < list.size(); i++) {
		activatePlugin(list.at(i));
	}
}

/*
 Loads configuration of activated plugins
 */
void PluginLoader::loadConfig() {
	// standardly load file plugin
	activatePlugin("File");
	// load from settings.
	QSettings settings(PhotosDD::organizationName(),
			PhotosDD::applicationName());
	settings.beginGroup(PhotosDD::applicationName());
	settings.beginGroup("plugins");
	qDebug("[PluginLoader] settings path: %s / %s / %s / %s",
			qPrintable(PhotosDD::organizationName()),
			qPrintable(PhotosDD::applicationName()),
			qPrintable(PhotosDD::applicationName()),
			qPrintable(settings.group()));

	QStringList list;
	list = settings.value("ActivatedPlugins", QStringList()).toStringList();
	activatePlugins(list);
	qDebug("[PluginLoader] loaded config for %s",
			qPrintable(PhotosDD::applicationName()));
}

/*
 Saves configuration of activated plugins
 Windows saves to regestry.
 Unix to config file in home folder
 */
void PluginLoader::saveConfig() {
	QSettings settings(PhotosDD::organizationName(),
			PhotosDD::applicationName());
	settings.beginGroup(PhotosDD::applicationName());
	settings.beginGroup("plugins");
	qDebug("[PluginLoader] settings path: %s / %s / %s / %s",
			qPrintable(PhotosDD::organizationName()),
			qPrintable(PhotosDD::applicationName()),
			qPrintable(PhotosDD::applicationName()),
			qPrintable(settings.group()));

	QStringList stringList; //Temp is the QList<int>
	for (int i = 0; i < this->getActivatedPlugins().size(); i++) {
		stringList.append(
				this->getActivatedPlugins().at(i)->getName() + " "
						+ this->getActivatedPlugins().at(i)->getVersion());
	}
	QVariant list;
	list.setValue(stringList);
	settings.setValue("ActivatedPlugins", list);
	qDebug("[PluginLoader] saved config for %s",
			qPrintable(PhotosDD::applicationName()));
}

/*
 Returns all loaded plugins
 */
QList<PhotosDDPlugin *> PluginLoader::getPlugins() {
	return pluginList;
}

/*
 Returns all plugins that are activated by user / config
 */
QList<PhotosDDPlugin *> PluginLoader::getActivatedPlugins() {
	return activatedPluginList;
}

/*
 Reads all files in plugin dir and loads containing plugins.
 */
void PluginLoader::loadPlugins() {
	// Loads Plugins in plugins dir
	QDir pluginsDir(QCoreApplication::applicationDirPath());
#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug"
			|| pluginsDir.dirName().toLower() == "release") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#endif

	pluginsDir.cd("plugins");

	foreach (QString fileName, pluginsDir.entryList(QDir::Files))
		{
			QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
			QObject *plugin = pluginLoader.instance();
			if (plugin) {
				// loads Plugins
				PhotosDDPlugin *plugin_ = qobject_cast<PhotosDDPlugin *> (
						plugin);
				if (plugin_) {
					addPlugin(plugin_);
				} else {
					delete plugin_;
				}
			}
		}
}

/*
 Removes all plugins from memory.
 */
void PluginLoader::unloadPlugins() {
	qDebug("[PluginLoader] unloads Plugins");
	foreach(PhotosDDPlugin * plugin, pluginList)
		{
			activatedPluginList.removeOne(plugin);
			pluginList.removeOne(plugin);
			delete plugin;
		}
}

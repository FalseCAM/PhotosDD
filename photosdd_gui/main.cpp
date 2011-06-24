/* (C) 2011 FalseCAM
 This file is part of PhotosDD-gui.

 PhotosDD is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 PhotosDD-gui is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("FalseCAM");
	QCoreApplication::setOrganizationDomain("www.falsecam.net");
	QCoreApplication::setApplicationName("PhotosDD");
	QCoreApplication::setApplicationVersion("0.2.0");
	app.setWindowIcon(QIcon(":/icon/logo"));

	// Localization
	QTranslator translator;
	QString filename;
	filename = QString("locale/photosdd_%1").arg(QLocale::system().name());
	qDebug("[Main] locale file: %s", qPrintable(filename));
	translator.load(filename, app.applicationDirPath());
	app.installTranslator(&translator);

	MainWindow w;
	w.show();

	return app.exec();
}

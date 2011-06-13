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
#include <QtCore/QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QEventLoop>
#include <QtCore/QDir>
#include <QtCore/QTranslator>
#include "atom.h"
#include "ui_atom.h"
#include "photosdd/photosdd.h"

Atom::Atom(QWidget *parent) :
	QWidget(parent), ui(new Ui::Atom) {
	// Localization
	QTranslator translator;
	QString filename;
	filename = QString("locale/%1_%2").arg(this->getName(),
			QLocale::system().name());
	translator.load(filename, QCoreApplication::applicationDirPath());
	QCoreApplication::installTranslator(&translator);

	tempDir = QString(
			QDir::tempPath() + "/" + QCoreApplication::applicationName() + "_"
					+ getName());
	ui->setupUi(this);
}

Atom::~Atom() {
	delete ui;
}

QString Atom::getName() {
	return QString("atom");
}
QString Atom::getTitle() {
	return tr("Atom-Feed");
}
QString Atom::getVersion() {
	return QString("1.0");
}
QString Atom::getAuthor() {
	return QString("FalseCAM");
}
QString Atom::getDescription() {
	return tr("Downloads Images included in Atom feeds");
}

QIcon Atom::getIcon() {
	return QIcon(":/icon/atom.png");
}

/*
 This function downloads downloads images to destination directory
 */
void Atom::download() {
	qDebug("[Atom] download starts ..");
	emit
	progress(0);
	QString destination = tempDir;
	QStringList urlList = getImagesFromAtom(ui->urlLineEdit->text());
	emit
	progress(10);
	for (int i = 0; i < urlList.size(); i++) {
		emit progress(10 + 90 / urlList.size() * i);
		if (PhotosDD::download(urlList.at(i), destination)) {
			QString file = destination + QDir::separator() + QFileInfo(
					QUrl(urlList.at(i)).path()).fileName();
			qDebug("[Atom] downloaded file: %s", qPrintable(file));
			files.append(file);
		} else {
			qDebug("[Atom] not downloaded file: %s", qPrintable(urlList.at(i)));
		}
	}
	qDebug("[Atom] download finished");
	emit progress(100);
}

QStringList Atom::getFiles() {
	qDebug("[Atom] returns downloaded files ..");
	return files;
}

QStringList Atom::getImagesFromAtom(QString rssUrl) {

	QStringList list;
	if (rssUrl.isEmpty()) {
		qDebug("[Atom] Error: Url string is empty");
		return list;
	}
	QUrl url(rssUrl);
	if (!url.isValid()) {
		qDebug("[Atom] Error: Invalid URL");
		return list;
	}
	if (url.scheme() != "http") {
		qDebug("[Atom] Error: URL must start with 'http:'");
		return list;
	}
	if (url.path().isEmpty()) {
		qDebug("[Atom] Error: URL has no path");
		return list;
	}

	list = parse(get(rssUrl));
	return list;
}

QStringList Atom::parse(QDomDocument document) {
	qDebug("[Atom] parses DomDocument");
	QStringList list;
	QDomElement root = document.documentElement();
	if (root.tagName() != "feed") {
		qDebug("Incompatible tagName: %s", qPrintable(root.tagName()));
		return list;
	}
	QDomElement entry = root.firstChildElement("entry");
	//entry = entry.firstChildElement("item");
	while (!entry.isNull()) {
		list.append(parseEntry(entry));
		entry = entry.nextSiblingElement();
	}
	return list;
}

QString Atom::parseEntry(const QDomElement &entry) {
	QString str;
	QDomElement link = entry.firstChildElement("link");
	while (!link.isNull()) {
		if (link.attribute("rel") == "enclosure" && link.attribute("type")
				== "image/jpeg") {
			str = link.attribute("href");
		}
		link = link.nextSiblingElement();
	}
	link = entry.firstChildElement("media:content");
	while (!link.isNull()) {
		if (link.attribute("type") == "image/jpeg") {
			str = link.attribute("url");
		}
		link = link.nextSiblingElement();
	}
	qDebug("[Atom] parse entry: %s", qPrintable(str));
	return str;
}

/*
 Downloads Atom feed file from url
 */
QDomDocument Atom::get(const QUrl &url) {
	qDebug("[Atom] downloads feed %s", qPrintable(url.toString()));
	QDomDocument document;
	if (!url.isValid())
		return document;

	if (!url.isValid()) {
		qDebug("[Atom] Error: Invalid URL");
		return document;
	}
	if (url.scheme() != "http") {
		qDebug("[Atom] Error: URL must start with 'http:'");
		return document;
	}
	if (url.path().isEmpty()) {
		qDebug("[Atom] Error: URL has no path");
		return document;
	}

	QNetworkAccessManager mgr;
	QNetworkReply *reply;

	reply = mgr.get(QNetworkRequest(url));

	QEventLoop waiter;
	connect(&mgr, SIGNAL(finished(QNetworkReply*)), &waiter, SLOT(quit()));

	waiter.exec();
	document.setContent(QString(reply->readAll()));
	return document;
}

Q_EXPORT_PLUGIN2(atom, Atom)
;

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
#include <QtCore/QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QEventLoop>
#include <QtCore/QDir>
#include "flickr.h"
#include "ui_flickr.h"
#include "photosdd/photosdd.h"
#include <QtWebkit/QWebPage>
#include <QtWebkit/QWebElementCollection>
#include <QtWebkit/QWebFrame>
#include <QtCore/QRegExp>

Flickr::Flickr(QWidget *parent) :
	QWidget(parent), ui(new Ui::Flickr) {
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
	connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(loadUrl(QUrl)));
	connect(ui->urlLineEdit, SIGNAL(editingFinished()), this, SLOT(loadUrl()));
	ui->webView->load(QUrl("http://www.flickr.com"));
}

Flickr::~Flickr() {
	PhotosDD::removeDir(tempDir);
	delete ui;
}

QString Flickr::getName() {
	return QString("flickr");
}
QString Flickr::getTitle() {
	return tr("Flickr");
}
QString Flickr::getVersion() {
	return QString("1.0");
}
QString Flickr::getAuthor() {
	return QString("FalseCAM");
}
QString Flickr::getDescription() {
	return tr("Downloads Images included in Flickr feeds");
}

QIcon Flickr::getIcon() {
	return QIcon(":/icon/flickr_64.png");
}

/*
 This function downloads downloads images to destination directory
 */
void Flickr::download() {

	qDebug("[Flickr] download starts ..");
	emit
	process(0);
	QStringList urlList;
	QWebElementCollection col;
	QWebFrame* frame = ui->webView->page()->mainFrame();
	col = frame->findAllElements("img");

	// gets list of files to download
	foreach (QWebElement el, col)
		{
			QString url = el.attribute("src", "");
			qDebug("[Flickr] image: %s", qPrintable(url));
			if (!url.isEmpty() && QUrl(url).scheme() != "http") {
				url.prepend(
						"http://"
								+ ui->webView->page()->mainFrame()->url().host());
				qDebug("[Flickr] Url is %s", qPrintable(url));
			}
			if (url.contains("flickr.com/") && (url.endsWith("s.jpg")
					|| url.endsWith("m.jpg")))
				urlList.append(this->getOriginalImageURL(url));
		}

	// downloads files
	QString destination = tempDir;
	emit
	process(10);
	for (int i = 0; i < urlList.size(); i++) {
		emit process(10 + 90 / urlList.size() * i);
		if (PhotosDD::download(urlList.at(i), destination)) {
			QString file = destination + QDir::separator() + QFileInfo(
					QUrl(urlList.at(i)).path()).fileName();
			qDebug("[Flickr] downloaded file: %s", qPrintable(file));
			files.append(file);
		} else {
			qDebug("[Flickr] not downloaded file: %s",
					qPrintable(urlList.at(i)));
		}
	}

	qDebug("[Flickr] download finished");
	emit process(100);
}

QStringList Flickr::getFiles() {
	qDebug("[Flickr] returns downloaded files ..");
	return files;
}

void Flickr::loadUrl(QUrl url) {
	qDebug("[Flickr] loaded url %s", qPrintable(url.host()));
	ui->urlLineEdit->setText(url.host() + url.path());
}

/*
 Converts a flickr small image into flickr original image
 */
QString Flickr::getOriginalImageURL(QString url) {
	qDebug("[Flickr] url is %s", qPrintable(url));
	if (ui->bigRadioButton->isChecked()) {
		url.replace("s.jpg", "b.jpg");
		url.replace("m.jpg", "b.jpg");
	} else if (ui->mediumRadioButton->isChecked()) {
		url.replace("s.jpg", "z.jpg");
		url.replace("m.jpg", "z.jpg");
	} else {
		url.replace("s.jpg", "s.jpg");
		url.replace("m.jpg", "s.jpg");
	}
	qDebug("[Flickr] new url is %s", qPrintable(url));
	return url;
}

void Flickr::loadUrl() {
	qDebug("[Flickr] load url %s", qPrintable(ui->urlLineEdit->text()));
	QString url = ui->urlLineEdit->text();
	if (!url.contains("http"))
		url.prepend("http://");
	ui->webView->load(QUrl(url));
}

Q_EXPORT_PLUGIN2(flickr, Flickr)
;

/* (C) 2011 FalseCAM
 This file is part of PhotosDD plugin Fotocommunity.

 Fotocommunity is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 Fotocommunity is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PhotosDD.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDir>
#include "fotocommunity.h"
#include "ui_fotocommunity.h"
#include "photosdd/photosdd.h"
#include <QWebPage>
#include <QWebElementCollection>
#include <QWebFrame>
#include <QRegExp>

Fotocommunity::Fotocommunity(QWidget *parent) :
	QWidget(parent), ui(new Ui::Fotocommunity) {
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
	connect(ui->urlLineEdit, SIGNAL(editingFinished()), this, SLOT(loadUrl()));
	connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(loadUrl(QUrl)));
	ui->webView->load(QUrl("http://www.fotocommunity.com"));
}

Fotocommunity::~Fotocommunity() {
	PhotosDD::removeDir(tempDir);
	delete ui;
}

QString Fotocommunity::getName() {
	return QString("fotocommunity");
}
QString Fotocommunity::getTitle() {
	return tr("Fotocommunity");
}
QString Fotocommunity::getVersion() {
	return QString("1.0");
}
QString Fotocommunity::getAuthor() {
	return QString("FalseCAM");
}
QString Fotocommunity::getDescription() {
	return tr("Downloads Images included in Fotocommunity feeds");
}

QIcon Fotocommunity::getIcon() {
	return QIcon(":/icon/fotocommunity.png");
}

/*
 This function downloads downloads images to destination directory
 */
void Fotocommunity::download() {
	qDebug("[Flickr] download starts ..");
	emit
	progress(0);
	QStringList urlList;
	QWebElementCollection col;
	QWebFrame* frame = ui->webView->page()->mainFrame();
	col = frame->findAllElements("img");

	foreach (QWebElement el, col)
		{
			QString url = el.attribute("src", "");
			qDebug("[Fotocommunity] image: %s", qPrintable(url));
			if (!url.isEmpty() && QUrl(url).scheme() != "http") {
				url.prepend(
						"http://"
								+ ui->webView->page()->mainFrame()->url().host());
				qDebug("[Fotocommunity] Url is %s", qPrintable(url));
			}
			if (url.contains("http://cdn.fotocommunity.com/") && url.endsWith(
					".jpg"))
				urlList.append(this->getOriginalImageURL(url));
		}

	// downloads files
	QString destination = tempDir;
	emit
	progress(10);
	for (int i = 0; i < urlList.size(); i++) {
		emit progress(10 + 90 / urlList.size() * i);
		if (PhotosDD::download(urlList.at(i), destination)) {
			QString file = destination + QDir::separator() + QFileInfo(
					QUrl(urlList.at(i)).path()).fileName();
			qDebug("[Fotocommunity] downloaded file: %s", qPrintable(file));
			files.append(file);
		} else {
			qDebug("[Fotocommunity] not downloaded file: %s",
					qPrintable(urlList.at(i)));
		}
	}

	qDebug("[Fotocommunity] download finished");
	emit progress(100);
}

QStringList Fotocommunity::getFiles() {
	qDebug("[Fotocommunity] returns downloaded files ..");
	return files;
}

void Fotocommunity::loadUrl(QUrl url) {
	qDebug("[Fotocommunity] loaded url %s", qPrintable(url.host()));
	ui->urlLineEdit->setText(url.host() + url.path());
}

/*
 Converts a fotocommunity small image into fotocommunity original image
 */
QString Fotocommunity::getOriginalImageURL(QString url) {
	qDebug("[Fotocommunity] url is %s", qPrintable(url));
	if (url.startsWith("http://cdn.fotocommunity.com/images/"))
		url.replace(url.lastIndexOf("b"), 1, "a");
	else if (url.startsWith("http://cdn.fotocommunity.com/thumbs/"))
		url.replace("/thumbs/", "/photos/");
	qDebug("[Fotocommunity] new url is %s", qPrintable(url));
	return url;
}

void Fotocommunity::loadUrl() {
	qDebug("[Fotocommunity] load url %s", qPrintable(ui->urlLineEdit->text()));
	QString url = ui->urlLineEdit->text();
	if (!url.contains("http"))
		url.prepend("http://");
	ui->webView->load(QUrl(url));
}

Q_EXPORT_PLUGIN2(fotocommunity, Fotocommunity)
;

/* (C) 2011 FalseCAM
 This file is part of PhotosDD plugin HTML.

 HTML is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 HTML is distributed in the hope that it will be useful,
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
#include "html.h"
#include "ui_html.h"
#include "photosdd/photosdd.h"
#include <QWebPage>
#include <QWebElementCollection>
#include <QWebFrame>

HTML::HTML(QWidget *parent) :
	QWidget(parent), ui(new Ui::HTML) {
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
}

HTML::~HTML() {
	delete ui;
}

QString HTML::getName() {
	return QString("html");
}
QString HTML::getTitle() {
	return tr("HTML");
}
QString HTML::getVersion() {
	return QString("1.0");
}
QString HTML::getAuthor() {
	return QString("FalseCAM");
}
QString HTML::getDescription() {
	return tr("Downloads Images included in HTML feeds");
}

QIcon HTML::getIcon() {
	return QIcon(":/icon/html_64.png");
}

/*
 This function downloads downloads images to destination directory
 */
void HTML::download() {
	qDebug("[HTML] download starts ..");
	emit
	progress(0);
	QStringList urlList;
	QWebElementCollection col;
	QWebFrame* frame = ui->webView->page()->mainFrame();
	col = frame->findAllElements("img");

	foreach (QWebElement el, col)
		{
			QString url = el.attribute("src", "");
			qDebug("[HTML] image: %s", qPrintable(url));
			if (!url.isEmpty() && QUrl(url).scheme() != "http") {
				url.prepend(
						"http://"
								+ ui->webView->page()->mainFrame()->url().host());
				qDebug("[HTML] Url is %s", qPrintable(url));
			}
			urlList.append(url);
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
			qDebug("[HTML] downloaded file: %s", qPrintable(file));
			files.append(file);
		} else {
			qDebug("[HTML] not downloaded file: %s", qPrintable(urlList.at(i)));
		}
	}

	qDebug("[HTML] download finished");
	emit progress(100);
}

QStringList HTML::getFiles() {
	qDebug("[HTML] returns downloaded files ..");
	return files;
}

void HTML::loadUrl() {
	qDebug("[HTML] load url %s", qPrintable(ui->urlLineEdit->text()));
	QString url = ui->urlLineEdit->text();
	if (!url.contains("http"))
		url.prepend("http://");
	ui->webView->load(QUrl(url));
}

Q_EXPORT_PLUGIN2(html, HTML)
;

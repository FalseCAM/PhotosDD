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
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDir>
#include <QWebPage>
#include <QWebElementCollection>
#include <QWebFrame>
#include <QRegExp>
#include "picasa.h"
#include "ui_picasa.h"
#include "../../photosdd/photosdd.h"


Picasa::Picasa(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Picasa)
{
    // Localization
    QTranslator translator;
    QString filename;
    filename = QString("locale/%1_%2").arg(this->getName(), QLocale::system().name());
    translator.load(filename, QCoreApplication::applicationDirPath());
    QCoreApplication::installTranslator(&translator);

    tempDir = QString(QDir::tempPath()+"/"+QCoreApplication::applicationName()+"_"+getName());
    ui->setupUi(this);
    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(loadUrl(QUrl)));
    connect(ui->urlLineEdit, SIGNAL(editingFinished()), this, SLOT(loadUrl()));
    connect(ui->webView, SIGNAL(loadProgress(int)), this, SLOT(loadProgress(int)));
    ui->webView->load(QUrl("http://picasaweb.google.com/lh/explore"));
}

Picasa::~Picasa()
{
    PhotosDD::removeDir(tempDir);
    delete ui;
}

QString Picasa::getName(){
    return QString("picasa");
}
QString Picasa::getTitle(){
    return tr("Picasa");
}
QString Picasa::getVersion(){
    return QString("1.0");
}
QString Picasa::getAuthor(){
    return QString("FalseCAM");
}
QString Picasa::getDescription(){
    return tr("Downloads Images included in Picasa feeds");
}

QIcon Picasa::getIcon(){
    return QIcon(":/icon/picasa_64.png");
}

/*
 This function downloads downloads images to destination directory
 */
void Picasa::download(){
    qDebug("[Flickr] download starts ..");
    emit progress(0);
    QStringList urlList;
    QWebElementCollection col;
    QWebFrame* frame = ui->webView->page()->mainFrame();
    col = frame->findAllElements("img");

    foreach (QWebElement el, col) {
        QString url = el.attribute("src", "");
        qDebug("[Picasa] image: %s", qPrintable(url));
        if(!url.isEmpty() && QUrl(url).scheme() != "http"){
            url.prepend("http://"+ui->webView->page()->mainFrame()->url().host());
            qDebug("[Picasa] Url is %s", qPrintable(url));
        }
        if((url.contains("s128") || url.contains("s144-c")) && (url.endsWith(".jpg") || url.endsWith(".JPG")))
            urlList.append(this->getOriginalImageURL(url));
    }

    // downloads files
    QString destination = tempDir;
    emit progress(10);
    for(int i = 0; i< urlList.size(); i++){
        emit progress(10 + 90 / urlList.size() * i);
        if(PhotosDD::download(urlList.at(i), destination)){
            QString file= destination + QDir::separator() + QFileInfo(QUrl(urlList.at(i)).path()).fileName();
            qDebug("[Picasa] downloaded file: %s" ,qPrintable(file));
            files.append(file);
        }else{
            qDebug("[Picasa] not downloaded file: %s" ,qPrintable(urlList.at(i)));
        }
    }
    qDebug("[Picasa] download finished");
    emit progress(100);
}

QStringList Picasa::getFiles(){
    qDebug("[Picasa] returns downloaded files ..");
    return files;
}

void Picasa::loadUrl(QUrl url){
    qDebug("[Picasa] loaded url %s", qPrintable(url.host()));
    ui->urlLineEdit->setText(url.host()+url.path());
}

/*
 Converts a picasa small image into picasa original image
 */
QString Picasa::getOriginalImageURL(QString url){
    qDebug("[Picasa] url is %s",qPrintable(url));
    if(ui->bigRadioButton->isChecked()){
        url.replace("s128/", "");
        url.replace("s144-c/", "");
    }else if(ui->mediumRadioButton->isChecked()){
        url.replace("s128", "s640");
        url.replace("s144-c/", "s640");
    }else{
        url.replace("s144-c/", "s144");
    }
    qDebug("[Picasa] new url is %s", qPrintable(url));
    return url;
}

void Picasa::loadProgress(int progress_){
    emit progress(progress_);
}

void Picasa::loadUrl(){
    qDebug("[Picasa] load url %s", qPrintable(ui->urlLineEdit->text()));
    QString url = ui->urlLineEdit->text();
    if(!url.contains("http"))
        url.prepend("http://");
    ui->webView->load(QUrl(url));
}

Q_EXPORT_PLUGIN2(picasa, Picasa);

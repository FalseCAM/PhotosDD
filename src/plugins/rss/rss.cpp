/* (C) 2011 FalseCAM
    This file is part of PhotosDD plugin RSS.

    RSS is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    RSS is distributed in the hope that it will be useful,
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
#include "rss.h"
#include "ui_rss.h"
#include "../../photosdd/photosdd.h"

RSS::RSS(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::RSS)
{
    // Localization
    QTranslator translator;
    QString filename;
    filename = QString("locale/%1_%2").arg(this->getName(), QLocale::system().name());
    translator.load(filename, QCoreApplication::applicationDirPath());
    QCoreApplication::installTranslator(&translator);

    tempDir = QString(QDir::tempPath()+"/"+QCoreApplication::applicationName()+"_"+getName());
    ui->setupUi(this);
}

RSS::~RSS()
{
    delete ui;
}

QString RSS::getName(){
    return QString("rss");
}
QString RSS::getTitle(){
    return tr("RSS-Feed");
}
QString RSS::getVersion(){
    return QString("1.0");
}
QString RSS::getAuthor(){
    return QString("FalseCAM");
}
QString RSS::getDescription(){
    return tr("Downloads Images included in rss feeds");
}

QIcon RSS::getIcon(){
    return QIcon(":/icon/rss.png");
}

/*
 This function downloads downloads images to destination directory
 */
void RSS::download(){
    qDebug("[RSS] download starts ..");
    emit progress(0);
    QString destination = tempDir;
    QStringList urlList = getImagesFromRSS(ui->urlLineEdit->text());
    emit progress(10);
    for(int i = 0; i< urlList.size(); i++){
        emit progress(10 + 90 / urlList.size() * i);
        if(PhotosDD::download(urlList.at(i), destination)){
            QString file= destination + QDir::separator() + QFileInfo(QUrl(urlList.at(i)).path()).fileName();
            qDebug("[RSS] downloaded file: %s" ,qPrintable(file));
            files.append(file);
        }else{
            qDebug("[RSS] not downloaded file: %s" ,qPrintable(urlList.at(i)));
        }
    }
    qDebug("[RSS] download finished");
    emit progress(100);
}

QStringList RSS::getFiles(){
    qDebug("[RSS] returns downloaded files ..");
    return files;
}


QStringList RSS::getImagesFromRSS(QString rssUrl){

    QStringList list;
    if(rssUrl.isEmpty()){
        qDebug("[RSS] Error: Url string is empty");
        return list;
    }
    QUrl url(rssUrl);
    if (!url.isValid()) {
        qDebug("[RSS] Error: Invalid URL");
        return list;
    }
    if (url.scheme() != "http") {
        qDebug("[RSS] Error: URL must start with 'http:'");
        return list;
    }
    if (url.path().isEmpty()) {
        qDebug("[RSS] Error: URL has no path");
        return list;
    }

    list = parse(get(rssUrl));
    return list;
}

QStringList RSS::parse(QDomDocument document){
    qDebug("[RSS] parses DomDocument");
    QStringList list;
    QDomElement root = document.documentElement();
    if( root.tagName() != "rss"){
        qDebug("Incompatible tagName: %s", qPrintable(root.tagName()) );
        return list;
    }
    QDomElement entry = root.firstChildElement("channel");
    entry = entry.firstChildElement("item");
    while(!entry.isNull()){
        list.append(parseEntry(entry));
        entry = entry.nextSiblingElement();
    }
    return list;
}

QString RSS::parseEntry(const QDomElement &entry){
    QString str;
    QDomElement link = entry.firstChildElement("enclosure");
    while(!link.isNull()){
        if(link.attribute("type") == "image/jpeg"){
            str = link.attribute("url");
        }
        link = link.nextSiblingElement();
    }
    link = entry.firstChildElement("media:content");
    while(!link.isNull()){
        if(link.attribute("type") == "image/jpeg"){
            str = link.attribute("url");
        }
        link = link.nextSiblingElement();
    }
    qDebug("[RSS] parse entry: %s", qPrintable(str));
    return str;
}

/*
 Downloads RSS feed file from url
 */
QDomDocument RSS::get(const QUrl &url){
    qDebug("[RSS] downloads feed %s", qPrintable(url.toString()));
    QDomDocument document;
    if(!url.isValid()) return document;

    if (!url.isValid()) {
        qDebug("[RSS] Error: Invalid URL");
        return document;
    }
    if (url.scheme() != "http") {
        qDebug("[RSS] Error: URL must start with 'http:'");
        return document;
    }
    if (url.path().isEmpty()) {
        qDebug("[RSS] Error: URL has no path");
        return document;
    }

    QNetworkAccessManager mgr;
    QNetworkReply *reply;

    reply = mgr.get( QNetworkRequest( url));

    QEventLoop waiter;
    connect(  &mgr, SIGNAL(finished(QNetworkReply*)), &waiter, SLOT(quit()));

    waiter.exec();
    document.setContent(QString(reply->readAll()));
    return document;
}

Q_EXPORT_PLUGIN2(rss, RSS);

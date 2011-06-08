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
#include "downloader.h"
#include <QUrl>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

Downloader::Downloader(QObject *parent) :
        QObject(parent)
{
}

void Downloader::download(QStringList list, QString folder){
    // downloads images of list into folder
    Downloader dl;
    for( int i = 0; i< list.length(); i++){
        dl.download(QUrl(list.at(i)), folder);
    }
}

bool Downloader::download(QString url, QString folder){
    Downloader dl;
    if( QFile(url).exists()){
        qDebug("file: %s", qPrintable(url));
        qDebug("file exists");
        return downloadFile(url, folder);
    }else{
        return dl.download(QUrl(url),folder);
    }
}

bool Downloader::download(QUrl url, QString folder){
    if(url.scheme() == "http")
        return downloadHttp(url, folder);
    else if(url.scheme() == "file")
        return downloadFile(url.toLocalFile(), folder);
    else{
        qDebug("[Downloader] unknown scheme (%s)", qPrintable(url.scheme()));
        return false;
    }
}

bool Downloader::downloadFile(QString url, QString folder){
    qDebug("[Downloader] downloads %s to %s", qPrintable(url), qPrintable(folder));

    /*
     Creates destination folder if does not exist.
     */
    QDir dir(folder);
    if (!dir.exists()){
        if (!dir.mkdir(dir.path()))
        {
            qDebug("[Downloader] Could not create directory %s", qPrintable(folder));
        }else{
            qDebug("[Downloader] directory %s created", qPrintable(folder));
        }
    }

    QString localFileName = folder + QDir::separator() + QFileInfo(url).fileName();
    qDebug("[Downloader] download file is %s", qPrintable(localFileName));
    if (localFileName.isEmpty())
        localFileName = folder + QDir::separator() +"empty.jpg";
    qDebug("[Downloader] local file name is %s", qPrintable(localFileName));
    QFile file(localFileName);
    if(file.exists()) return true;
    if(url.startsWith(QDir::tempPath())){
        if(QFile::rename(url, localFileName))
            return true;
    }
    QFile f(url);
    bool ret = f.copy(localFileName);
    if(ret == false)qDebug("[Downloader] file error: %s", qPrintable(f.errorString()));
    return ret;
}

bool Downloader::downloadHttp(QUrl url, QString folder){
    qDebug("[Downloader] downloads %s to %s", qPrintable(url.toString()), qPrintable(folder));

    if (!url.isValid()) {
        qDebug("[Downloader] Error: Invalid URL");
        return false;
    }

    if (url.scheme() != "http") {
        qDebug("[Downloader] Error: URL must start with 'http:'");
        return false;
    }

    if (url.path().isEmpty()) {
        qDebug("[Downloader] Error: URL has no path");
        return false;
    }

    QDir dir(folder);
    if (!dir.exists()){
        if (!dir.mkdir(dir.path()))
        {
            qDebug("[Downloader] Could not create directory %s", qPrintable(folder));
        }else{
            qDebug("[Downloader] directory %s created", qPrintable(folder));
        }
    }

    QString localFileName = folder + QDir::separator() + QFileInfo(url.path()).fileName();
    qDebug("[Downloader] download file is %s", qPrintable(localFileName));
    if (localFileName.isEmpty())
        localFileName = "empty.jpg";
    qDebug("[Downloader] local file name is %s", qPrintable(localFileName));
    QFile file(localFileName);
    if(file.exists()) return true;
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug("[Downloader] Error: Cannot write file %s: %s",
               qPrintable(file.fileName()), qPrintable(file.errorString()));
        return false;
    }

    QNetworkAccessManager mgr;
    QNetworkReply *reply;

    reply = mgr.get( QNetworkRequest( url));

    QEventLoop waiter;
    connect(  &mgr, SIGNAL(finished(QNetworkReply*)), &waiter, SLOT(quit()));

    waiter.exec();
    qDebug("[Downloader] Reply error: %s",qPrintable(reply->errorString()));

    file.write(reply->readAll());
    file.close();
    return true;
}

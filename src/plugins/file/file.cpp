/* (C) 2011 FalseCAM
    This file is part of PhotosDD plugin File.

    File is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    File is distributed in the hope that it will be useful,
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
#include <QFileDialog>
#include "file.h"
#include "ui_file.h"
#include "../../photosdd/photosdd.h"

File::File(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::File)
{
    // Localization
    QTranslator translator;
    QString filename;
    filename = QString("locale/%1_%2").arg(this->getName(), QLocale::system().name());
    translator.load(filename, QCoreApplication::applicationDirPath());
    QCoreApplication::installTranslator(&translator);

    ui->setupUi(this);
    connect(this, SIGNAL(image_list_Changed(int)), this, SLOT(imageListChanged(int)));
    connect(ui->ShowIconCheckBox, SIGNAL(toggled(bool)), this, SLOT(refreshList()));
    this->setAcceptDrops(true);
}

File::~File()
{
    delete ui;
}

QString File::getName(){
    return QString("file");
}
QString File::getTitle(){
    return tr("File");
}
QString File::getVersion(){
    return QString("1.0");
}
QString File::getAuthor(){
    return QString("FalseCAM");
}
QString File::getDescription(){
    return tr("Gets files from computer.");
}

QIcon File::getIcon(){
    return QIcon(":/icon/file.png");
}

/*
 This function downloads downloads images to destination directory
 */
void File::download(){
    qDebug("[File] download starts ..");
    qDebug("[File] download finished");
    emit progress(100);
}

QStringList File::getFiles(){
    qDebug("[File] returns downloaded files ..");
    return this->imagesList;
}


// Add Images to Image List
void File::addImages(QStringList files)
{
    // Check if a file exists
    if( !files.isEmpty())
    {
        for(int i = 0; i< files.count(); i++){
            //foreach(QString file, files){
            emit progress(int((i+1)*100/files.count()) );
            if(QFile().exists(files.at(i)) && !fileAlreadyAdded(files.at(i)))
            {
                imagesList.append(files.at(i));
                emit image_list_Changed(this->imagesList.count()-1 );
            }
        }
        emit progress(0);
    }
}

void File::addDir(QString dirString){
    if(QDir().exists(dirString)){
        QDir dir = QDir(dirString);
        // Set Filter to only load Image Files
        dir.setNameFilters(QStringList() << QString("*.bmp")
                           << QString("*.jpg")
                           << QString("*.jpeg")
                           << QString("*.png")
                           << QString("*.tif")
                           << QString("*.tiff") );
        QFileInfoList finfo = dir.entryInfoList(QDir::Files );
        QStringList files;
        // generate file list
        for (int i=0; i<finfo.count(); i++){
            files.append( finfo.at(i).absoluteFilePath());
        }
        // adds files to Image List
        this->addImages(files);
    }
}

void File::addRDir(QString dirString){
    if(QDir().exists(dirString)){
        QDir dir = QDir(dirString);
        QFileInfoList finfo = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
        for(int i=0; i<finfo.count(); i++){
            addRDir( finfo.at(i).absoluteFilePath());
        }
        this->addDir(dirString);
    }
}

bool File::fileAlreadyAdded(QString file){
    if(this->imagesList.contains(file)) return true;
    return false;
}

// loads preview of an Image in Image List
void File::imageListChanged(int index){
    QStringList files = imagesList;
    if(index < 0 || index > files.count())
        return;
    // Process Bar
    emit progress(33);
    // check if file exists
    if(files.at(index)=="") return;
    if( !QFile(files.at(index)).exists() ) return;
    QListWidgetItem *item = new QListWidgetItem(files.at(index));
    if( this->ui->ShowIconCheckBox->isChecked() )
    {
        QIcon icon;
        QPixmap pixmap;
        // make copy of scaled image
        pixmap = QPixmap(files.at(index)).scaledToHeight(48);
        emit progress(66);
        // open Image to use it as Icon
        icon = QIcon(pixmap);
        // Process Bar
        emit progress(100);
        // create List Item
        item->setIcon(icon);
    }
    // add Item to List
    ui->ImageListWidget->addItem(item);
}

// show images in ImageListWidget
void File::refreshList()
{
    ui->ImageListWidget->clear();
    for(int i = 0; i< this->imagesList.size(); i++)
    {
        emit progress((100 / this->imagesList.size()) * i);
        imageListChanged(i);
    }
    // Process Bar
    emit progress(0);
}


// Drag and Drop
void File::dragEnterEvent( QDragEnterEvent* event )
{
    // Drag part
    const QMimeData* md = event->mimeData();
    if( event && ( md->hasUrls() ))
        event->acceptProposedAction();
}

void File::dropEvent( QDropEvent* event)
{
    // Drop part
    if(event && event->mimeData())
    {
        const QMimeData *mData = event->mimeData();
        // Drop Images from FileManager into ImageList
        if( mData->hasUrls())
        {
            QList<QUrl> urls = mData->urls();
            QStringList files;
            for(int x = 0; x<urls.count(); ++x)
            {
                if(QFileInfo(urls.at(x).toLocalFile()).isDir())
                    this->addRDir(urls.at(x).toLocalFile());
                else
                    this->addImages(QStringList(urls.at(x).toLocalFile()));
            }
        }
    }
}

void File::on_addFilesButton_clicked()
{
    // Last open dir is last file dir
    if(imagesList.count()>0)
        this->lastOpenFileDir = QDir(imagesList.last()).absolutePath();
    // Create File Dialog and open selected Files
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Please select a file"), this->lastOpenFileDir,
                                                      "All Supported Image Files (*.bmp *.jpg *.jpeg *.png *.tif *.tiff);;"
                                                      "Joint Photographic Experts Group File (*.jpg *.jpeg);;"
                                                      "All Files (*.*)"
                                                      );
    if(files.isEmpty()) return;
    // checks for existing dir and saves last dialog opened dir
    if( QDir(QFileInfo(files.at(0)).absoluteDir()).exists() )
        this->lastOpenFileDir = QFileInfo(files.at(0)).absoluteDir().absolutePath();

    addImages(files);
}

void File::on_addDirButton_clicked()
{
    // Last open dir is last file dir
    if(imagesList.count()>0)
        this->lastOpenFileDir = QDir(imagesList.last()).absolutePath();
    // Create dir dialog to choose a directory
    QString dirString = QFileDialog::getExistingDirectory(this, tr("Please select a directory"), this->lastOpenFileDir );
    // return if no Dir is chosen or if Dir is non existent
    if(dirString.isEmpty()) return;
    if(!QDir(dirString).exists() ) return;
    // save chosen dir to show the same dir next File Dialog
    this->lastOpenFileDir = dirString;
    addDir(dirString);
}

void File::on_clearListButton_clicked()
{
    this->imagesList.clear();
    this->refreshList();
}

Q_EXPORT_PLUGIN2(file, File);

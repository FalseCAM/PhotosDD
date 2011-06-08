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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../photosdd/photosddplugin.h"
#include "../photosdd/photosdd.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QList>
#include <QIODevice>
#include <QSettings>
#include "tipdialog.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    photosDD = new PhotosDD();
    ui->setupUi(this);
    initSignalSlots();
    loadPluginWidgets();

    this->setWindowTitle(QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion());
    ui->SaveToDirLineEdit->setText(QDir::homePath());

    loadConfig();
    TipDialog(this).startUp();
}
/*
 This class connects actions/signals to functions/slots
 */
void MainWindow::initSignalSlots(){
    // SaveTo selector
    connect(ui->SaveToDirButton, SIGNAL(clicked()), this, SLOT(setSaveToDir()));
    // plugin config
    connect(ui->actionConfig, SIGNAL(triggered()), this, SLOT(configPlugins()));
    // About Boxes in Menu
    connect(ui->actionAbout_PhotosDD_GUI, SIGNAL(triggered()), this, SLOT(aboutPhotosDD_GUI()));
    connect(ui->actionAbout_PhotosDD, SIGNAL(triggered()), this, SLOT(aboutPhotosDD()));
    connect(ui->actionAbout_QT, SIGNAL(triggered()), this, SLOT(aboutQT()));
    // load Plugin Widget
    // shows tips
    connect(ui->actionTips, SIGNAL(triggered()), this, SLOT(showTips()));
}

MainWindow::~MainWindow()
{
    saveConfig();
    delete photosDD;
    delete ui;
}

void MainWindow::loadPluginWidgets(){
    // Adds in PluginLoader loaded plugins to tabWidget
    qDebug("[MainWindow] loads plugin Widgets");

    ui->tabWidget->clear();

    foreach (PhotosDDPlugin *plugin, photosDD->getPlugins()) {
        if(plugin)
            ui->tabWidget->addTab(plugin, plugin->getTitle());
    }
}

void MainWindow::configPlugins(){
    photosDD->configPlugins(this);
    loadPluginWidgets();
}

// set Save Dir
void MainWindow::setSaveToDir()
{
    // opens a Dialog to choose save directory
    QString dir = QFileDialog::getExistingDirectory(
            this,
            "Please select a directory.",
            ui->SaveToDirLineEdit->text() );
    // checks for dir exists
    if( QDir(dir).exists() )
    {
        // save chosen dir
        if(!dir.isEmpty())
            ui->SaveToDirLineEdit->setText(dir);
    }
}

/*
 About boxes that show information for PhotosDD-GUI,
 PhotosDD library and QT library.
 */
void MainWindow::aboutPhotosDD_GUI(){
    QMessageBox aboutBox;
    aboutBox.about(0,tr("About ")+ QCoreApplication::applicationName(),
                   ("<p align=\"right\"><h1>") + QCoreApplication::applicationName() +
                   ("</h1></p>") +
                   ("<p align=\"left\"><font size=\"2\" color=\"grey\">")+tr("Version ") +
                   QCoreApplication::applicationVersion() +
                   ("</font>") +
                   ("<p align=\"left\">(C) 2011 <a href=\"")+QCoreApplication::organizationDomain()+("\">")+
                   QCoreApplication::organizationName()+("</a></p>") +
                   ("<p><a href=\"http://photosdd.sourceforge.net/\">") +
                   tr("Homepage")+("</a></p>") +
                   ("<br/><br/><p>")+
                   tr("English translation: FalseCAM")+
                   ("</p>"));
}

void MainWindow::aboutPhotosDD(){
    PhotosDD::about(0);
}

void MainWindow::aboutQT(){
    QMessageBox::aboutQt(0,tr("About QT"));
}

/*
 This function starts for all plugins the download function
 */

void MainWindow::download(){
    ui->overallProgressBar->setValue(1);
    QList<PhotosDDPlugin*> plugins = photosDD->getPlugins();
    for(int i = 0; i < plugins.count(); i++){
        connect(photosDD->getPlugins().at(i), SIGNAL(progress(int)), ui->pluginProgressBar, SLOT(setValue(int)));
        ui->pluginProgressBar->setValue(1);
        ui->overallProgressBar->setValue(100/plugins.count()*i/2);
        plugins.at(i)->download();
        disconnect(ui->pluginProgressBar, SLOT(setValue(int)));
        ui->pluginProgressBar->setValue(100);
        ui->overallProgressBar->setValue(100/plugins.count()*(i/2+1));
        copyImages(plugins.at(i)->getFiles());
    }
    ui->overallProgressBar->setValue(100);
    ui->pluginProgressBar->setValue(0);
}

/*
 Copies downloaded files to destination directory
 */
void MainWindow::copyImages(QStringList list){
    foreach(QString file, list){
        qDebug("[MainWindow] copies %s to %s", qPrintable(file), qPrintable(ui->SaveToDirLineEdit->text()));
        PhotosDD::download(file, ui->SaveToDirLineEdit->text());
    }
}

void MainWindow::on_DownloadButton_clicked()
{
    QDir dir(ui->SaveToDirLineEdit->text());
    if(dir.exists()){
        download();
    }else{
        QMessageBox::information(0, tr("Information"), tr("Direcory to save to does not exist"));
    }
}

/*
 Opens a dialog that shows tips for using this program.
 */
void MainWindow::showTips(){
    TipDialog(this).exec();
}

/*
 Loads config usings Qts settings class
 */
void MainWindow::loadConfig(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    ui->SaveToDirLineEdit->setText(settings.value("SaveToDir", QDir::homePath()).toString());
}

/*
 Saves config using Qts settings class.
 */
void MainWindow::saveConfig(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("SaveToDir", ui->SaveToDirLineEdit->text());
}

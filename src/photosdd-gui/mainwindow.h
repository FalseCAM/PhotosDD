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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "../photosdd/photosdd.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PhotosDD *photosDD;
    void loadPluginWidgets();
    // connects signals to slots
    void initSignalSlots();
    void loadConfig();
    void saveConfig();

private slots:
    void on_DownloadButton_clicked();
    void setSaveToDir();
    // configs Plugins
    void configPlugins();
    // About boxes.
    void aboutPhotosDD_GUI();
    void aboutPhotosDD();
    void aboutQT();
    // download routine
    void download();
    void copyImages(QStringList list);
    // Tips on startup
    void showTips();
};

#endif // MAINWINDOW_H

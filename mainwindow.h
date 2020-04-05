/**
    Copyright (C) 2012-2016  Matteo Pasotti <matteo.pasotti@gmail.com>

    This file is part of getaircrafts - Yet Another flightgear Aircraft Installer

    getaircrafts is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    getaircrafts is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QResource>
#include <QMessageBox>
#include <QFileDialog>

#include "webservice.h"
#include "xml2html.h"
#include "Installer.h"
#if defined Q_OS_MACX || defined Q_OS_WIN
#include "yalib.h"
#else
#include "yalib/yalib.h"
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void closeEvent(QCloseEvent *event);

    void on_pbtRetrieveAircraftList_clicked();

    void linkClickedSlot(QUrl url);

    void download_progress(int percent, double speed, QString unit);

    void model_downloaded(QString path);

    void populateWebView(QString xml);

    void about();

    void aboutQt();

    void on_pbtBrowse_clicked();

private:
    Ui::MainWindow *ui;
    QString output;
    QString getCSS();
    QString getJQuery();
    QString getJQueryUI();
    QString getJSFunctions();
    QStringList getAircraftList(QString path);
    webservice *ws;
    Yalib *ya;
    QString fg_detected_version;
};

#endif // MAINWINDOW_H

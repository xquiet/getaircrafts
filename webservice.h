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

#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
//#include <QXmlStreamReader>
#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QDir>

class webservice : public QObject
{
    Q_OBJECT

private:
    QString userAgent;
    QString hostRef;
    QTime downloadTime;

private slots:
    void retrieve_aircraftlist_xml(QNetworkReply *reply);
    void modelDownloaded(QNetworkReply *reply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
    void signal_download_progress(int percent, double speed, QString unit);
    void signal_model_downloaded(QString path);
    void signal_aircraftlist_retrieved(QString html);

public:
    webservice();
    void getaircrafts(QString version);
    void downloadModel(QUrl url);
    QNetworkReply *currentDownload;
};

#endif // WEBSERVICE_H

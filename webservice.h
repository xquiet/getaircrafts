#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
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
    void parse_xml_aircraftlist(QNetworkReply *reply);
    void modelDownloaded(QNetworkReply *reply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
    void signal_download_progress(int percent, double speed, QString unit);
    void signal_model_downloaded(QString path);
    void signal_aircraftlist_retrieved(QString html);

public:
    webservice();
    void getaircrafts();
    void downloadModel(QUrl url);
    QNetworkReply *currentDownload;
};

#endif // WEBSERVICE_H

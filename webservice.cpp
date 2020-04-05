#include "webservice.h"

webservice::webservice()
{
    userAgent = "Mozilla/5.0 (X11; Linux x86_64; rv:17.0) Gecko/20100101 Firefox/17.0";
    // new ref http://mirrors.ibiblio.org/flightgear/ftp/Aircraft-trunk/catalog.xml
    hostRef = "http://yaflight.grysol.com";
    // initializing
    currentDownload = NULL;
}

void webservice::getaircrafts(QString version)
{
    QNetworkAccessManager *mgr;
    QNetworkRequest req;
    mgr = new QNetworkAccessManager();
    req.setUrl(QUrl(hostRef+"/index.php/fgaircrafts/getaircrafts/?version="+version.replace(".","_")));
    req.setRawHeader("User-Agent",userAgent.toStdString().data());
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(retrieve_aircraftlist_xml(QNetworkReply*)));
    mgr->get(req);
}

void webservice::retrieve_aircraftlist_xml(QNetworkReply *reply)
{
    QString sDataReturned = QString(reply->readAll()).trimmed().replace("\n","");
    emit signal_aircraftlist_retrieved(sDataReturned);
}

void webservice::downloadModel(QUrl url)
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    QNetworkRequest req;
    req.setUrl(url);
    req.setRawHeader("User-Agent",userAgent.toStdString().data());
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(modelDownloaded(QNetworkReply*)));
    currentDownload = mgr->get(req);
    downloadTime.start();
    connect(currentDownload,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
}

void webservice::modelDownloaded(QNetworkReply *reply)
{
    if(!currentDownload->error())
    {
        qDebug("Download Succeded");
    }
    else
    {
        qFatal("Download Failed");
        return;
    }
    QByteArray ba = reply->readAll();
    QString tempFile = QDir::tempPath()+QDir::separator()+QFileInfo(reply->url().path()).fileName();
    QFile destFile(tempFile);
    if(!destFile.open(QIODevice::WriteOnly))
    {
        qWarning("%s",destFile.errorString().toStdString().data());
        return;
    }
    destFile.write(ba);
    destFile.close();
    currentDownload = NULL;
    emit signal_model_downloaded(tempFile);
}

void webservice::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    int percent = bytesReceived * 100 / bytesTotal;
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }

    emit signal_download_progress(percent, speed, unit + "\t" + currentDownload->url().toString());
}

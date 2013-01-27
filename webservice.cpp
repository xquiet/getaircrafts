#include "webservice.h"

webservice::webservice()
{
    userAgent = "Mozilla/5.0 (X11; Linux x86_64; rv:17.0) Gecko/20100101 Firefox/17.0";
    hostRef = "http://yaflight.altervista.org";
    // initializing
    currentDownload = NULL;
}

void webservice::getaircrafts()
{
    QNetworkAccessManager *mgr;
    QNetworkRequest req;
    mgr = new QNetworkAccessManager();
    req.setUrl(QUrl(hostRef+"/index.php/fgaircrafts/getaircrafts/"));
    req.setRawHeader("User-Agent",userAgent.toStdString().data());
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(parse_xml_aircraftlist(QNetworkReply*)));
    mgr->get(req);
}

void webservice::parse_xml_aircraftlist(QNetworkReply *reply)
{
    QString sDataReturned = QString(reply->readAll()).trimmed().replace("\n","");
    //qDebug("%s",sDataReturned.toStdString().data());
    QXmlStreamReader xml(sDataReturned);
    QString key = "";
    QString prevKey = "";
    QString output = "";
    QString authors = "";
    QString description = "";
    QString version = "";
    QString image = "";
    int idDiag = 0;
    //output += "<h2>Available Aircrafts</h2>";

    while(!xml.atEnd())
    {
        xml.readNext();
        //qDebug("%s",xml.name().toString().toStdString().data());
        //qDebug("%s",xml.text().toString().toStdString().data());
        QString xmlName = xml.name().toString();
        if((xmlName.compare("aircraft")==0)||
                xmlName.compare("aircraft")==0)
        {
            // do nothing
        }
        if(xmlName.compare("title")==0)
        {
            key = xml.readElementText();
            if(key.compare(prevKey)!=0)
            {
                if(description.trimmed().compare("")!=0)
                {
                    output += "</div> <!-- links -->";
                    output += "</div><!-- end details -->";
                    output += "<div style=\"clear: both;\"></div>";
                    output += "<input type=\"button\" onclick=\"showDialog("+QString::number(idDiag)+")\" value=\"Details\" />";
                    output += "<div id=\"detailsdiag"+QString::number(idDiag)+"\" title=\""+prevKey+"\" class=\"mydialog\">";
                    idDiag++;
                    output += "<div class=\"thumb\"><img src=\""+image+"\" alt=\""+key+"\" /></div>";
                    output += "<div class=\"details\">";
                    output += "<div><b>Authors:</b>&nbsp;&nbsp;"+authors+"</div>";
                    output += "<div><b>Version:</b>&nbsp;&nbsp;"+version+"</div>";
                    output += "<div><b>Description:</b><br />"+description+"</div>";
                    output += "<div style=\"clear: both;\"></div>";
                    output += "</div><!-- end details into dialog -->";
                    output += "</div><!-- end dialog -->";
                    output += "</div><!-- end aircraft -->";
                }
                description = "";
                authors = "";
                version = "";
                prevKey = key;
            }
            output += "<div class=\"aircraft\"><h3>" + key + "</h3>";
        }
        else if(xmlName.compare("image")==0)
        {
            image = xml.readElementText();
            output += "<div class=\"thumb\"><img src=\"" + image + "\" alt=\""+ key + "\" /></div>";
        }
        else if(xmlName.compare("model_name")==0)
        {
            description = xml.readElementText();
            QString brief_description = description;
            if(brief_description.length()>30)
            {
                brief_description = brief_description.mid(0,30) + "...";
            }
            output += "<div class=\"details\">";
            output += "<div class=\"modelname\"><b>Description:</b>&nbsp;"+brief_description+"</div>";
        }
        else if(xmlName.compare("author")==0)
        {
            authors = xml.readElementText();
            QString brief_authors = authors;
            if(brief_authors.length()>30)
            {
                brief_authors = brief_authors.mid(0,30) + "...";
            }
            output += "<div class=\"author\"><b>Authors:</b>&nbsp;&nbsp;<em>"+brief_authors+"</em></div>";
            output += "<div class=\"links\">";
        }
        else if(xmlName.compare("version")==0)
        {
            version = xml.readElementText();
            output += "<div class=\"version\"><b>Version:</b>&nbsp;&nbsp;"+version+"</div>";
        }
        else if(xmlName.contains("mirror"))
        {
            output += "<a href=\"" + xml.readElementText() + "\" title=\""+key+"\">" + xmlName.mid(0,1).toUpper()+xmlName.mid(1,xmlName.length()) + "</a>&nbsp;";
        }
    }
    output += "</div><!-- end aircraft last -->";
    if(xml.hasError())
    {
        qDebug("ERROR: %s",xml.errorString().toStdString().data());
    }
    emit signal_aircraftlist_retrieved(output);
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

    emit signal_download_progress(percent, speed, unit + currentDownload->url().toString());
}

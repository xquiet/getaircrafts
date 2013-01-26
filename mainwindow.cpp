#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initializing
    currentDownload = NULL;

    // hide progressbar
    ui->pbarDownload->hide();

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->webView->page(),SIGNAL(linkClicked(QUrl)),
                      this,SLOT(linkClickedSlot(QUrl)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbtRetrieveAircraftList_clicked()
{
    QNetworkAccessManager *mgr;
    QNetworkRequest req;
    mgr = new QNetworkAccessManager();
    //req.setUrl(QUrl("http://yaflight.altervista.org/index.php/fgaircrafts/getcompletelist/"));
    //req.setUrl(QUrl("http://yaflight.altervista.org/index.php/fgaircrafts/getaircraftlist/"));
    req.setUrl(QUrl("http://yaflight.altervista.org/index.php/fgaircrafts/getaircrafts/"));
    req.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64; rv:17.0) Gecko/20100101 Firefox/17.0");
    //connect(http,SIGNAL(requestFinished(int,bool)),this,SLOT(parse_xml_aircraftlist()));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(parse_xml_aircraftlist(QNetworkReply*)));
    ui->lblStatus->setText(tr("Loading aircraft list"));
    mgr->get(req);
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
}

void MainWindow::parse_xml_aircraftlist(QNetworkReply *reply)
{
    QString sDataReturned = QString(reply->readAll()).trimmed().replace("\n","");
    //qDebug("%s",sDataReturned.toStdString().data());
    QXmlStreamReader xml(sDataReturned);
    QString key = "";
    QString prevKey = "";
    output = "";
    output = getCSS();
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
                output += "</div> <!-- links -->";
                output += "</div>";
                prevKey = key;
            }
            output += "<div class=\"aircraft\"><h3>" + key + "</h3>";
        }
        else if(xmlName.compare("image")==0)
        {
            output += "<div class=\"thumb\"><img src=\"" + xml.readElementText() + "\" alt=\""+ key + "\" /></div>";
            output += "<div class=\"links\">";
        }
        else if(xmlName.contains("mirror"))
        {
            output += "<a href=\"" + xml.readElementText() + "\" title=\""+key+"\">" + xmlName + "</a>&nbsp;";
        }
    }
    output += "</div>";
    if(xml.hasError())
    {
        qDebug("ERROR: %s",xml.errorString().toStdString().data());
    }
    ui->webView->setHtml(output);
    ui->lblStatus->setText(tr("Done"));
}

QString MainWindow::getCSS()
{
    QResource r(":/html/css/style.css");
    QByteArray ba( reinterpret_cast< const char* >( r.data() ), r.size() );
    QString css = QString(ba);
    css = "<style type=\"text/css\">"+css+"</style>";
    return css;
}

void MainWindow::linkClickedSlot(QUrl url)
{
    qDebug("%s",url.toString().toStdString().data());
    if(url.toString().trimmed().compare("")==0)
        return;
    QNetworkAccessManager *mgr = new QNetworkAccessManager();
    QNetworkRequest req;
    req.setUrl(url);
    req.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64; rv:17.0) Gecko/20100101 Firefox/17.0");
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(modelDownloaded(QNetworkReply*)));
    ui->lblStatus->setText(tr("Downloading model..."));
    currentDownload = mgr->get(req);
    downloadTime.start();
    connect(currentDownload,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
}

void MainWindow::modelDownloaded(QNetworkReply *reply)
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
    ui->lblStatus->setText(tr("Model downloaded"));
    ui->pbarDownload->setValue(0);
    ui->pbarDownload->hide();
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(ui->pbarDownload->isHidden())
        ui->pbarDownload->setHidden(false);
    int percent = bytesReceived * 100 / bytesTotal;
    ui->pbarDownload->setValue(percent);

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
    ui->lblStatus->setText(QString::fromLatin1("%1 %2")
                        .arg(speed, 3, 'f', 1).arg(unit));

    ui->pbarDownload->update();
    ui->lblStatus->update();
}

/*void MainWindow::parse_xml_aircraftlist(QNetworkReply *reply)
{
    QString sDataReturned = QString(reply->readAll());
    //qDebug("%s",sDataReturned.toStdString().data());
    QStringList aircrafts = sDataReturned.split("\n",QString::SkipEmptyParts);
    foreach(QString aircraft, aircrafts)
    {
        qDebug("%s\n",aircraft.toStdString().data());
    }
}*/

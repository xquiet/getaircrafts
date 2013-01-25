#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mgr = NULL;
    // GET aircraft list
    // http://yaflight.altervista.org/index.php/fgaircrafts/getaircraftlist/?
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbtRetrieveAircraftList_clicked()
{
    mgr = new QNetworkAccessManager();
    //req.setUrl(QUrl("http://yaflight.altervista.org/index.php/fgaircrafts/getcompletelist/"));
    //req.setUrl(QUrl("http://yaflight.altervista.org/index.php/fgaircrafts/getaircraftlist/"));
    req.setUrl(QUrl("http://yaflight.altervista.org/index.php/fgaircrafts/getaircrafts/"));
    req.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64; rv:17.0) Gecko/20100101 Firefox/17.0");
    //connect(http,SIGNAL(requestFinished(int,bool)),this,SLOT(parse_xml_aircraftlist()));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(parse_xml_aircraftlist(QNetworkReply*)));
    mgr->get(req);
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
        qDebug("%s",xml.name().toString().toStdString().data());
        qDebug("%s",xml.text().toString().toStdString().data());
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
            output += "<a href=\"" + xml.readElementText() + "\" title=\"\">" + xmlName + "</a>&nbsp;";
        }
    }
    output += "</div>";
    if(!xml.hasError())
    {
        qDebug("ERROR: %s",xml.errorString().toStdString().data());
    }
    ui->webView->setHtml(output);
}

QString MainWindow::getCSS()
{
    QResource r(":/html/css/style.css");
    QByteArray ba( reinterpret_cast< const char* >( r.data() ), r.size() );
    QString css = QString(ba);
    css = "<style type=\"text/css\">"+css+"</style>";
    return css;
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

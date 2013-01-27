#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initialization
    ws = new webservice();

    ui->cbofgversion->addItems(QStringList()
                               << "v2.4"
                               << "v2.6"
                               << "v2.8"
                               );

    // hide progressbar
    ui->pbarDownload->hide();

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionAbout_Qt,SIGNAL(triggered()),this,SLOT(aboutQt()));
    connect(ui->webView->page(),SIGNAL(linkClicked(QUrl)),
                      this,SLOT(linkClickedSlot(QUrl)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(ws)
    {
        if((ws->currentDownload != NULL)&&(!ws->currentDownload->isFinished()))
        {
            QMessageBox msgBox(QMessageBox::Warning,tr("Warning"),tr("An ongoing download will be dropped\nAre you sure?"),QMessageBox::Ok|QMessageBox::Cancel);
            if(msgBox.exec() == QMessageBox::Cancel)
            {
                event->ignore();
            }
        }
    }
}

void MainWindow::on_pbtRetrieveAircraftList_clicked()
{
    ui->lblStatus->setText(tr("Loading aircraft list"));
    ws->getaircrafts();
    connect(ws,SIGNAL(signal_aircraftlist_retrieved(QString)),this,SLOT(populateWebView(QString)));
}

void MainWindow::populateWebView(QString html)
{
    html = "</head><body>" + html;
    html = getJSFunctions() + html; // last inserted, my own functions
    html = "<script src=\"http://code.jquery.com/ui/1.10.0/jquery-ui.js\"></script>" + html; // inserted AFTER jquery cause it requires jquery
    html = "<script src=\"http://code.jquery.com/jquery-1.9.0.js\"></script>" + html;
    html = "<link rel=\"stylesheet\" href=\"http://code.jquery.com/ui/1.10.0/themes/base/jquery-ui.css\" />" + html;
    html = getCSS() + html;
    html = "<html><head>" + html + "</body></html>";
    //qDebug("%s",html.toStdString().data());
    ui->webView->setHtml(html);
    ui->lblStatus->setText(tr("Done"));
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
}

QString MainWindow::getCSS()
{
    QResource r(":/html/css/style.css");
    QByteArray ba( reinterpret_cast< const char* >( r.data() ), r.size() );
    QString css = QString(ba);
    css = "<style type=\"text/css\">"+css+"</style>";
    return css;
}

QString MainWindow::getJQuery()
{
    QResource r(":/html/js/jquery-1.9.0-min.js");
    QByteArray ba( reinterpret_cast< const char* >( r.data() ), r.size() );
    QString jqcode = QString(ba);
    jqcode = "<script type=\"text/javascript\">"+jqcode+"</script>";
    qDebug("%s",jqcode.toStdString().data());
    return jqcode;
}

QString MainWindow::getJQueryUI()
{
    QResource r(":/html/js/jquery-ui-1.10.0.custom.min.js");
    QByteArray ba( reinterpret_cast< const char* >( r.data() ), r.size() );
    QString jquicode = QString(ba);
    jquicode = "<script type=\"text/javascript\">"+jquicode+"</script>";
    qDebug("%s",jquicode.toStdString().data());
    return jquicode;
}

QString MainWindow::getJSFunctions()
{
    QResource r(":/html/js/functions.js");
    QByteArray ba( reinterpret_cast< const char* >( r.data() ), r.size() );
    QString code = QString(ba);
    code = "<script type=\"text/javascript\">"+code+"</script>";
    return code;
}

void MainWindow::linkClickedSlot(QUrl url)
{
    qDebug("%s",url.toString().toStdString().data());
    if(url.toString().trimmed().compare("")==0)
        return;
    ui->lblStatus->setText(tr("Downloading model..."));
    connect(ws,SIGNAL(signal_model_downloaded(QString)),this,SLOT(model_downloaded(QString)));
    ws->downloadModel(url);
    connect(ws,SIGNAL(signal_download_progress(int, double, QString)),this,SLOT(download_progress(int, double, QString)));

}

void MainWindow::model_downloaded(QString path)
{
    QMessageBox msgBox(QMessageBox::Information,"Model stored",path,QMessageBox::Ok);
    msgBox.exec();
    ui->lblStatus->setText(tr("Model downloaded"));
    ui->pbarDownload->setValue(0);
    ui->pbarDownload->hide();
}

void MainWindow::download_progress(int percent, double speed, QString unit)
{
    if(ui->pbarDownload->isHidden())
        ui->pbarDownload->setHidden(false);

    ui->pbarDownload->setValue(percent);

    ui->lblStatus->setText(QString::fromLatin1("%1 %2")
                        .arg(speed, 3, 'f', 1).arg(unit));

    ui->pbarDownload->update();
    ui->lblStatus->update();
}

void MainWindow::about()
{
    QMessageBox msgBox(QMessageBox::Information,
                       tr("YaPManager"),
                       tr("Yet another Package Manager for FlightGear\n(C) 2013 by Matteo Pasotti <matteo@xquiet.eu>"),
                       QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::aboutQt()
{
    QMessageBox msgBox(this);
    msgBox.aboutQt(this, "About Qt");
}

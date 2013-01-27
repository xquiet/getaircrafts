#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // initialization
    ws = new webservice();

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
    html = getJQuery() + html;
    html = getCSS() + html;
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
    return jqcode;
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

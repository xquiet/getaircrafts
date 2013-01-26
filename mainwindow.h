#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QResource>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTime>

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
    void on_pbtRetrieveAircraftList_clicked();

    void parse_xml_aircraftlist(QNetworkReply *reply);

    void modelDownloaded(QNetworkReply *reply);

    void linkClickedSlot(QUrl url);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    Ui::MainWindow *ui;
    QString output;
    QString getCSS();
    QNetworkReply *currentDownload;
    QTime downloadTime;
};

#endif // MAINWINDOW_H

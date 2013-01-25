#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QResource>
#include <QFile>

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

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *mgr;
    QNetworkRequest req;
    QString output;
    QString getCSS();
};

#endif // MAINWINDOW_H

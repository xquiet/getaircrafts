#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QResource>
#include <QMessageBox>

#include "webservice.h"

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
    void closeEvent(QCloseEvent *event);

    void on_pbtRetrieveAircraftList_clicked();

    void linkClickedSlot(QUrl url);

    void download_progress(int percent, double speed, QString unit);

    void model_downloaded(QString path);

    void populateWebView(QString html);

private:
    Ui::MainWindow *ui;
    QString output;
    QString getCSS();
    QString getJQuery();
    webservice *ws;
};

#endif // MAINWINDOW_H

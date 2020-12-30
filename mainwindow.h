#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <unistd.h>
#include "math.h"
#include "threadTest.h"
#include "phidgetRead.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void init();


    threadTest* p_thread;
    QTimer*             p_Timer;
    phidgetRead* p_phidget;

    bool bRun;

private slots:

    void OnTimer();
    void rawValOutput();

    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();

private:
    Ui::MainWindow *ui;
};

extern MainWindow* pWnd;


#endif // MAINWINDOW_H

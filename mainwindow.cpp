#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow *pWnd;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pWnd = this;
    init();
}

MainWindow::~MainWindow()
{
    p_thread->stop();
    p_thread->wait();
    bRun = false;

    delete p_phidget;
    delete p_thread;
    delete p_Timer;
    delete ui;

}

void MainWindow::init() {

    // memory allocation
    p_Timer = new QTimer(this);
    p_phidget = new phidgetRead();
    p_thread = new threadTest();

    bRun = false;

    connect(p_Timer, SIGNAL(timeout()), this, SLOT(OnTimer()));

    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
    p_thread->on();
}

//void MainWindow::update()
//{
//    qDebug() << "update() called";
//}

void MainWindow::on_pushButton_start_clicked()
{
    p_thread->start();
    p_Timer->start(100);
    bRun = true;
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);
}

void MainWindow::on_pushButton_stop_clicked()
{
    bRun= false;
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
}

void MainWindow::OnTimer()
{
    rawValOutput();
}

void MainWindow::rawValOutput() {

    ui->lineEdit_0->setText(QString::number(p_phidget->encoderVal[0], 'f', 4)); // roll
    ui->lineEdit_1->setText(QString::number(p_phidget->encoderVal[1], 'f', 4)); // joint1
    ui->lineEdit_2->setText(QString::number(p_phidget->encoderVal[2], 'f', 4)); // joint2
    ui->lineEdit_3->setText(QString::number(p_phidget->encoderVal[3], 'f', 4)); // joint3
}

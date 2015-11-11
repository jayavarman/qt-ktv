#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMutex>
#include <QWaitCondition>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString command = "mplayer /home/sarith/Downloads/song1.mp4";
    //sleep(5);
    //system("mkfifo /tmp/mplayer-control");
    system("mplayer -slave -fs -input file=/tmp/mplayer-control /home/sarith/Videos/jerm_song.DAT");
    //msleep(100);
    system("echo \"af pan=2:1:1:0:0\" > /tmp/mplayer-control");
    //system(qPrintable(command));
    //ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::msleep(unsigned long msecs)
{
    QMutex mutex;
    mutex.lock();

    QWaitCondition waitCondition;
    waitCondition.wait(&mutex, msecs);

    mutex.unlock();
}

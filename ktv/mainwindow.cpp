#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QString command = "mplayer /home/sarith/Downloads/song1.mp4";
    //sleep(5);
    //system("mkfifo /tmp/mplayer-control");
    //system("mplayer -slave -fs -playlist /home/sarith/Videos/playlist.txt -input file=/tmp/mplayer-control -af pan=2:1:1:0:0");
    //system("mplayer -slave -input file=/tmp/mplayer-control /home/sarith/Videos/jerm_song.DAT -af pan=2:1:1:0:0");
    //msleep(100);
    //system("echo \"af pan=2:1:1:0:0\" > /tmp/mplayer-control");
    //system(qPrintable(command));
    //ui->setupUi(this);

    mplayer_proc = new QProcess( parent );
    connect( mplayer_proc, SIGNAL( finished(int) ), this, SLOT( exitedMPlayer(int) ) );
    //connect( mplayer_proc, SIGNAL( wroteToStdin () ), this, SLOT( wroteToStdinMPlayer() ) );
    //connect( mplayer_proc, SIGNAL( readyReadStderr () ), this, SLOT( readyReadStderrMPlayer() ) );
    //connect( mplayer_proc, SIGNAL( readyReadStdout() ), this, SLOT( readStdoutMPlayer() ) );
    mplayer_proc->start("mplayer -slave -fs -input file=/tmp/mplayer-control /home/sarith/Videos/song1.mp4");

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

void MainWindow::exitedMPlayer(int exitCode) {
    qDebug() << "MPlayer exited." << exitCode;
    mplayer_proc->start("mplayer -slave -fs -input file=/tmp/mplayer-control /home/sarith/Videos/jerm_song.DAT");
}

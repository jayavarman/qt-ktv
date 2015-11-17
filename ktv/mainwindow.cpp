#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <string>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QtSql/QtSql>

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
    //mplayer_proc->start("mplayer -slave -fs -input file=/tmp/mplayer-control /home/sarith/Videos/song1.mp4");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/sarith/ex1");
    bool ok = db.open();
    if(ok){
        QSqlQuery query;
        query.prepare("SELECT * FROM tbl1");
        if(!query.exec()){
            qDebug() << query.lastError();
        } else {
            QSqlRecord rec = query.record();
            int cols = rec.count();
            qDebug() << cols;

            for( int r=0; query.next(); r++ )
                  for( int c=0; c<cols; c++ )
                    qDebug() << QString( "Row %1, %2: %3" ).arg( r ).arg( rec.fieldName(c) ).arg( query.value(c).toString() );

        }
    }
    db.close();
    qDebug() << ok;
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
    //mplayer_proc->start("mplayer -slave -fs -input file=/tmp/mplayer-control /home/sarith/Videos/jerm_song.DAT");
}

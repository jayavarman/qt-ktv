#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <string>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QtSql/QtSql>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
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

    /*widget = new QWidget;
    setCentralWidget(widget);
    qDebug() << widget->winId();*/
    system("rm /tmp/mplayer-control");
    system("mkfifo /tmp/mplayer-control");
    mplayer_proc = new QProcess( parent );
    connect( mplayer_proc, SIGNAL( finished(int) ), this, SLOT( exitedMPlayer(int) ) );
    //mplayer_proc->setProcessChannelMode(QProcess::MergedChannels);
    //mplayer_proc->start("mplayer -slave -wid " + QString::number(widget->winId()) + "  -input file=/tmp/mplayer-control /home/sarith/Videos/jerm_song.DAT");
    mplayer_proc->start("mplayer -slave -input file=/tmp/mplayer-control /home/sarith/Music/16/464.DAT");
    /*QWindow::fromWinId(this->winId());*/
    /*QPushButton *btnPause = new QPushButton("Pause", this);
    QPushButton *btnNext = new QPushButton("Next", this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(btnPause);
    vLayout->addWidget(btnNext);
    widget->setLayout(vLayout);
    connect(btnPause, SIGNAL( clicked() ), this, SLOT( pauseSong() ) );
    connect(btnNext, SIGNAL( clicked() ), this, SLOT( nextSong() ) );*/

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/sarith/qt-ktv/ktv/ktv.sqlite");
    bool ok = db.open();
    if(ok){
        QSqlQuery query;
        query.prepare("SELECT code,title FROM songs WHERE code IN (16463, 16464, 16465)");
        if(!query.exec()){
            qDebug() << query.lastError();
        } else {
            QSqlRecord rec = query.record();
            int cols = rec.count();
            qDebug() << cols;
            SongListItem *item;
            for( int r=0; query.next(); r++ ){
                //for( int c=0; c<cols; c++ ){
                    //qDebug() << QString( "Row %1, %2: %3" ).arg( r ).arg( rec.fieldName(c) ).arg( query.value(c).toString() );
                    //ui->songList->addItem(query.value(c).toString());
                    item = new SongListItem();
                    item->setText(query.value(1).toString());
                    item->setSongCode(query.value(0).toString());
                    ui->songList->addItem(item);
                //}
                if(r >= 100){
                    break;
                }
            }

        }
    }
    db.close();
    qDebug() << ok;
}

MainWindow::~MainWindow(){
    delete mplayer_proc;
    delete ui;
}

void MainWindow::msleep(unsigned long msecs){
    QMutex mutex;
    mutex.lock();

    QWaitCondition waitCondition;
    waitCondition.wait(&mutex, msecs);

    mutex.unlock();
}

void MainWindow::exitedMPlayer(int exitCode) {
    qDebug() << "MPlayer exited." << exitCode;

    SongListItem *myitem = dynamic_cast<SongListItem*>(ui->songList->currentItem());
    qDebug() << "code: " << myitem->getSongCode();
    QString code = myitem->getSongCode();
    QString folderCode = code.left(2);
    QString fileCode = code.right(3);
    mplayer_proc->start("mplayer -slave  -input file=/tmp/mplayer-control  /home/sarith/Music/" + folderCode + "/" + fileCode + ".DAT");
}

void MainWindow::on_nextButton_clicked()
{
    qDebug() << "next song";
    system("echo quit > /tmp/mplayer-control");
}

void MainWindow::on_playButton_clicked()
{
    qDebug() << "pause song";
    system("echo pause > /tmp/mplayer-control");
}

void MainWindow::on_vocalButton_clicked()
{
    qDebug() << "karaoke mode";
    system("echo af pan=2:1:1:0:0 > /tmp/mplayer-control");
}

void MainWindow::on_stereoButton_clicked()
{
    qDebug() << "stereo mode";
    system("echo af pan=2:0:0:1:1 > /tmp/mplayer-control");
}

void MainWindow::on_songList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    SongListItem *myitem = dynamic_cast<SongListItem*>(current);
    qDebug() << "my item code: " << myitem->getSongCode();
    qDebug() << "item selected " << current->text();
}

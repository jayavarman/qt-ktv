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
    mplayer_proc->start("mplayer -slave -geometry 0:0 -input file=/tmp/mplayer-control /home/sarith/Music/16/464.DAT");
    /*QWindow::fromWinId(this->winId());*/
    /*QPushButton *btnPause = new QPushButton("Pause", this);
    QPushButton *btnNext = new QPushButton("Next", this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(btnPause);
    vLayout->addWidget(btnNext);
    widget->setLayout(vLayout);
    connect(btnPause, SIGNAL( clicked() ), this, SLOT( pauseSong() ) );
    connect(btnNext, SIGNAL( clicked() ), this, SLOT( nextSong() ) );*/

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/sarith/qt-ktv/ktv/ktv.sqlite");
    bool ok = db.open();
    if(ok){
        QSqlQuery query;
        query.prepare("SELECT singer FROM songs GROUP BY singer ORDER BY singer ASC");
        if(!query.exec()){
            qDebug() << query.lastError();
        } else {
            for( int r=0; query.next(); r++ ){
                ui->singerList->addItem(query.value(0).toString());
            }
            ui->singerList->setCurrentRow(0);
        }

        QSqlQuery query1;
        query1.prepare("SELECT code,title FROM songs ORDER BY title ASC");
        if(!query1.exec()){
            qDebug() << query1.lastError();
        } else {
            SongListItem *item;
            for( int r=0; query1.next(); r++ ){
                item = new SongListItem();
                item->setText(query1.value(1).toString() + " (" + query1.value(0).toString() + ")");
                item->setSongCode(query1.value(0).toString());
                ui->songList->addItem(item);
            }
            ui->songList->setCurrentRow(0);
        }
    }

    qDebug() << ok;
}

MainWindow::~MainWindow(){
    delete mplayer_proc;
    delete ui;
    db.close();
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
    QListWidgetItem *item = ui->listPlayList->takeItem(0);
    SongListItem *myitem = dynamic_cast<SongListItem*>(item);
    qDebug() << "code: " << myitem->getSongCode();
    QString code = myitem->getSongCode();
    qDebug() << "play song code " << code;
    QString folderCode = code.left(2);
    QString fileCode = code.right(3);
    //qDebug() << "mplayer -slave -geometry 0:0 -input file=/tmp/mplayer-control  /media/sarith/MyPassport/Karaoke/" + folderCode + "/" + fileCode + ".dat";
    QString filename = "/media/sarith/MyPassport/Karaoke/" + folderCode + "/" + fileCode + ".dat";
    QFile file(filename);
    if( !file.exists() ){
        filename = "/media/sarith/MyPassport/Karaoke/" + folderCode + "/" + fileCode + ".avi";
        QFile file(filename);
        if( !file.exists() ){
            filename = "/media/sarith/MyPassport/Karaoke/" + folderCode + "/" + fileCode + ".DAT";
        }
    }
    mplayer_proc->start("mplayer -slave -geometry 0:0 -input file=/tmp/mplayer-control  " + filename);
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

void MainWindow::on_songList_itemClicked(QListWidgetItem *item)
{
    SongListItem *myitem = dynamic_cast<SongListItem*>(item);
    qDebug() << "my item code: " << myitem->getSongCode();
    qDebug() << "item selected " << item->text();
}

void MainWindow::on_singerList_itemClicked(QListWidgetItem *item)
{
    QString singer = item->text();
    qDebug() << "singer " << singer;
    //return;
    if(db.open()){
        QSqlQuery query;
        qDebug() << "SELECT code,title FROM songs WHERE singer='" + singer + "' ORDER BY title ASC";
        query.prepare("SELECT code,title FROM songs WHERE singer='" + singer + "' ORDER BY title ASC");
        qDebug() << "111111111";
        if(!query.exec()){
            qDebug() << query.lastError();
        } else {
            ui->songList->clear();
            qDebug() << "3333333";
            SongListItem *item;
            qDebug() << "22222222";
            for( int r=0; query.next(); r++ ){
                item = new SongListItem();
                item->setText(query.value(1).toString() + " (" + query.value(0).toString() + ")");
                item->setSongCode(query.value(0).toString());
                ui->songList->addItem(item);
            }
            ui->songList->setCurrentRow(0);
        }
    }
}

void MainWindow::on_addToPlayListButton_clicked()
{
    SongListItem *item = dynamic_cast<SongListItem*>(ui->songList->currentItem());
            /*item = new SongListItem();
            item->setText(query1.value(1).toString());
            item->setSongCode(query1.value(0).toString());*/
    qDebug() << "before add " << item->text() << " code " << item->getSongCode();
    SongListItem *item2 = new SongListItem;
    item2->setText(item->text());
    item2->setSongCode(item->getSongCode());
    item2->setFlags(0);
    ui->listPlayList->addItem(item2);
    ui->listPlayList->setCurrentRow(0);
}

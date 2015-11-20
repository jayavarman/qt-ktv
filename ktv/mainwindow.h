#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QListWidgetItem>
#include "songlistitem.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    QProcess *mplayer_proc;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void msleep(unsigned long msecs);

private:
    Ui::MainWindow *ui;
    QWidget *widget;

private slots:
    void exitedMPlayer(int exitCode);
    void on_nextButton_clicked();
    void on_playButton_clicked();
    void on_vocalButton_clicked();
    void on_stereoButton_clicked();
    void on_songList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
};

#endif // MAINWINDOW_H

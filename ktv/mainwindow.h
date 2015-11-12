#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QProcess *mplayer_proc;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void msleep(unsigned long msecs);

private:
    Ui::MainWindow *ui;

private slots:
    void exitedMPlayer(int exitCode);
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include <QApplication>
#include "ktvtcpserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    KtvTcpServer server;
    server.startServer();

    MainWindow w;
    w.show();
    return a.exec();
}

#include "ktvtcpserver.h"
#include "ktvthread.h"

KtvTcpServer::KtvTcpServer(QObject *parent) : QTcpServer(parent){

}

void KtvTcpServer::startServer()
{
    int port = 1983;

    if(!this->listen(QHostAddress::Any, port)) {
        qDebug() << "Couldn't start server";
    } else {
        qDebug() << "Listening to port " << port << " ... ";
    }
}

void KtvTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "client ID: " << socketDescriptor << " connecting ...";

    KtvThread *thread = new KtvThread(socketDescriptor, this);

    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));

    thread->start();
}


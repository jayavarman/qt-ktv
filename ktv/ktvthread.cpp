#include "ktvthread.h"

KtvThread::KtvThread(qintptr ID, QObject *parent) : QThread(parent) {
    this->socketDescriptor = ID;
}

void KtvThread::run()
{
    // thread starts here
    qDebug() << "Thread started";

    socket = new QTcpSocket();

    if(!socket->setSocketDescriptor(this->socketDescriptor)){
        // something is wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    // This makes the slot to be invoked immediately, when the signal is emitted.

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // we'll have multiple clients, we want to know which is which
    qDebug() << "client ID: " << this->socketDescriptor << " connected";

    exec();
}

void KtvThread::readyRead()
{
    QByteArray data = socket->readAll();

    qDebug() << "client ID: " << this->socketDescriptor << " data: " << data;

    socket->write(data);
}

void KtvThread::disconnected()
{
    qDebug() << "client ID: " << this->socketDescriptor << " disconnected";

    socket->deleteLater();
}

#ifndef KTVTHREAD_H
#define KTVTHREAD_H

#include <QTcpSocket>
#include <QThread>
#include <QDebug>

class KtvThread : public QThread {
    Q_OBJECT
    public:
        explicit KtvThread(qintptr ID, QObject *parent = 0);
        void run();

    signals:
        void error(QTcpSocket::SocketError socketError);

    public slots:
        void readyRead();
        void disconnected();

    private:
        QTcpSocket *socket;
        qintptr socketDescriptor;
};

#endif // KTVTHREAD_H

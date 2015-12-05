#ifndef KTVTCPSERVER_H
#define KTVTCPSERVER_H

#include <QTcpServer>
#include <QDebug>

class KtvTcpServer: public QTcpServer {
    Q_OBJECT

    public:
        explicit KtvTcpServer(QObject *parent = 0);
        void startServer();

    signals:

    protected:
        void incomingConnection(qintptr socketDescriptor);
};

#endif // KTVTCPSERVER_H

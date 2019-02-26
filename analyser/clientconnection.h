#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

#include "tools/projectutilities.h"

class ClientConnection: public QThread
{
    Q_OBJECT
public:
    explicit ClientConnection(int iID, QObject *parent = nullptr);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

public slots:

private:
    QTcpSocket *socket;
    int socketDescriptor;

};

#endif // CLIENTCONNECTION_H

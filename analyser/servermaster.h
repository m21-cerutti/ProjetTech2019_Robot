#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QNetworkInterface>

#include "tools/projectutilities.h"
#include "analyser/clientconnection.h"

#define DEBUG_NETWORK
#define PORT 9999

class ServerMaster : public QTcpServer
{
    Q_OBJECT

public :

    ServerMaster(QObject *parent = nullptr);

    void StartServer();

private slots :
    void incomingConnection(qintptr socketDescriptor);

private :
    QTcpServer *server;
};

#endif // SERVER_H

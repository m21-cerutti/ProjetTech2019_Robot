#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QNetworkInterface>

#include "tools/projectutilities.h"
#include "analyser/iarobot.h"
#include "analyser/clientconnection.h"

#define PORT 5260

class ServerMaster : public QTcpServer
{
    Q_OBJECT

public :

    ServerMaster(IARobot& analyser, QObject *parent = nullptr);

    void StartServer();

private slots :
    void incomingConnection(int socketDescriptor);

private :
    IARobot& analyser;
    QTcpServer *server;
};

#endif // SERVER_H

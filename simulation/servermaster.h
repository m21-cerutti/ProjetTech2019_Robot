#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QNetworkInterface>

#include "simulation/clientconnection.h"

#define DEBUG_NETWORK
#define PORT 9999

class ServerMaster : public QTcpServer
{
    Q_OBJECT

public :

    ServerMaster(QObject *parent = nullptr);

    /**
     * @brief Start the server.
     */
    void StartServer();

private slots :

    /**
     * @brief handle new connection, only one client at most for debuging windows.
     */
    void newConnection();

private :
    QTcpServer *server;
};

#endif // SERVER_H

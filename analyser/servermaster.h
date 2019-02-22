#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QAbstractSocket>

#include "tools/projectutilities.h"
#include "analyser/ia.h"

#define PORT 5260




class ServerMaster : public QTcpServer
{
    Q_OBJECT

public :

    enum Cmd
    {
        WAIT,
        ACK,
        MOVE
    };

    ServerMaster(IA& analyser);
    ~ServerMaster();

    void sendInstructions(int i);

private slots :

    void first_connexion() ;
    void read();



private :

    QTcpSocket *clientConnection;
};

#endif // SERVER_H

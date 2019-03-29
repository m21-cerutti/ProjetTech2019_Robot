#include "analyser/servermaster.h"

ServerMaster::ServerMaster(QObject *parent):
    QTcpServer(parent)
{

}

void ServerMaster::StartServer()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);

#ifdef DEBUG_NETWORK
    int i = 0;
    qDebug() << "Choose adress.";
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if ((address.protocol() == QAbstractSocket::IPv4Protocol))
            qDebug() << i <<": "<< address.toString();
        i++;
    }
    int adresschoice;
    std::cin >> adresschoice;

    QHostAddress address = QNetworkInterface::allAddresses().at(adresschoice);
#else
    QHostAddress address = localhost;
#endif

    if(!this->listen(address, PORT))
    {
        qDebug() << "Could not start server with " <<address.toString();
    }
    else
    {
        qDebug() << "Listening on "<< this->serverAddress().toString() <<":"<<PORT<< "...";
        connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
    }
}

void ServerMaster::newConnection()
{
    qDebug() << " Connecting...";
    QTcpSocket* socket = nextPendingConnection();
    ClientConnection *client = new ClientConnection(socket, this);
    //connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    client->run();
}

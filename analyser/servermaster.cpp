#include "servermaster.h"

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
    }


}

void ServerMaster::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    ClientConnection *thread = new ClientConnection(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

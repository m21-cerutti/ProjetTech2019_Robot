#include "servermaster.h"

ServerMaster::ServerMaster(QObject *parent):
    QTcpServer(parent)
{

}

void ServerMaster::StartServer()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    /*
    int i = 0;
    qDebug() << "Choose adress.";
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if ((address.protocol() == QAbstractSocket::IPv4Protocol || address.protocol() == QAbstractSocket::IPv6Protocol) && address != localhost){
            qDebug() << i <<": "<< address.toString();
        }
        i++;
    }
    else
    {
        qDebug() << "Listening on "<< this->serverAddress().toString() <<":"<<PORT<< "...";
    }

    int adresschoice;
    std::cin >> adresschoice;

    QHostAddress address = QNetworkInterface::allAddresses().at(adresschoice);
    */

    if(!this->listen(localhost, PORT))
    {
        qDebug() << "Could not start server with " <<localhost.toString();
    }
    else
    {
        qDebug() << "Listening on "<< this->serverAddress().toString() <<":"<<PORT<< "...";
    }


}

void ServerMaster::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    ClientConnection *thread = new ClientConnection(socketDescriptor,this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

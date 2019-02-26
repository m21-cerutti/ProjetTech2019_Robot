#include "servermaster.h"

ServerMaster::ServerMaster(IARobot &_analyser, QObject *parent):
    QTcpServer(parent),
    analyser(_analyser)
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             qDebug() << address.toString();
    }
}

void ServerMaster::StartServer()
{
    if(!this->listen(QHostAddress::AnyIPv4, 5260))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening...";
    }
}

void ServerMaster::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    ClientConnection *thread = new ClientConnection(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

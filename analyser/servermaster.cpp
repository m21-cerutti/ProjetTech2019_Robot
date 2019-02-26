#include "servermaster.h"

ServerMaster::ServerMaster(IARobot &_analyser, QObject *parent):
    QTcpServer(parent),
    analyser(_analyser)
{

}

void ServerMaster::StartServer()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if ((address.protocol() == QAbstractSocket::IPv4Protocol)&& address != localhost){
            if(!this->listen(address, PORT))
            {
                qDebug() << "Could not start server with " <<address.toString();
            }
            else
            {
                qDebug() << "Listening on "<< this->serverAddress().toString() <<":"<<PORT<< "...";
                break;
            }
        }
    }


}

void ServerMaster::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    ClientConnection *thread = new ClientConnection(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

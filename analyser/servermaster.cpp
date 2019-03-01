#include "servermaster.h"

ServerMaster::ServerMaster(IARobot &_analyser, QObject *parent):
    QTcpServer(parent),
    analyser(_analyser)
{

}

void ServerMaster::StartServer()
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);

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
    ClientConnection *thread = new ClientConnection(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

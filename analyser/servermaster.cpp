#include "servermaster.h"

ServerMaster::ServerMaster(IARobot analyser, QObject *parent):
    QTcpServer(parent),
    analyser(analyser)
{

}

void ServerMaster::StartServer()
{
    if(!this->listen(QHostAddress::Any, 5260))
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

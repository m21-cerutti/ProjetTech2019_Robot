#include "clientconnection.h"

ClientConnection::ClientConnection(int ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void ClientConnection::run()
{
    // thread starts here
    qDebug() << socketDescriptor << " Starting thread";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()),Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << socketDescriptor << " Client connected";

    // make this thread a loop
    exec();
}

void ClientConnection::parseCommand(QByteArray data)
{
    if (data.isEmpty())
    {
        qWarning() << "No data.";
    }

    QList<QByteArray> cmd = data.split(';');

    if (cmd[0].toStdString() == "echo")
    {
        qDebug() << socketDescriptor << "command: " << cmd[0];

        //echo
        cmd.removeFirst();
        socket->write(cmd.join());

        qDebug() << socketDescriptor << "resend: " << cmd;

    }
    else if (cmd[0].toStdString() == "start")
    {
        qDebug() << socketDescriptor << "command: " << cmd[0];

        QImage returnImage;
        if(returnImage.loadFromData(cmd[1])) {
            qDebug() << socketDescriptor << " image 1 ok. ";
        }

        socket->write("ok");
    }
    else
    {
        qDebug() << socketDescriptor << " Incorrect command: " << cmd[0];
    }

}

void ClientConnection::readyRead()
{
    QByteArray data = socket->readAll();
    //qDebug() << socketDescriptor << " Data in: " << data;
    parseCommand(data);

    //echo primitive
    //socket->write(data);
}

void ClientConnection::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}

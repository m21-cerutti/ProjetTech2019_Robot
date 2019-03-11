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

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << socketDescriptor << " Client connected";

    // make this thread a loop
    exec();
}

void ClientConnection::analyse(char *cmd, int len)
{
    if (len == 0)
    {
        qWarning("Null command");
        return;
    }

    qDebug() <<"Command <"<< len <<">: " << cmd;
    qDebug() <<"Type: " << cmd[0];

    if (cmd[0] == 'e')
    {
        QByteArray new_cmd;
        new_cmd.append('r');
        new_cmd.append(cmd, len);
        //emit send(new_cmd);
        qDebug()<<"Recho send " << QString(cmd+1);
        socket->write(cmd);

    }
    else if (cmd[0] == 'r')
    {

    }
    else if (cmd[0] == 't')
    {

    }
    else if (cmd[0] == 's')
    {

    }
    else
    {
        qDebug() << socketDescriptor << " Incorrect command: " << cmd[0];
    }

    delete cmd;
}

void ClientConnection::send(QByteArray& package)
{
    QDataStream out(socket);
    out.setByteOrder(QDataStream::BigEndian);
    out <<(qint32) package.size();
    out << package;
}

void ClientConnection::readyRead()
{
    //https://openclassrooms.com/forum/sujet/qt-sockets-connexion-client-33733
    QDataStream in(socket);
    in.setByteOrder(QDataStream::BigEndian);

    if (size_package == 0)
    {
        if ((int)socket->bytesAvailable() < (int)sizeof(int32_t))
            return;

        in >> size_package;
        qDebug() << "New size package : " <<size_package;
    }

    if (socket->bytesAvailable() < size_package)
    {
        qDebug() << socket->bytesAvailable() <<"/" <<size_package<<" bytes.";
        qDebug() << socket->bytesAvailable()/(double)size_package *100<<"%.";
        return;
    }

    char* command= new char[size_package+1];
    if(in.readRawData(command, size_package) > 0)
    {
        emit analyse(command, size_package);
    }
    else
    {
        qWarning("Problem read data");
    }
    size_package = 0;
}

void ClientConnection::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    //socket->disconnectFromHost();
    socket->close();
    socket->deleteLater();
    quit();
}

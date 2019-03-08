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

void ClientConnection::analyse(char *s, int len)
{
    if (len == 0)
    {
        qWarning("Null command");
        return;
    }

    qDebug() <<"Command <"<< len <<">: " << s;

    /*
    if(cmd[0]=="")
    {
        qDebug() << socketDescriptor << "Empty";
        break;
    }
    else if (cmd[0].toStdString() == "echo")
    {
        if(cmd.length() == 2)
        {
            QString newcmd = "r_echo;" + QString(cmd[1]) +"\n";
            socket->writeData(newcmd.toStdString().c_str());
            is_complete = true;
        }
    }
    else if (cmd[0].toStdString() == "r_echo")
    {
        if(cmd.length() == 2)
        {
            qDebug() << socketDescriptor << "Echo complete: " << cmd[1];
            is_complete = true;
        }
    }
    else if (cmd[0].toStdString() == "refresh")
    {
        if(cmd.length() == 2)
        {
            if (REFRESH != cmd[1].toInt())
            {
                qDebug() << socketDescriptor << "refresh int size: " << cmd[1].length();
                QString newcmd = "refresh;" + QString::number(REFRESH) +"\n";
                socket->write(newcmd.toStdString().c_str());
            }
            is_complete = true;
        }
    }
    else if (cmd[0].toStdString() == "stereo")
    {
        qDebug() << socketDescriptor << " large command: " << cmd[0];
        if(cmd.length() == 7)
        {
            qDebug() << socketDescriptor << " large command complete: " << cmd[0];
            is_complete = true;
        }
    }
    else
    {
        qDebug() << socketDescriptor << " Incorrect command: " << cmd[0];
    }
    */


    delete s;
}

void ClientConnection::send(QString message)
{
    QByteArray package;
    QDataStream out(package);
    out << (quint32) 0;
    out << message;
    out.device()->seek(0);
    out << (quint32) (package.size() - sizeof(quint32));
    socket->write(package);
}

void ClientConnection::readyRead()
{
    //https://openclassrooms.com/forum/sujet/qt-sockets-connexion-client-33733
    QDataStream in(socket);
    in.setByteOrder(QDataStream::BigEndian);

    if (size_package == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint32))
            return;

        in >> size_package;
        qDebug() << "Need " <<size_package<<".";
    }

    if (socket->bytesAvailable() < size_package)
    {
        qDebug() << socket->bytesAvailable() <<"/" <<size_package<<".";
        qDebug() << socket->bytesAvailable()/(double)size_package<<"of data has come.";
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
    qDebug() << "read data";
    size_package = 0;
}

void ClientConnection::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}

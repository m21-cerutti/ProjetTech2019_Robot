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

void ClientConnection::parseCommand()
{
    if (buffer.isEmpty())
    {
        qWarning() << "No data.";
        return;
    }

    QDataStream buf(buffer);

    qDebug() << socketDescriptor << " Data in: " << buffer;

    QByteArray copy = buffer.mid(0);
    QList<QByteArray> list_commands = copy.split('\n');
    for(QByteArray command: list_commands)
    {
        bool is_complete = false;

        QList<QByteArray> cmd = command.split(';');

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
                socket->write(newcmd.toStdString().c_str());
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

        if(is_complete)
        {
            buffer.remove(0, command.length()+1);
        }

        /*Netcat*/
        /*
        else
        {
            buffer.chop(1);
        }
        */

    }
}

void ClientConnection::readyRead()
{
    //https://openclassrooms.com/forum/sujet/qt-sockets-connexion-client-33733
    QDataStream in(socket);

    if (size_package == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint32))
            return;

        in >> size_package;
    }

    if (socket->bytesAvailable() < size_package)
        return;

    // Si on arrive jusqu'à cette ligne, on peut récupérer le message entier
    QString messageRecu;
    in >> buffer;

    parseCommand();
}

void ClientConnection::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}

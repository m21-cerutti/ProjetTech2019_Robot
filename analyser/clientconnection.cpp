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
        QString format = "r"+QString(cmd+1);
        QByteArray new_cmd;
        new_cmd.append(format);
        send(new_cmd);
        qDebug()<<"Recho send " << format;

    }
    else if (cmd[0] == 'r')
    {
        qDebug()<<"Echo complete : " << QString(cmd+1);
    }
    else if (cmd[0] == 't')
    {
        qDebug() << "Size client is "<< QString(cmd+1).toInt();
        if(REFRESH != QString(cmd+1).toInt())
        {
            QString format = "t";
            QByteArray new_cmd;
            new_cmd.append(format);
            QByteArray integer;
            QDataStream stream(&integer, QIODevice::WriteOnly);
            stream << REFRESH;
            new_cmd.append(integer);
            send(new_cmd);
        }
    }
    else if (cmd[0] == 's')
    {/*
        std::vector<char> image_data(cmd,)
        QString(cmd+1).toInt()
                int size_left;
        cv::imdecode(cv::Mat(1, size_left, CV_8UC3, cmd+sizeof (int32_t)), CV_LOAD_IMAGE_UNCHANGED);
    */
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
    //Fait automatiquement
    //out <<(int32_t) package.size();
    out << package;
    /*
    for(auto byte : package)
    {
        qDebug() << "in: <" << byte;
    }
     qDebug() << "Send : prefix <" << sizeof(int32_t) << "> "<<package.size();
     */
}

void ClientConnection::readyRead()
{
    QDataStream in(socket);
    in.setByteOrder(QDataStream::BigEndian);

    while(socket->bytesAvailable() > 0)
    {
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
            command[size_package]='\0';
            emit analyse(command, size_package);
        }
        else
        {
            qWarning("Problem read data");
        }
        size_package = 0;
    }
}

void ClientConnection::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    //socket->disconnectFromHost();
    socket->close();
    socket->deleteLater();
    quit();
}

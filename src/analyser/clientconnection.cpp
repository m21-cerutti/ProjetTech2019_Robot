#include "analyser/clientconnection.h"

ClientConnection::ClientConnection(QTcpSocket* _socket, QObject *parent) :
    QObject(parent),
    socket(_socket),
    analyser(IARobot())
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ClientConnection::run()
{

    qDebug() << " Client connected";

    // make this thread a loop
    //exec();
    while(socket->state() == QTcpSocket::ConnectedState)
    {
        socket->waitForReadyRead(30);
    }
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
        int time_refresh = QString(cmd+1).toInt();
        qDebug() << "Time refresh client is "<< time_refresh;
        if(REFRESH != QString(cmd+1).toInt())
        {

            QByteArray integer;
            QDataStream stream(&integer, QIODevice::WriteOnly);
            stream << REFRESH;
            QString format = "t";
            QByteArray new_cmd;
            new_cmd.append(format);
            new_cmd.append(integer);
            send(new_cmd);

        }
    }
    else if (cmd[0] == 's')
    {
        QString num_left = QString(cmd+1);
        int size_left = num_left.toInt();
        QString num_right = QString(cmd+num_left.length()+size_left+2);
        int size_right = num_right.toInt();

        int offset_left = num_left.length()+2;
        int offset_right = num_left.length()+size_left+num_right.length()+3;

        //qDebug() << "Size left is "<< size_left;
        //qDebug() << "Size right is "<< size_right;
        //qDebug() << "Offset left "<< offset_left;
        //qDebug() << "Data :\n"<< cmd+offset_left;
        //qDebug() << "Offset right "<< offset_right;
        //qDebug() << "Data :\n"<<  cmd+offset_right;

        /*
        QImage img;
        if(img.loadFromData((uchar*)(cmd+offset_left),size_left))
        {
            cv::Mat img_left;
            CVQTInterface::toMatCV(img,img_left);
            ProjectUtilities::showMatrice("left", img_left);
        }
        else
        {
            qWarning("No image");
        }
        */


        cv::Mat tmp_l(1, size_left, CV_8UC3, cmd+offset_left);
        cv::Mat img_left = cv::imdecode(tmp_l, CV_LOAD_IMAGE_UNCHANGED);


        cv::Mat tmp_r(1, size_right, CV_8UC3, cmd+offset_right);
        cv::Mat img_right = cv::imdecode(tmp_r, CV_LOAD_IMAGE_UNCHANGED);

        //analyser.update(img_left, img_right);

        QString format = "i";
        QByteArray new_cmd;
        new_cmd.append(format);
        send(new_cmd);
    }
    else
    {
        qDebug() << " Incorrect command: " << cmd[0];
    }

    delete cmd;
}

void ClientConnection::send(QByteArray& package)
{
    QDataStream out(socket);
    out.setByteOrder(QDataStream::BigEndian);
    out << package;
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
            qDebug() << "Begin transfer...";
        }

        if (socket->bytesAvailable() < size_package)
        {
            //qDebug() << socket->bytesAvailable() <<"/" <<size_package<<" bytes.";
            //qDebug() << socket->bytesAvailable()/(double)size_package *100<<"%.";
            return;
        }
        qDebug() << "End transfer.";
        char* command= new char[size_package];
        if(in.readRawData(command, size_package) > 0)
        {
            command[size_package]='\0';
            analyse(command, size_package);
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
    qDebug() << " Disconnected";
    socket->disconnectFromHost();
    socket->close();
    socket->deleteLater();
}

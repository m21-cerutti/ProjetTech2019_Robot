#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QList>
#include <QStringList>
#include <QImage>

#include "tools/projectutilities.h"
#include "analyser/iarobot.h"

class ClientConnection: public QThread
{
    Q_OBJECT
public:
    explicit ClientConnection(int ID, QObject *parent = nullptr);
    void run();

private:
    void parseCommand();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:

    QTcpSocket *socket;
    int socketDescriptor;
    quint32 size_package;
    QByteArray buffer;
    IARobot analyser;

};

#endif // CLIENTCONNECTION_H

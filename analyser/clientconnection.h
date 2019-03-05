#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QList>
#include <QStringList>
#include <QImage>

#include "tools/projectutilities.h"

class ClientConnection: public QThread
{
    Q_OBJECT
public:
    explicit ClientConnection(int ID, QObject *parent = nullptr);
    void run();

private:
    void parseCommand(QByteArray data);

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    int socketDescriptor;

};

#endif // CLIENTCONNECTION_H

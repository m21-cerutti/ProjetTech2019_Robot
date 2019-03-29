#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QList>
#include <QStringList>
#include <QImage>

#include "analyser/iarobot.h"

class ClientConnection: public QObject
{
    Q_OBJECT
public:

    explicit ClientConnection(QTcpSocket *socket, QObject *parent = nullptr);
    void run();

signals:

    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private slots:
    //Delete s
    void analyse(char *cmd, int len);
    void send(QByteArray& package);

private:

    QTcpSocket *socket;
    int32_t size_package = 0;
    IARobot analyser;

};

#endif // CLIENTCONNECTION_H

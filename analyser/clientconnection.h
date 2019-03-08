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

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private slots:
    //Delete s
    void analyse(char *s, int len);
    void send(QString message);

private:

    QTcpSocket *socket;
    int socketDescriptor;

    quint64 size_package;

    IARobot analyser;

};

#endif // CLIENTCONNECTION_H

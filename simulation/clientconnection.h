#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QList>
#include <QStringList>
#include <QImage>

#include "simulation/iarobot.h"

class ClientConnection: public QObject
{
    Q_OBJECT
public:

    explicit ClientConnection(QTcpSocket *socket, QObject *parent = nullptr);

    /**
     * @brief Main loop for the client.
     */
    void run();

signals:

    /**
     * @brief error signal
     * @param socketerror
     */
    void error(QTcpSocket::SocketError socketerror);

public slots:
    /**
     * @brief call when data received.
     */
    void readyRead();

    /**
     * @brief call when data disconnected.
     */
    void disconnected();

private slots:

    /**
     * @brief analyse the command given.
     * @param cmd the command.
     * @param len the length.
     */
    void analyse(char *cmd, int len);

    /**
     * @brief send a package of bytes.
     * @param package datas to send.
     */
    void send(QByteArray& package);

private:


    /**
     * @brief Actual package size to receive in order to analyse.
     */
    int32_t size_package = 0;

    QTcpSocket *socket;

    IARobot analyser;

};

#endif // CLIENTCONNECTION_H

#include "servermaster.h"

ServerMaster::ServerMaster(IA& analyser):
QTcpServer(nullptr)
{

    listen(QHostAddress::Any,4000);
    QObject::connect(this, SIGNAL(newConnection()),
                      this, SLOT(first_connexion()));
}

ServerMaster::~ServerMaster()
{

}

void ServerMaster::sendInstructions(int i)
{

}

// si un client demande une connexion
void ServerMaster::first_connexion()
 {
    // on crée une nouvelle socket pour ce client
    clientConnection = nextPendingConnection();
    // si on reçoit des données, le slot lecture() est appelé
    QObject:: connect(clientConnection, SIGNAL(readyRead()),
    this, SLOT(read()));
}

void ServerMaster::read()
{
    QString line;
    while(clientConnection->canReadLine())    // tant qu'on peut lire sur la socket
    {
        line = clientConnection->readLine(); // on lit une ligne
    }
    QTextStream texte(clientConnection);      // création d'un flux pour écrire dans la socket
    texte << "message reçu" << endl;          // message à envoyer au client
}

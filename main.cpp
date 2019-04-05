#include "windows/mainwindow.h"
#include "simulation/servermaster.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

#ifdef SIMULATION

    ServerMaster server;
    server.StartServer();

#else

    MainWindow w;
    w.show();

#endif

    int r = a.exec();
    return r;
}

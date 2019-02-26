#include "mainwindow.h"
#include "analyser/servermaster.h"
#include "analyser/iarobot.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef SIMULATION

    IARobot analyser = IARobot();
    ServerMaster server(analyser);
    server.StartServer();

#else

    MainWindow w;
    w.show();

#endif

    int r = a.exec();
    return r;
}

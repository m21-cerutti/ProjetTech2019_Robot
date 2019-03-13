
#include "mainwindow.h"
#include "analyser/servermaster.h"

#include "custom_controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{


#ifdef IS_COMPUTER
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
#else
    cerutti::CustomController controller = cerutti::CustomController();

#endif
}

#include <QApplication>
#include "tools/imageanalysertests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ImageAnalyserTests test;
    int ret = test.startTests();

    return ret;
}

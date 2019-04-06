#include <QApplication>
#include "jaumoon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JauMoon w;
    w.show();
    
    return a.exec();
}

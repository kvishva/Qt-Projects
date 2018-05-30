#include "medicarewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //initate the Medicarewindow
    MediCareWindow w;
    w.show();

    return a.exec();
}

#include "mycalendarwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CalendarWidget w;
    w.show();

    return a.exec();
}

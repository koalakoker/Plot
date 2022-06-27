#include "mainwindow.h"
#include "mwplot.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    MWPlot plot("Title");

    for (int i = 0; i < 360; i++) {
        plot.addPoint(i, SData(sin(3 * i * M_PI / 180.0f)));
    }

    plot.show();

    return a.exec();
}

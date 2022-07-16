#include "mainwindow.h"
#include "wplot/sdata.h"
#include "wplot/wplot.h"

#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;

    WPlot plot(&w);
    float m = 1;
    for (int i = 0; i < (360*3); i++) {
        plot.addPoint(i, SData(i));
    }
    plot.createPlot();
    w.show();

    return a.exec();
}

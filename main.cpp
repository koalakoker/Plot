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
    int xmax = 360 * 3;
    for (int i = -xmax; i < xmax; i++) {
        double y;
        if (i != 0) {
            y = m*sin(i*M_PI/180)/i;
        } else {
            y = 0;
        }
        plot.addPoint(i, SData(y));
    }
    plot.createPlot();
    w.show();

    return a.exec();
}

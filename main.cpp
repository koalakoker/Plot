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
    double start = -20.0, end = 20.0;
    double x = start;
    double dx = 0.01;
    int n = (end- start) / dx;
    for (int i = 0; i < n; i++) {
        double y;
        if (x != 0.0) {
            y = m*sin(x)/x;
        } else {
            y = 1;
        }
        plot.addPoint(x, SData(y));
        x += dx;
    }
    plot.createPlot();
    w.show();

    return a.exec();
}

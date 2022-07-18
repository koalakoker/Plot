#include "mainwindow.h"
#include "wplot/sdata.h"
#include "wplot/wplot.h"

#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;

    // Example of plot in the main widget
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

    // Example of plot in stand alone widget
    WPlot plot2;
    for (int i = 0; i < 360; i++) {
        plot2.addPoint(i, SData(sin(i*M_PI/180)));
    }
    plot2.createPlot();
    plot2.show();
    plot2.setWindowTitle("Sin function");
    plot2.resize(QSize(800, 600));

    return a.exec();
}

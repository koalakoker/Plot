#include "mainwindow.h"
#include "wplot/sdata.h"
#include "wplot/wplot.h"

#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;

    WPlot plot(&w);
    float m = 10;
    for (int i = 0; i < (360*3); i++) {
        plot.addPoint(i, SData(m*sin(i * M_PI / 180.0f) + (m/4)*sin(3 * i * M_PI/180)));
    }
    plot.createPlot();
    w.show();

    return a.exec();
}

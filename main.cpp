#include "mainwindow.h"
#include "wplot/sdata.h"
#include "wplot/wplot.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    WPlot plot(&w);
    float m = 6000000;
    for (int i = 0; i < 3600; i++) {
        plot.addPoint(i, SData(m*sin(2 * i * M_PI / 180.0f),
                               m*sin((M_PI*2/6) + 2 * i * M_PI / 180.0f),
                               m*sin((2 * M_PI*2/6) + 2 * i * M_PI / 180.0f),
                               m*sin((3 * M_PI*2/6) + 2 * i * M_PI / 180.0f),
                               m*sin((4 * M_PI*2/6) + 2 * i * M_PI / 180.0f),
                               m*sin((5 * M_PI*2/6) + 2 * i * M_PI / 180.0f)));
    }
    plot.createPlot();
    w.show();

    return a.exec();
}

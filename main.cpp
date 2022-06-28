#include "mainwindow.h"
#include "wplot.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    WPlot plot(&w);

    for (int i = 0; i < 360; i++) {
        plot.addPoint(i, SData(sin(2 * i * M_PI / 180.0f),
                               sin((M_PI*2/6) + 2 * i * M_PI / 180.0f),
                               sin((2 * M_PI*2/6) + 2 * i * M_PI / 180.0f),
                               sin((3 * M_PI*2/6) + 2 * i * M_PI / 180.0f),
                               sin((4 * M_PI*2/6) + 2 * i * M_PI / 180.0f),
                               sin((5 * M_PI*2/6) + 2 * i * M_PI / 180.0f)));
    }
    plot.createPlot();
    w.show();

    return a.exec();
}

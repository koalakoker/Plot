#include "wplot/cursors.h"

#include <QDebug>

Cursors::Cursors()
{
}

QCursor Cursors::get(Cursor_t type) {
    QCursor cur;
    switch (type) {
    case ZoomIn:
    {
        QPixmap cursor_pixmap = QPixmap(":/png/icons8-zoom-in-24.png");
        cur = QCursor(cursor_pixmap, 0, 0);
    }
        break;
    case ZoomOut:
    {
        QPixmap cursor_pixmap = QPixmap(":/png/icons8-zoom-out-24.png");
        cur = QCursor(cursor_pixmap, 0, 0);
    }
        break;
    default:
        break;
    }
    return cur;
};

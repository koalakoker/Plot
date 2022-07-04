#ifndef CURSORS_H
#define CURSORS_H

#include "QtCore/qnamespace.h"
#include "QtGui/qcursor.h"

class Cursors
{
private:
    Cursors();
public:

    typedef enum {
        ZoomIn,
        ZoomOut
    } Cursor_t;

    static QCursor get(Cursor_t type);
};

#endif // CURSORS_H

#ifndef JSONDATA_H
#define JSONDATA_H

#include <QString>

class JSONData
{
public:
    typedef enum {
        INT,
        NUMBER,
        BOOL,
        STRING
    } Data_t;

    JSONData(QString name, Data_t type, void* value);

    QString m_name;
    Data_t m_type;
    void* m_value;
};

#endif // JSONDATA_H

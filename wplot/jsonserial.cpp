#include "jsonserial.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>

JSONSerial::JSONSerial()
{

}

void JSONSerial::add(QString name, int value) {
    values.append(JSONData(name, JSONData::INT, (void*)(new int(value))));
}
void JSONSerial::add(QString name, double value){
    values.append(JSONData(name, JSONData::NUMBER, (void*)(new double(value))));
}
void JSONSerial::add(QString name, bool value){
    values.append(JSONData(name, JSONData::BOOL, (void*)(new bool(value))));
}
void JSONSerial::add(QString name, QString value) {
    values.append(JSONData(name, JSONData::STRING, (void*)(new QString(value))));
}

void JSONSerial::read(QString name, int& value) {
    value = m_object.value(name).toInt();
}
void JSONSerial::read(QString name, double& value) {
    value = m_object.value(name).toDouble();
}
void JSONSerial::read(QString name, bool& value) {
    value = m_object.value(name).toBool();
}
void JSONSerial::read(QString name, QString& value) {
    value = m_object.value(name).toString();
}

void JSONSerial::save(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << toJSON();
}
void JSONSerial::load(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);

    QString line;
    while (!in.atEnd()) {
        line += in.readLine();
    }
    values.clear();
    fromJSON(line);
}
QString JSONSerial::toJSON() {
    QJsonObject mainObject;

    foreach (JSONData element, this->values) {
        switch (element.m_type) {
        case JSONData::INT:
        {
            mainObject.insert(element.m_name, *(int*)(element.m_value));
            break;
        }
        case JSONData::NUMBER:
        {
            mainObject.insert(element.m_name, *(double*)(element.m_value));
            break;
        }
        case JSONData::BOOL:
        {
            mainObject.insert(element.m_name, *(bool*)(element.m_value));
            break;
        }
        case JSONData::STRING:
        {
            mainObject.insert(element.m_name, *(QString*)(element.m_value));
        }
            break;

        default:
            break;
        }

    }

    QJsonDocument jsonDoc;
    jsonDoc.setObject(mainObject);

    return jsonDoc.toJson();
}
void JSONSerial::fromJSON(QString jsonString) {
    QByteArray qbJSON = jsonString.toUtf8();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(qbJSON);
    if(jsonDocument.isObject() == false) qDebug() << "It is not a JSON object";
    m_object = jsonDocument.object();
}

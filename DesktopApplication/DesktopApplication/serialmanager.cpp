#include "serialmanager.h"
#include <QList>
#include <QString>
#include <QVariant>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

SerialManager::SerialManager(QObject *parent)
    : QObject{parent}
{

}

QVariantList SerialManager::getSerials()
{
    const auto ports {QSerialPortInfo::availablePorts()};
    QVector<QVariant> list;
    for (auto &port : ports) {
        list.push_back(QVariant::fromValue(new SerialInfo(
            port.portName(),
            QString::number(port.vendorIdentifier()),
            QString::number(port.productIdentifier()),
            port.manufacturer(),
            this
        )));
    }

    return QVariantList::fromVector(list);
}

void SerialManager::onSerialSelected(const QString &serialName)
{
    qDebug() << "Port " << serialName;
    emit serialConnectionSelected(1, serialName);
}

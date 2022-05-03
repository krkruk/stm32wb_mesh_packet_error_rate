#include "serialmanager.h"
#include <QList>
#include <QString>
#include <QVariant>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

#include <algorithm>

SerialManager::SerialManager(QObject *parent)
    : QObject{parent} {

}

QVariantList SerialManager::getSerials() {
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

    std::sort(std::begin(list), std::end(list), [](const QVariant &lhs, const QVariant &rhs) {
        return lhs.value<SerialInfo*>()->name()
               .compare(rhs.value<SerialInfo*>()->name()) < 0;
    });

    return QVariantList::fromVector(list);
}

QVariantMap SerialManager::getConnectionSettings() {
    QSettings settings;
    QVariantMap values;
    QStringList keys = settings.allKeys();
    for (const QString &key : qAsConst(keys)) {
        QStringList tokenizedKey = key.split('.');
        QString millis = tokenizedKey.last();
        int ticks = settings.value(key, 0).toInt();
        values.insert(millis, ticks);
    }

    return values;
}

void SerialManager::onSerialSelected(const QString &serialName) {
    auto index = serialInstances.contains(serialName)
                 ? serialInstances.value(serialName)
                 : serialInstances.count();

    serialInstances[serialName] = index;
    qDebug() << "Port " << serialName << " at index: " << index;
    emit serialConnectionSelected(index, serialName);
}

#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QVariantList>
#include "serialinfo.h"

class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList getSerials();

signals:
    void serialConnectionSelected(int index, const QString &serialName);

public slots:
    void onSerialSelected(const QString &serialName);
};

#endif // SERIALMANAGER_H

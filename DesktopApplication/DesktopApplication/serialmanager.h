#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QVariantMap>
#include <QVariantList>
#include <QHash>
#include <QSettings>
#include "serialinfo.h"

class SerialManager : public QObject {
    Q_OBJECT

    QHash<QString, int> serialInstances;
  public:
    explicit SerialManager(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList getSerials();

    Q_INVOKABLE QVariantMap getConnectionSettings();
  signals:
    void serialConnectionSelected(int index, const QString &serialName);

  public slots:
    void onSerialSelected(const QString &serialName);
};

#endif // SERIALMANAGER_H

#ifndef SERIALINFO_H
#define SERIALINFO_H

#include <QObject>

class SerialInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString vendorIdentifier READ vendorIdentifier WRITE setVendorIdentifier NOTIFY vendorIdentifierChanged)
    Q_PROPERTY(QString productIdentifier READ productIdentifier WRITE setProductIdentifier NOTIFY productIdentifierChanged)
    Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY manufacturerChanged)

public:
    explicit SerialInfo(QObject *parent = nullptr);
    SerialInfo(const QString &name, const QString &vendorIdentifier, const QString &productIdentifier, const QString &manufacturer, QObject *parent = nullptr);


    const QString &name() const;
    void setName(const QString &newName);

    const QString &vendorIdentifier() const;
    void setVendorIdentifier(const QString &newVendorIdentifier);

    const QString &productIdentifier() const;
    void setProductIdentifier(const QString &newProductIdentifier);

    const QString &manufacturer() const;
    void setManufacturer(const QString &newManufacturer);

signals:

    void nameChanged();
    void vendorIdentifierChanged();
    void productIdentifierChanged();
    void manufacturerChanged();

private:
    QString m_name;
    QString m_vendorIdentifier;
    QString m_productIdentifier;
    QString m_manufacturer;
};

Q_DECLARE_METATYPE(SerialInfo*)
#endif // SERIALINFO_H

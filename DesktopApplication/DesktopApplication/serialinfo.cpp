#include "serialinfo.h"

SerialInfo::SerialInfo(QObject *parent) :
    SerialInfo("", "", "", "", parent)
{

}

SerialInfo::SerialInfo(const QString &name, const QString &vendorIdentifier, const QString &productIdentifier, const QString &manufacturer, QObject *parent) : QObject(parent),
    m_name(name),
    m_vendorIdentifier(vendorIdentifier),
    m_productIdentifier(productIdentifier),
    m_manufacturer(manufacturer)
{}

const QString &SerialInfo::name() const
{
    return m_name;
}

void SerialInfo::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

const QString &SerialInfo::vendorIdentifier() const
{
    return m_vendorIdentifier;
}

void SerialInfo::setVendorIdentifier(const QString &newVendorIdentifier)
{
    if (m_vendorIdentifier == newVendorIdentifier)
        return;
    m_vendorIdentifier = newVendorIdentifier;
    emit vendorIdentifierChanged();
}

const QString &SerialInfo::productIdentifier() const
{
    return m_productIdentifier;
}

void SerialInfo::setProductIdentifier(const QString &newProductIdentifier)
{
    if (m_productIdentifier == newProductIdentifier)
        return;
    m_productIdentifier = newProductIdentifier;
    emit productIdentifierChanged();
}

const QString &SerialInfo::manufacturer() const
{
    return m_manufacturer;
}

void SerialInfo::setManufacturer(const QString &newManufacturer)
{
    if (m_manufacturer == newManufacturer)
        return;
    m_manufacturer = newManufacturer;
    emit manufacturerChanged();
}

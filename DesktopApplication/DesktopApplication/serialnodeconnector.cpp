#include "serialnodeconnector.h"
#include <QDebug>
#include <QDateTime>

#include <functional>

#include "getaddresscommand.h"

SerialNodeConnector::SerialNodeConnector(QObject *parent) : QObject{parent}, lineCounter{0}
{
    qDebug() << "Created connector";
}

SerialNodeConnector::SerialNodeConnector(const QString &portName, QObject *parent)
    : QObject{parent}, port{new QSerialPort(portName)}, lineCounter{0}
{

}

SerialNodeConnector::~SerialNodeConnector()
{
    QString portName = port ? port->portName() : "null";
    qDebug() << "Closing " << portName;
}

void SerialNodeConnector::write(const QByteArray &data)
{
    qDebug() << "CMD =" << qPrintable(data);

    if (port) {
        qint64 result = port->write(data);
        qDebug() << "Wrote... result=" << result;
        port->flush();
    }
}

void SerialNodeConnector::processLine(const QDateTime &timestamp, const QString &line)
{

}

void SerialNodeConnector::open(const QString &portName)
{
    qDebug() << "Opening the port " << portName;
    if (!port) {
        port.reset(new QSerialPort);
    }
    port->setPortName(portName);
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);

    connect(port.get(), &QSerialPort::readyRead, this, &SerialNodeConnector::onReadyReadTriggered);
    connect(port.get(), &QSerialPort::bytesWritten, this, &SerialNodeConnector::handleBytesWritten);

    connect(port.get(), &QSerialPort::errorOccurred, this, [](QSerialPort::SerialPortError error) {
        if (error != QSerialPort::NoError) {
            qDebug() << "Got an error" << error;
        }
    });

    if (!port->open(QIODevice::ReadWrite)) {
        qDebug() << "Cannot connect..." << port->errorString();
    }
}

void SerialNodeConnector::runCommand(const int &cmd)
{
    switch (cmd) {
    case Stm32SupportedOperations::GET_ADDRESS: {
        qDebug() << "Getting node address";
        command.reset(new GetAddressCommand(std::bind(&SerialNodeConnector::write, this, std::placeholders::_1)));
        command->initialize(0, 0xc000, 0, 0);
        break;
    }
    case Stm32SupportedOperations::UNKNOWN:
    default:
        qDebug() << "Unrecognized command";
    }
}

void SerialNodeConnector::onReadyReadTriggered()
{
    if (!port && !port->canReadLine()) {
        return;
    }

    QByteArray rawLine = port->readLine();
    QString line = QString(rawLine).trimmed();
    ++lineCounter;
    QDateTime now {QDateTime::currentDateTimeUtc()};
    QString processedline = QString("[%1,%2]: %3")
            .arg(lineCounter, 12, 10, QChar('0'))
            .arg(now.toString(Qt::ISODateWithMs), line);
    emit logLineReceived(processedline);

    processLine(now, line);
}

void SerialNodeConnector::handleBytesWritten(qint64 bytes)
{
    qDebug() << "Bytes written" <<bytes;
}

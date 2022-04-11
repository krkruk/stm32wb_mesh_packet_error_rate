#include "serialnodeconnector.h"
#include <QDebug>
#include <QDateTime>
#include <QTimer>

#include <functional>

#include "getaddresscommand.h"
#include "calibratecommand.h"
#include "linemessagedispatcher.h"

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
    if (port) {
        QString portName = port->portName();
        port->flush();
        qDebug() << "Closing " << portName;
    }
}

void SerialNodeConnector::scheduleWrite(const QByteArray &data)
{
    // Simulate typing in a standard serial terminal so the STM32WB can keep up...
    if (port) {
        port->clear();
    }
    LineMessageDispatcher *dispatcher = new LineMessageDispatcher(data);
    connect(dispatcher, &LineMessageDispatcher::writeCharacter, this, &SerialNodeConnector::write, Qt::QueuedConnection);
    QThreadPool::globalInstance()->start(dispatcher);
}

void SerialNodeConnector::write(const QChar &data)
{
    if (port) {
        port->write(QString(data).toLocal8Bit());
        port->flush();
    }
}

void SerialNodeConnector::processLine(const QDateTime &timestamp, const QString &line)
{
    if (command) {
        command->iterate(timestamp, line);
    }
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

    connect(port.get(), &QSerialPort::errorOccurred, this, [](QSerialPort::SerialPortError error) {
        if (error != QSerialPort::NoError) {
            qDebug() << "Got an error" << error;
        }
    });

    if (port->open(QIODevice::ReadWrite)) {
        port->sendBreak();      // restart the device once connected
    }
}

void SerialNodeConnector::runCommand(const int &cmd, const QVariant &parameters)
{
    switch (cmd) {
    case Stm32SupportedOperations::GET_ADDRESS: {
        qDebug() << "Getting node address";
        command = GetAddressCommand::create(
                    std::bind(&SerialNodeConnector::scheduleWrite, this, std::placeholders::_1),
                    0, 0xc000, 0, 0);
        break;
    }
    case Stm32SupportedOperations::CALIBRATE: {
        qDebug() << "Getting node address";
        command = CalibrateCommand::create(
                    std::bind(&SerialNodeConnector::scheduleWrite, this, std::placeholders::_1),
                    0, 0, 0, 0);
        break;
    }
    case Stm32SupportedOperations::UNKNOWN:
    default:
        qDebug() << "Unrecognized command";
    }

    if (command) {
        qDebug() << "Connecting command slots/signals";
        connect(command.get(), &SerialCommand::timeout, this, &SerialNodeConnector::onQueryTimeout);
        connect(command.get(), &SerialCommand::resultReceived, this, &SerialNodeConnector::onQueryResultReceived);
        connect(command.get(), &SerialCommand::error, this, &SerialNodeConnector::onQueryError);
        emit runningQuery();
    }
}

void SerialNodeConnector::onQueryTimeout()
{
    qDebug() << "Query timeout";
    command.reset();
    emit timeout();
}

void SerialNodeConnector::onQueryResultReceived(const QVariant &result)
{
    emit resultReceived(command->identifier(), result);
    command.reset();
}

void SerialNodeConnector::onQueryError()
{
    command.reset();
    emit error();
}

void SerialNodeConnector::onReadyReadTriggered()
{
    if (!port || !port->canReadLine()) {
        return;
    }

    QByteArray rawLine = port->readLine(1024);
    QString line = QString(rawLine).trimmed();
    ++lineCounter;
    QDateTime now {QDateTime::currentDateTimeUtc()};
    QString processedline = QString("[%1,%2]: %3")
            .arg(lineCounter, 12, 10, QChar('0'))
            .arg(now.toString(Qt::ISODateWithMs), line);
    emit logLineReceived(processedline);

    processLine(now, line);
}

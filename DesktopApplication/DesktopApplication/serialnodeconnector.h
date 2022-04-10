#ifndef SERIALNODECONNECTOR_H
#define SERIALNODECONNECTOR_H

#include <QObject>
#include <QScopedPointer>
#include <QtSerialPort>

#include <memory>

#include "SerialCommand.h"
#include "stm32supportedoperations.h"


class SerialNodeConnector : public QObject
{
    Q_OBJECT

    QScopedPointer<QSerialPort> port;
    long lineCounter;

    std::unique_ptr<SerialCommand> command;

public:
    explicit SerialNodeConnector(QObject *parent = nullptr);
    explicit SerialNodeConnector(const QString &portName, QObject *parent = nullptr);
    ~SerialNodeConnector();


private:
    void write(const QByteArray &data);
    void processLine(const QDateTime &timestamp, const QString &line);


signals:
    void logLineReceived(const QString &line);


public slots:
    void open(const QString &portName);
    void runCommand(const int &cmd);


private slots:
    void onReadyReadTriggered();
    void handleBytesWritten(qint64 bytes);
};

#endif // SERIALNODECONNECTOR_H

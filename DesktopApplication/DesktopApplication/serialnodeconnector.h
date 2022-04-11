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
    static constexpr int MOCK_TYPING_SPEED_MS = 100;

    QScopedPointer<QSerialPort> port;
    long lineCounter;

    std::unique_ptr<SerialCommand> command;

public:
    explicit SerialNodeConnector(QObject *parent = nullptr);
    explicit SerialNodeConnector(const QString &portName, QObject *parent = nullptr);
    ~SerialNodeConnector();


private:
    void scheduleWrite(const QByteArray &data);
    void write(const QChar &data);
    void processLine(const QDateTime &timestamp, const QString &line);


signals:
    void logLineReceived(const QString &line);
    void characterToWriteReceived(const QChar &character);

    void runningQuery();
    void resultReceived(int id, const QVariant &result);
    void timeout();


public slots:
    void open(const QString &portName);
    void runCommand(const int &cmd);


private slots:
    void onQueryTimeout();
    void onQueryResultReceived(const QVariant &result);
    void onReadyReadTriggered();
};

#endif // SERIALNODECONNECTOR_H

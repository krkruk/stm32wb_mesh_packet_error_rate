#ifndef SERIALNODECONNECTOR_H
#define SERIALNODECONNECTOR_H

#include <QObject>
#include <QScopedPointer>
#include <QtSerialPort>
#include <QThreadPool>

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
    void processLine(const QDateTime &timestamp, const QString &line);


signals:
    void logLineReceived(const QString &line);
    void characterToWriteReceived(const QChar &character);

    void runningQuery();
    void resultReceived(int id, const QVariant &result);
    void timeout();
    void error();


public slots:
    void open(const QString &portName);
    void runCommand(const int &cmd, const QVariant &parameters);
    void write(const QChar &data);


private slots:
    void onQueryTimeout();
    void onQueryResultReceived(const QVariant &result);
    void onQueryError();
    void onReadyReadTriggered();
};

#endif // SERIALNODECONNECTOR_H

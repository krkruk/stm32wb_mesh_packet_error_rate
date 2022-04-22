#ifndef SERIALNODECONNECTOR_H
#define SERIALNODECONNECTOR_H

#include <QObject>
#include <QScopedPointer>
#include <QtSerialPort>
#include <QThreadPool>

#include <memory>

#include "SerialCommand.h"
#include "stm32supportedoperations.h"


class SerialNodeConnector : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enableLogArea READ enableLogArea WRITE setEnableLogArea NOTIFY enableLogAreaChanged)
    QScopedPointer<QSerialPort> port;
    long lineCounter;
    std::unique_ptr<SerialCommand> command;

    bool m_enableLogArea {false};

  public:
    explicit SerialNodeConnector(QObject *parent = nullptr);
    explicit SerialNodeConnector(const QString &portName, QObject *parent = nullptr);
    ~SerialNodeConnector();


    bool enableLogArea() const;
    void setEnableLogArea(bool newEnableLogArea);

  private:
    void processLine(const QDateTime &timestamp, const QString &line);



  signals:
    void logLineReceived(const QString &line);
    void characterToWriteReceived(const QChar &character);

    void runningQuery();
    void resultReceived(int id, const QVariant &result);
    void timeout();
    void error();

    void enableLogAreaChanged();

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

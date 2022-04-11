#ifndef SERIALCOMMAND_H
#define SERIALCOMMAND_H
#include <QObject>
#include <QScopedPointer>
#include <QTimer>
#include "stm32supportedoperations.h"

class QDateTime;
class QString;

class SerialCommand : public QObject {
    Q_OBJECT

    std::function<void(QByteArray)> writeCallback;
    QScopedPointer<QTimer> timer;
public:
    virtual ~SerialCommand() {};

    virtual Stm32SupportedOperations::Stm32SupportedOperationsEnums identifier() const {
        return Stm32SupportedOperations::UNKNOWN;
    }

    virtual void initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
        Q_UNUSED(srcAddr)
        Q_UNUSED(dstAddr)
        Q_UNUSED(intervalMs)
        Q_UNUSED(timeout)

//        timer->start(300000);   // 5minutes timeout
        timer->start(10000);   // 5minutes timeout
    }
    virtual void iterate(const QDateTime &timestamp, const QString &data) = 0;


protected:
    explicit SerialCommand(std::function<void(QByteArray)> write, QObject *parent = nullptr)
        : QObject{parent}, writeCallback{write}, timer{new QTimer} {
        timer->setSingleShot(true);
        timer->setTimerType(Qt::CoarseTimer);
        connect(timer.data(), &QTimer::timeout, this, &SerialCommand::timeout);
    }

    void write(const QByteArray &data) {
        writeCallback(data);
    }

    void cancelTimeout() {
        if (timer) {
            timer->stop();
        }
    }


signals:
    void resultReceived(const QVariant &result);
    void timeout();
    void error();
};

#endif // SERIALCOMMAND_H

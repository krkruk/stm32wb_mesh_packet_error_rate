#ifndef SERIALCOMMAND_H
#define SERIALCOMMAND_H
#include <QObject>
#include <QScopedPointer>
#include <QTimer>
#include <QThreadPool>
#include "linemessagedispatcher.h"
#include "stm32supportedoperations.h"

class QDateTime;
class QString;

class SerialCommand : public QObject {
    Q_OBJECT
    QScopedPointer<QTimer> timer;
    int timeoutMillis {300000};

  public:
    virtual ~SerialCommand() {};

    virtual Stm32SupportedOperations::Stm32SupportedOperationsEnums identifier() const {
        return Stm32SupportedOperations::UNKNOWN;
    }

    virtual void initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout) {
        Q_UNUSED(srcAddr)
        Q_UNUSED(dstAddr)
        Q_UNUSED(intervalMs)
        Q_UNUSED(timeout)

        timer->start(timeoutMillis);   // 5minutes timeout
    }
    virtual void iterate(const QDateTime &timestamp, const QString &data) = 0;


    void setTimeoutMillis(int newTimeoutMillis);

  protected:
    explicit SerialCommand(QObject *parent = nullptr);
    void write(const QByteArray &data);
    void cancelTimeout();


  signals:
    void writeCharacter(const QChar &character);
    void resultReceived(const QVariant &result);
    void timeout();
    void error();
};

#endif // SERIALCOMMAND_H

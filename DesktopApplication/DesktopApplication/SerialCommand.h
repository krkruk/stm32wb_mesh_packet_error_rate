#ifndef SERIALCOMMAND_H
#define SERIALCOMMAND_H
#include <QObject>

class QDateTime;
class QString;

class SerialCommand {
public:
    virtual ~SerialCommand() {};

    virtual void initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) = 0;
    virtual void iterate(const QDateTime &timestamp, const QString &data) = 0;
    virtual bool isComplete() const = 0;
};

#endif // SERIALCOMMAND_H

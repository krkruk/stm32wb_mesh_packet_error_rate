#ifndef GETADDRESSCOMMAND_H
#define GETADDRESSCOMMAND_H
#include "SerialCommand.h"
#include <functional>


class GetAddressCommand : public SerialCommand
{
    std::function<void(QByteArray)> write;
public:
    GetAddressCommand(std::function<void(QByteArray)> write);
    ~GetAddressCommand() override;
    void initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) override;
    void iterate(const QDateTime &timestamp, const QString &data) override;
    bool isComplete() const override;
};

#endif // GETADDRESSCOMMAND_H

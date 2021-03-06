#ifndef GETADDRESSCOMMAND_H
#define GETADDRESSCOMMAND_H
#include "SerialCommand.h"
#include <QRegularExpression>
#include <functional>
#include <memory>


class GetAddressCommand : public SerialCommand {
    Q_OBJECT
    static constexpr int DATA_COLUMN = 2;
    const QRegularExpression dataExtractRegex;
  public:
    virtual Stm32SupportedOperations::Stm32SupportedOperationsEnums identifier() const override {
        return Stm32SupportedOperations::GET_ADDRESS;
    }

    void initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout) override;

    ~GetAddressCommand() override;
    static std::unique_ptr<SerialCommand> create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout);
    static SerialCommand* create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout);

    void iterate(const QDateTime &timestamp, const QString &data) override;

  protected:
    explicit GetAddressCommand(QObject *parent);
};

#endif // GETADDRESSCOMMAND_H

#ifndef RUNPACKETERRORRATECOMMAND_H
#define RUNPACKETERRORRATECOMMAND_H

#include "SerialCommand.h"

class RunPacketErrorRateCommand : public SerialCommand {
    Q_OBJECT

  public:
    ~RunPacketErrorRateCommand() override;
    static std::unique_ptr<SerialCommand> create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout);
    static SerialCommand* create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout);

    Stm32SupportedOperations::Stm32SupportedOperationsEnums identifier() const override;
    void iterate(const QDateTime &timestamp, const QString &data) override;

  protected:
    explicit RunPacketErrorRateCommand(QObject *parent = nullptr);
    void initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) override;
};

#endif // RUNPACKETERRORRATECOMMAND_H

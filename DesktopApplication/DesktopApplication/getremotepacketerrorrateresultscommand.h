#ifndef GETREMOTEPACKETERRORRATERESULTSCOMMAND_H
#define GETREMOTEPACKETERRORRATERESULTSCOMMAND_H

#include "SerialCommand.h"

class GetRemotePacketErrorRateResultsCommand : public SerialCommand {
    Q_OBJECT
  public:
    ~GetRemotePacketErrorRateResultsCommand() override;
    static std::unique_ptr<SerialCommand> create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout);
    static SerialCommand* create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout);

    Stm32SupportedOperations::Stm32SupportedOperationsEnums identifier() const override;
    void iterate(const QDateTime &timestamp, const QString &data) override;

  protected:
    explicit GetRemotePacketErrorRateResultsCommand(QObject *parent = nullptr);
    void initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) override;
};

#endif // GETREMOTEPACKETERRORRATERESULTSCOMMAND_H

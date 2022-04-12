#include "getremotepacketerrorrateresultscommand.h"


GetRemotePacketErrorRateResultsCommand::~GetRemotePacketErrorRateResultsCommand() {

}

std::unique_ptr<SerialCommand> GetRemotePacketErrorRateResultsCommand::create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
    return std::unique_ptr<SerialCommand>(create(nullptr, srcAddr, dstAddr, intervalMs, timeout));
}

SerialCommand *GetRemotePacketErrorRateResultsCommand::create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
    SerialCommand *cmd = new GetRemotePacketErrorRateResultsCommand(parent);
    cmd->initialize(srcAddr, dstAddr, intervalMs, timeout);
    return cmd;
}

Stm32SupportedOperations::Stm32SupportedOperationsEnums GetRemotePacketErrorRateResultsCommand::identifier() const {
}

GetRemotePacketErrorRateResultsCommand::GetRemotePacketErrorRateResultsCommand(QObject *parent)
    : SerialCommand{parent} {

}

void GetRemotePacketErrorRateResultsCommand::initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
}

void GetRemotePacketErrorRateResultsCommand::iterate(const QDateTime &timestamp, const QString &data) {
}

#include "runpacketerrorratecommand.h"

RunPacketErrorRateCommand::RunPacketErrorRateCommand(QObject *parent)
    : SerialCommand{parent} {
}

RunPacketErrorRateCommand::~RunPacketErrorRateCommand() {
}

std::unique_ptr<SerialCommand> RunPacketErrorRateCommand::create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
    return std::unique_ptr<SerialCommand>(create(nullptr, srcAddr, dstAddr, intervalMs, timeout));
}

SerialCommand *RunPacketErrorRateCommand::create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
    SerialCommand *cmd = new RunPacketErrorRateCommand(parent);
    cmd->initialize(srcAddr, dstAddr, intervalMs, timeout);
    return cmd;
}

Stm32SupportedOperations::Stm32SupportedOperationsEnums RunPacketErrorRateCommand::identifier() const {
    return Stm32SupportedOperations::MEASURE_PER;
}

void RunPacketErrorRateCommand::initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
}

void RunPacketErrorRateCommand::iterate(const QDateTime &timestamp, const QString &data) {
}

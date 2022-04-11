#include "calibratecommand.h"

std::unique_ptr<SerialCommand> CalibrateCommand::create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
    return std::unique_ptr<SerialCommand>(create(nullptr, srcAddr, dstAddr, intervalMs, timeout));
}

SerialCommand *CalibrateCommand::create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
    CalibrateCommand *cmd = new CalibrateCommand(parent);
    cmd->initialize(srcAddr, dstAddr, intervalMs, timeout);
    return cmd;
}


CalibrateCommand::CalibrateCommand(QObject *parent) : SerialCommand{parent}
{}

void CalibrateCommand::initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
}

CalibrateCommand::~CalibrateCommand()
{

}

void CalibrateCommand::iterate(const QDateTime &timestamp, const QString &data)
{
}

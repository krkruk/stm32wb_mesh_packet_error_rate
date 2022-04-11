#include "calibratecommand.h"

std::unique_ptr<SerialCommand> CalibrateCommand::create(std::function<void (QByteArray)> write, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
    return std::unique_ptr<SerialCommand>(create(nullptr, write, srcAddr, dstAddr, intervalMs, timeout));
}

SerialCommand *CalibrateCommand::create(QObject *parent, std::function<void (QByteArray)> write, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
    CalibrateCommand *cmd = new CalibrateCommand(write, parent);
    cmd->initialize(srcAddr, dstAddr, intervalMs, timeout);
    return cmd;
}


CalibrateCommand::CalibrateCommand(std::function<void(QByteArray)> write, QObject *parent) : SerialCommand(write, parent)
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

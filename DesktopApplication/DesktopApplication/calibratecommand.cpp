#include "calibratecommand.h"
#include <QStringBuilder>

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

CalibrateCommand::~CalibrateCommand()
{

}

void CalibrateCommand::initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
    Q_UNUSED(srcAddr)
    Q_UNUSED(dstAddr)
    /*
     * COMMAND: ATAP SET-6 iiiiTTTT src dst
     *
     * iiii - interval in ticks
     * TTTT - stop after TTTT ticks
     * src - source, here 0x00 as the command is launched locally
     * dst - dst, here 0x00 as the command is launched locally
     */
    uint32_t timingsCmd = timeout;
    timingsCmd |= intervalMs << 16;

    const QString hexTimings = QString("%1").arg(timingsCmd, 8, 16, QChar('0'));
    const QString cmd = QString("ATAP SET-06 %1 0000 0000").arg(hexTimings);

    SerialCommand::initialize(srcAddr, dstAddr, intervalMs, timeout);
    write(cmd.toLocal8Bit());
}


void CalibrateCommand::iterate(const QDateTime &timestamp, const QString &data)
{
}

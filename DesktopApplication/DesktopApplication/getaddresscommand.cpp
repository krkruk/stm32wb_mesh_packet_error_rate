#include "getaddresscommand.h"
#include <QDebug>
#include <QStringBuilder>

GetAddressCommand::GetAddressCommand(QObject *parent)
    : SerialCommand{parent}, dataExtractRegex{".*\\=\\[([a-fA-F0-9]{0,8})\\]"}
{
}

GetAddressCommand::~GetAddressCommand()
{
}

std::unique_ptr<SerialCommand> GetAddressCommand::create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
    return std::unique_ptr<SerialCommand>(create(nullptr, srcAddr, dstAddr, intervalMs, timeout));
}

SerialCommand *GetAddressCommand::create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
    SerialCommand *cmd = new GetAddressCommand(parent);
    cmd->initialize(srcAddr, dstAddr, intervalMs, timeout);
    return cmd;
}


void GetAddressCommand::initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout)
{
    Q_UNUSED(srcAddr)
    Q_UNUSED(intervalMs)
    Q_UNUSED(timeout)

    // ATVR 0x0001-0xFFFF 0x0002 0x01 Board Type
    const QString address = QString("%1").arg(dstAddr, 4, 16, QChar('0'));
    const QString opcode = QString("%1").arg(0x2, 4, 16, QChar('0'));
    const QString boardTypeCmd = "01";

    const QString cmd =
            "ATVR "
            % address
            % " " % opcode
            % " " % boardTypeCmd;

    SerialCommand::initialize(srcAddr, dstAddr, intervalMs, timeout);
    write(cmd.toLocal8Bit());
}

void GetAddressCommand::iterate(const QDateTime &timestamp, const QString &data)
{
    Q_UNUSED(timestamp)
    /*
     * Retrieve the following line and extract the address:
     *   Vendor_OnResponseDataCb: elementIndex=[00], peer_addr=[c000], dst_peer_addr=[03], command=[02], Response=[00]
     *
     *   address can be found under dst_peer_addr (in hexadecimal)
     */
    if (data.startsWith("Vendor_OnResponseDataCb")) {
        const QStringList tokenized {data.split(", ")};
        const QString resultColumn = tokenized.at(DATA_COLUMN);

        const QRegularExpressionMatch match {dataExtractRegex.match(resultColumn)};
        if (match.isValid()) {
            const QString nodeAddress = match.captured(1);
            emit resultReceived(nodeAddress);
            cancelTimeout();
        }
    }
    else if (data.startsWith("Not Entered valid test parameters")) {
        emit error();
    }
}

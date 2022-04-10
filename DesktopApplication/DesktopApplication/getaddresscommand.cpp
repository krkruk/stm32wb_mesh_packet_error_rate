#include "getaddresscommand.h"
#include <QDebug>
#include <QStringBuilder>

GetAddressCommand::GetAddressCommand(std::function<void (QByteArray)> write)
    : SerialCommand{}, write{write}
{
}

GetAddressCommand::~GetAddressCommand()
{
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

//    QString cmd =
//            "ATVR "
//            % address
//            % " " % opcode
//            % " " % boardTypeCmd
//            % "\n";
//    QString cmd("ATCL 0004 8203 0100\n");
    QString cmd("ATAP SET-06 04008000 0000 0000\r\n");  //\r\n is mandatory!!!
    write(cmd.toLocal8Bit());
}

void GetAddressCommand::iterate(const QDateTime &timestamp, const QString &data)
{
}

bool GetAddressCommand::isComplete() const
{
    return false;
}

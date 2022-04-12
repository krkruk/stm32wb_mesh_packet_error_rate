#include "runpacketerrorratecommand.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QDebug>

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
    /*
     * COMMAND: ATAP SET-5 iiiiTTTT src dst
     *
     * iiii - interval in ticks
     * TTTT - stop after TTTT ticks
     * src - source, here 0x00 as the command is launched locally
     * dst - dst, here 0x00 as the command is launched locally
     */

    uint32_t timings = timeout;
    timings |= intervalMs << 16;

    QString cmd = QString("ATAP SET-05 %1 %2 %3")
                  .arg(timings, 8, 16, QChar('0'))
                  .arg(srcAddr, 4, 16, QChar('0'))
                  .arg(dstAddr, 4, 16, QChar('0'));

    SerialCommand::initialize(srcAddr, dstAddr, intervalMs, timeout);
    write(cmd.toLocal8Bit());
}

void RunPacketErrorRateCommand::iterate(const QDateTime &timestamp, const QString &data) {
    // sample results for query ATAP SET-05 04008000 0003 0004
    // expected results like: {"func":"SET-05","addr":"0003","counter":31,"HAL_TickFreq":1,"startTick":541677,"stopTick":557674,"tick_diff":-15997}
    QJsonParseError parseStatus;
    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit(), &parseStatus);
    if (parseStatus.error != QJsonParseError::NoError) {
        qDebug() << "Cannot parse '" << data << "' because of:" << parseStatus.errorString();
        return;
    }

    QJsonValue function = doc["func"];
    QJsonValue tick_diff = doc["tick_diff"];
    QJsonValue counter = doc["counter"];

    if (!(tick_diff.isDouble() && tick_diff.toDouble())) {
        return;
    }
    if (!(function.isString() && function.toString().startsWith("SET-05"))) {
        return;
    }

    qDebug() << "Final result = " << data;
    emit resultReceived(counter.toInt());
}

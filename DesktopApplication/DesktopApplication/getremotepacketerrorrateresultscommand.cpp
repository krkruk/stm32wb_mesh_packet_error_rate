#include "getremotepacketerrorrateresultscommand.h"
#include <QJsonDocument>
#include <QJsonValue>


GetRemotePacketErrorRateResultsCommand::~GetRemotePacketErrorRateResultsCommand() {

}

std::unique_ptr<SerialCommand> GetRemotePacketErrorRateResultsCommand::create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout) {
    return std::unique_ptr<SerialCommand>(create(nullptr, srcAddr, dstAddr, intervalMs, timeout));
}

SerialCommand *GetRemotePacketErrorRateResultsCommand::create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout) {
    SerialCommand *cmd = new GetRemotePacketErrorRateResultsCommand(parent);
    cmd->initialize(srcAddr, dstAddr, intervalMs, timeout);
    return cmd;
}

Stm32SupportedOperations::Stm32SupportedOperationsEnums GetRemotePacketErrorRateResultsCommand::identifier() const {
    return Stm32SupportedOperations::GET_PER_RESULT;
}

GetRemotePacketErrorRateResultsCommand::GetRemotePacketErrorRateResultsCommand(QObject *parent)
    : SerialCommand{parent} {

}

void GetRemotePacketErrorRateResultsCommand::initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout) {
    // ATAP GET-05 00000000 srcAddr dstAddr

    QString cmd = QString("ATAP GET-05 000000000000 %1 %2")
                  .arg(srcAddr, 2, 16, QChar('0'))
                  .arg(dstAddr, 2, 16, QChar('0'));

    setTimeoutMillis(10000); // 10 seconds timeout
    SerialCommand::initialize(srcAddr, dstAddr, intervalMs, timeout);
    write(cmd.toLocal8Bit());
}

void GetRemotePacketErrorRateResultsCommand::iterate(const QDateTime &timestamp, const QString &data) {
    Q_UNUSED(timestamp)

    // expected results like results: {"func":"GET-05","addr":"0x0004","counter":0,"peer":04}
    QJsonParseError parseStatus;
    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit(), &parseStatus);
    if (parseStatus.error != QJsonParseError::NoError) {
        qDebug() << "Cannot parse '" << data << "' because of:" << parseStatus.errorString();
        return;
    }

    QJsonValue function = doc["func"];
    QJsonValue address = doc["addr"];
    QJsonValue counter = doc["counter"];

    if (!(function.isString() && function.toString().startsWith("GET-05"))) {
        return;
    }

    emit resultReceived(counter.toInt());
}

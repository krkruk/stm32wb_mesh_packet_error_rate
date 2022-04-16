#include "calibratecommand.h"
#include <QStringBuilder>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonValue>
#include <QTimer>

#include <algorithm>
#include <math.h>

std::unique_ptr<SerialCommand> CalibrateCommand::create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout) {
    return std::unique_ptr<SerialCommand>(create(nullptr, srcAddr, dstAddr, intervalMs, timeout));
}

SerialCommand *CalibrateCommand::create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout) {
    CalibrateCommand *cmd = new CalibrateCommand(parent);
    cmd->initialize(srcAddr, dstAddr, intervalMs, timeout);
    return cmd;
}


CalibrateCommand::CalibrateCommand(QObject *parent) : SerialCommand{parent}
{}

CalibrateCommand::~CalibrateCommand() {

}

void CalibrateCommand::initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint32_t timeout) {
    Q_UNUSED(srcAddr)
    Q_UNUSED(dstAddr)
    /*
     * COMMAND: ATAP SET-6 iiiiTTTTTT src dst
     *
     * iiii - interval in ticks - 16 bits
     * TTTTTT - stop after TTTT ticks - 32 bits
     * src - source, here 0x00 as the command is launched locally
     * dst - dst, here 0x00 as the command is launched locally
     */
    expectedInterval = intervalMs;
    previousMeanTimestampDiff = expectedInterval;
    this->timeout = timeout;
    calibrate(intervalMs, timeout); // initialize ticks as expected milliseconds
}


void CalibrateCommand::iterate(const QDateTime &timestamp, const QString &data) {
    if (initializationError) {
        emit error();
    }
    QJsonParseError parseStatus;
    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit(), &parseStatus);
    if (parseStatus.error != QJsonParseError::NoError) {
        qDebug() << "Cannot parse '" << data << "' because of:" << parseStatus.errorString();
        return;
    }

    QJsonValue status = doc[KEY_STATUS];
    QJsonValue counter = doc[KEY_COUNTER];
    QJsonValue tick_diff = doc[KEY_TICK_DIFF];

    if (status.toString("not_running").startsWith("running")) {
        timestamps.append(timestamp);
    } else if (tick_diff.isDouble()) {
        auto result = computeNewInterval();
        if (result.second > TICK_TOLERANCE_THRESHOLD) { // 5% of difference
            qDebug() << "Test New tick interval =" << result.first;
            QTimer::singleShot(INTEVAL_BETWEEN_ATTEMPTS_MS, Qt::CoarseTimer, this, [newInterval=result.first, this]() {
                calibrate(newInterval, timeout);
            });
            return;
        }

        settings.setValue(QString("millis.to.ticks.%1").arg(expectedInterval), currentInterval);
        settings.sync();
        emit resultReceived(currentInterval);
    }
}

QString CalibrateCommand::generateCommand(uint16_t intervalMs, uint32_t timeout) {
    uint64_t timingsCmd = timeout;
    timingsCmd |= static_cast<uint64_t>(intervalMs) << 32;
// 00000000 0000 0000
// 000000000000 00 00
    const QString hexTimings = QString("%1").arg(timingsCmd, 12, 16, QChar('0'));
    return QString("ATAP SET-06 %1 00 00").arg(hexTimings);
}

void CalibrateCommand::calibrate(uint16_t newIntervalTicks, uint32_t timeout) {
    if (newIntervalTicks < 1 || timeout < 1) {
        qDebug() << "Interval and Timeout must be greater than 0";
        initializationError = true;
        write("\r");
        return;
    }

    currentInterval = newIntervalTicks;
    timestamps.clear();
    QString cmd {generateCommand(newIntervalTicks, timeout)};
    SerialCommand::initialize(0, 0, newIntervalTicks, timeout);
    write(cmd.toLocal8Bit());
}

std::pair<uint16_t, double> CalibrateCommand::computeNewInterval() {
    QVector<qint64> timestampDiffs;
    std::transform(std::cbegin(timestamps), std::cend(timestamps),
                   std::back_inserter(timestampDiffs),
                   [] (const QDateTime &timestamp) -> long { return timestamp.toMSecsSinceEpoch(); });

    std::adjacent_difference(std::begin(timestampDiffs), std::end(timestampDiffs), std::begin(timestampDiffs));
    timestampDiffs.removeFirst();

    qint64 sum = 0;
    for (qint64 elem : qAsConst(timestampDiffs)) {
        sum += elem;
    }
    double meanTimestampDiff = sum / static_cast<double>(timestampDiffs.size());
    double accuracy = abs((meanTimestampDiff - expectedInterval) / static_cast<double>(expectedInterval));
    double change = meanTimestampDiff > previousMeanTimestampDiff
                    ? -1.2 : 1.2;

    previousMeanTimestampDiff = meanTimestampDiff;
    double correctionCoeff = accuracy * currentInterval;
    double interval = currentInterval + change * correctionCoeff;

    qDebug() << "SUM=" << sum << "DIFF = " << meanTimestampDiff << "interval="<< interval << "accuracy=" << accuracy;
    return std::make_pair(interval, abs(accuracy));
}

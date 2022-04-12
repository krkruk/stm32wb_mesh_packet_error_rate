#include "calibratecommand.h"
#include <QStringBuilder>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonValue>
#include <QTimer>

#include <algorithm>
#include <math.h>

std::unique_ptr<SerialCommand> CalibrateCommand::create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
    return std::unique_ptr<SerialCommand>(create(nullptr, srcAddr, dstAddr, intervalMs, timeout));
}

SerialCommand *CalibrateCommand::create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
    CalibrateCommand *cmd = new CalibrateCommand(parent);
    cmd->initialize(srcAddr, dstAddr, intervalMs, timeout);
    return cmd;
}


CalibrateCommand::CalibrateCommand(QObject *parent) : SerialCommand{parent}
{}

CalibrateCommand::~CalibrateCommand() {

}

void CalibrateCommand::initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) {
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
    expectedInterval = intervalMs;
    this->timeout = timeout;
    calibrate(intervalMs, timeout);
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
        const QString dateBucket = timestamp.toString(Qt::ISODate);
        if (countsPerSecond.isEmpty()) {
            startTimestamp = timestamp;
            countsPerSecond[dateBucket] = 0;
        }
        ++(countsPerSecond[dateBucket]);
    } else if (tick_diff.isDouble()) {
        double millisPerCountBucketed = computeMillisPerCount();
        const double percentageDiff = (millisPerCountBucketed - expectedInterval) / expectedInterval;
        uint16_t newInterval = (1 - percentageDiff) * currentInterval;
        if (percentageDiff < TICK_TOLERANCE_THRESHOLD && newInterval != currentInterval) { // 5% of difference
            qDebug() << "Test New tick interval =" << newInterval;
            QTimer::singleShot(INTEVAL_BETWEEN_ATTEMPTS_MS, Qt::CoarseTimer, this, [newInterval, this]() {
                calibrate(newInterval, timeout);
            });
            return;
        }

        settings.setValue(QString("millis.to.ticks.%1").arg(expectedInterval), currentInterval);
        emit resultReceived(currentInterval);
    }
}

QString CalibrateCommand::generateCommand(uint16_t intervalMs, uint16_t timeout) {
    uint32_t timingsCmd = timeout;
    timingsCmd |= intervalMs << 16;

    const QString hexTimings = QString("%1").arg(timingsCmd, 8, 16, QChar('0'));
    return QString("ATAP SET-06 %1 0000 0000").arg(hexTimings);
}

void CalibrateCommand::calibrate(uint16_t newIntervalTicks, uint16_t timeout) {
    if (newIntervalTicks < 1 || timeout < 1) {
        qDebug() << "Interval and Timeout must be greater than 0";
        initializationError = true;
        write("\r");
        return;
    }

    currentInterval = newIntervalTicks;
    countsPerSecond.clear();
    QString cmd {generateCommand(newIntervalTicks, timeout)};
    SerialCommand::initialize(0, 0, newIntervalTicks, timeout);
    write(cmd.toLocal8Bit());
}

double CalibrateCommand::computeMillisPerCount() {
    // case 1 - average count in bucketed set
    countsPerSecond.remove(countsPerSecond.firstKey()); // remove the first and the last case
    countsPerSecond.remove(countsPerSecond.lastKey());
    QList<uint32_t> cntValues = countsPerSecond.values();

    uint32_t bucketedCountSum = std::accumulate(std::cbegin(cntValues), std::cend(cntValues), 0,
    [](uint32_t sum, uint32_t elem) {
        return sum + elem;
    });
    double pingsPerMillisecondBucketed = bucketedCountSum / static_cast<double>(countsPerSecond.size()) / 1000.0f;
    return (1 / pingsPerMillisecondBucketed);

    // case 2 - number of ticks ie 200ticks=100ms
//        finishTimestamp = timestamp;
//        qint64 timediff = {(finishTimestamp.toMSecsSinceEpoch() - startTimestamp.toMSecsSinceEpoch())};
//        double pingsPerMillisecond = counter.toDouble() / (1.0f * timediff);
//        double millisPerCount = 1 / pingsPerMillisecond;
}

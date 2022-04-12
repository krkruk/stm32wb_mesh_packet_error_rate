#ifndef CALIBRATECOMMAND_H
#define CALIBRATECOMMAND_H
#include <QMap>
#include <QDateTime>
#include <QSettings>

#include <memory>

#include "SerialCommand.h"

class CalibrateCommand : public SerialCommand {
    Q_OBJECT
    static constexpr const double TICK_TOLERANCE_THRESHOLD = 0.05;
    static constexpr const int INTEVAL_BETWEEN_ATTEMPTS_MS = 5000;
    static constexpr const char *const KEY_STATUS = "status";
    static constexpr const char *const KEY_FUNC = "func";
    static constexpr const char *const KEY_COUNTER = "counter";
    static constexpr const char *const KEY_TICK_DIFF = "tick_diff";

    bool initializationError {false};
    uint16_t expectedInterval {0};
    uint16_t currentInterval {0};
    uint16_t timeout {0};
    QMap<QString, uint32_t> countsPerSecond;
    QDateTime startTimestamp;
    QDateTime finishTimestamp;
    QSettings settings;

  public:
    static std::unique_ptr<SerialCommand> create(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout);
    static SerialCommand* create(QObject *parent, uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout);

    ~CalibrateCommand() override;

    Stm32SupportedOperations::Stm32SupportedOperationsEnums identifier() const override {
        return Stm32SupportedOperations::CALIBRATE;
    }

    void iterate(const QDateTime &timestamp, const QString &data) override;

  protected:
    explicit CalibrateCommand(QObject *parent);
    void initialize(uint16_t srcAddr, uint16_t dstAddr, uint16_t intervalMs, uint16_t timeout) override;

  private:
    QString generateCommand(uint16_t intervalMs, uint16_t timeout);
    void calibrate(uint16_t newIntervalTicks, uint16_t timeout);
    double computeMillisPerCount();
};

#endif // CALIBRATECOMMAND_H

#ifndef CALIBRATECOMMAND_H
#define CALIBRATECOMMAND_H
#include <memory>

#include "SerialCommand.h"

class CalibrateCommand : public SerialCommand
{
    Q_OBJECT
    bool initializationError {false};

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
};

#endif // CALIBRATECOMMAND_H

#ifndef STM32SUPPORTEDOPERATIONS_H
#define STM32SUPPORTEDOPERATIONS_H
#include <QObject>


class Stm32SupportedOperations {
    Q_GADGET
public:
    enum Stm32SupportedOperationsEnums {
        UNKNOWN,
        GET_ADDRESS,
        CALIBRATE,
        MEASURE_PER
    };
  Q_ENUM(Stm32SupportedOperationsEnums)
};

#endif // STM32SUPPORTEDOPERATIONS_H

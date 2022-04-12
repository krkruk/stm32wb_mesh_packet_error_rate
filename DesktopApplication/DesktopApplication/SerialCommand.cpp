#include "SerialCommand.h"


void SerialCommand::setTimeoutMillis(int newTimeoutMillis)
{
    timeoutMillis = newTimeoutMillis;
}

SerialCommand::SerialCommand(QObject *parent)
    : QObject{parent}, timer{new QTimer} {
    timer->setSingleShot(true);
    timer->setTimerType(Qt::CoarseTimer);
    connect(timer.data(), &QTimer::timeout, this, &SerialCommand::timeout);
}

void SerialCommand::write(const QByteArray &data) {
    // Simulate typing in a standard serial terminal so the STM32WB can keep up...
    LineMessageDispatcher *dispatcher = new LineMessageDispatcher(data);
    connect(dispatcher, &LineMessageDispatcher::writeCharacter, this, &SerialCommand::writeCharacter, Qt::QueuedConnection);
    QThreadPool::globalInstance()->start(dispatcher);
}

void SerialCommand::cancelTimeout() {
    if (timer) {
        timer->stop();
    }
}

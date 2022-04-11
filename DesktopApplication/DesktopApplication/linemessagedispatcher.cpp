#include "linemessagedispatcher.h"
#include <QDebug>
#include <QThread>

LineMessageDispatcher::LineMessageDispatcher(QByteArray line, QObject *parent)
    : QObject{parent}, QRunnable{}, line{line}
{
    setAutoDelete(true);
}

LineMessageDispatcher::~LineMessageDispatcher()
{
    qDebug() << "Killing dispatcher...";
}


void LineMessageDispatcher::run()
{
    qDebug() << "Command to dispatch" << qPrintable(line);

    for (int i = 0; i < line.size(); i++) {
        QChar character = line.at(i);
        qDebug() << "Write CHAR = " << character;
        emit writeCharacter(character);
        QThread::currentThread()->msleep(MOCK_TYPING_SPEED_MS);
    }
    emit writeCharacter('\r'); // carriage return must be sent to activate a query!
}


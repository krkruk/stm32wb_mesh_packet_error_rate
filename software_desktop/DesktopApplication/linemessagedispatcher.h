#ifndef LINEMESSAGEDISPATCHER_H
#define LINEMESSAGEDISPATCHER_H

#include <QObject>
#include <QRunnable>
#include <QByteArray>
#include <QChar>
#include <functional>

class LineMessageDispatcher : public QObject, public QRunnable
{
    Q_OBJECT
    static constexpr int MOCK_TYPING_SPEED_MS = 100;
    const QByteArray line;
public:
    LineMessageDispatcher(QByteArray line, QObject *parent=nullptr);
    ~LineMessageDispatcher() override;

    // QRunnable interface
public:
    void run() override;

signals:
    void writeCharacter(const QChar &character);
};

#endif // LINEMESSAGEDISPATCHER_H

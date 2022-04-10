#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "serialmanager.h"
#include "serialnodeconnector.h"
#include "stm32supportedoperations.h"


static void registerTypes();

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    registerTypes();
    engine.load(url);

    return app.exec();
}

void registerTypes() {
    qmlRegisterSingletonType<SerialManager>("stm32.SerialManager", 1, 0, "Serial",
                                            [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new SerialManager();
    });

    qmlRegisterType<SerialNodeConnector>("stm32.SerialManager", 1, 0, "SerialConnector");
    qmlRegisterUncreatableType<Stm32SupportedOperations>("stm32.SerialManager", 1, 0, "Stm32SupportedOperations", "Cannot instantiate an enum");
}

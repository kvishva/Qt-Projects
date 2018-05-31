#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <winclient.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<WinClient>("learnsafe.winclient",1,0,"WinClient");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    WinClient myClient;

    QObject *wind = engine.rootObjects().first();
    QObject *rect = wind->findChild<QObject*>("rectButtReg");
    QObject::connect(rect,SIGNAL(qmlSignal(QString)),&myClient,SLOT(sendRegistration(QString)));

    return app.exec();
}

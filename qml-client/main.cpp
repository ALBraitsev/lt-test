#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Model.h"

extern QObject *topLevel;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Model>("com.lt.client", 1, 0, "Model");
    QQmlApplicationEngine engine(":/res/client.qml");
    topLevel = engine.rootObjects().value(0);

    return app.exec();
}

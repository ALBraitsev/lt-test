#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Model.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Model>("com.lt.client", 1, 0, "Model");
    QQmlApplicationEngine engine(":/res/client.qml");

    return app.exec();
}

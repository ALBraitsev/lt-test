#include <QGuiApplication>
// #include <QQuickView>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // QQuickView *view = new QQuickView;
    // view->setSource(QUrl::fromLocalFile(":/res/client.qml"));
    // view->show();

    QQmlApplicationEngine engine(":/res/client.qml");


    return app.exec();
}

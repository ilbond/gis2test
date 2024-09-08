#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "filemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<FileManager>("Gis", 1, 0, "FileManager");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("gis2test", "Main");

    return app.exec();
}

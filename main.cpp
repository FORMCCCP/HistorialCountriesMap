#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>


#include "src/control/totalcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    TotalController* controller = new TotalController();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Maincontroller", controller);

    engine.loadFromModule("HistorialCountriesMaps", "Main");

    return QCoreApplication::exec();
}

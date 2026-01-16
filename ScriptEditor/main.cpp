#pragma execution_character_set("utf-8")
#include "mainwindow.h"
//--------------------------------------------------------------
#include <QApplication>
#include <QStyleFactory>
//--------------------------------------------------------------
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);


    QCoreApplication::setOrganizationName("Velog");
    QCoreApplication::setApplicationName("ScriptEditor");
    QCoreApplication::setApplicationVersion("1.0");

    a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.show();
    return a.exec();
}

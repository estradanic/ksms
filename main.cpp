#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    execute("mkdir ~/.ksms");
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

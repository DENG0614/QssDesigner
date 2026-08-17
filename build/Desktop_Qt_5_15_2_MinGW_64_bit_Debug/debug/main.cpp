#include <QApplication>
#include <QDir>
#include <QFileInfo>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    QString dir = QCoreApplication::applicationDirPath();
    if (!QFileInfo(dir + QStringLiteral("/MyProject.ui")).exists())
        dir = QDir::currentPath();

    if (!w.loadFromUi(dir + QStringLiteral("/MyProject.ui"),
                       dir + QStringLiteral("/MyProject.qss"))) {
        w.setWindowTitle(QStringLiteral("MyProject"));
        w.resize(960, 640);
    }
    w.show();
    return app.exec();
}

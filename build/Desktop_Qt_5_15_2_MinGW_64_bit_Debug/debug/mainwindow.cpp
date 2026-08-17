#include "mainwindow.h"

#include <QFile>
#include <QDockWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QUiLoader>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

bool MainWindow::loadFromUi(const QString &uiPath, const QString &qssPath)
{
    QFile uiFile(uiPath);
    if (!uiFile.open(QIODevice::ReadOnly))
        return false;

    QUiLoader loader;
    QWidget *ui = loader.load(&uiFile, this);
    uiFile.close();
    if (!ui)
        return false;

    if (QMainWindow *mw = qobject_cast<QMainWindow *>(ui)) {
        setCentralWidget(mw->centralWidget());
        setWindowTitle(mw->windowTitle());
        setWindowIcon(mw->windowIcon());
        if (mw->menuBar())
            setMenuBar(mw->menuBar());
        if (mw->statusBar())
            setStatusBar(mw->statusBar());
        Q_FOREACH (QToolBar *tb, mw->findChildren<QToolBar *>())
            addToolBar(tb);
        Q_FOREACH (QDockWidget *dw, mw->findChildren<QDockWidget *>())
            addDockWidget(Qt::LeftDockWidgetArea, dw);
        mw->deleteLater();
    } else {
        setCentralWidget(ui);
    }

    QFile qss(qssPath);
    if (qss.open(QIODevice::ReadOnly)) {
        setStyleSheet(QString::fromUtf8(qss.readAll()));
        qss.close();
    }
    resize(960, 640);
    return true;
}

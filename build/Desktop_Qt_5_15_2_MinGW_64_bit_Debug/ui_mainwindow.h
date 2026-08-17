/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "designview.h"
#include "previewwidget.h"
#include "propertypanel.h"
#include "qsseditor.h"
#include "widgetpalette.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionExportUi;
    QAction *actionExportProject;
    QAction *actionExit;
    QAction *actionDelete;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionAlignLeft;
    QAction *actionAlignRight;
    QAction *actionAlignTop;
    QAction *actionAlignBottom;
    QAction *actionAlignHCenter;
    QAction *actionAlignVCenter;
    QAction *actionEqualWidth;
    QAction *actionEqualHeight;
    QAction *actionBringToFront;
    QAction *actionSendToBack;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionZoomReset;
    QAction *actionSaveCustomStyle;
    QAction *actionAbout;
    QWidget *centralwidget;
    DesignView *centralView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuAlign;
    QMenu *menuOrder;
    QMenu *menuView;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QLabel *styleLabel;
    QComboBox *styleCombo;
    QDockWidget *widgetsDock;
    WidgetPalette *widgetsPalette;
    QDockWidget *propertyDock;
    PropertyPanel *propertyPanel;
    QDockWidget *styleDock;
    QssEditor *qssEditor;
    QDockWidget *previewDock;
    PreviewWidget *previewWidget;
    QStatusBar *statusbar;
    QLabel *statusLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 800);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionExportUi = new QAction(MainWindow);
        actionExportUi->setObjectName(QString::fromUtf8("actionExportUi"));
        actionExportProject = new QAction(MainWindow);
        actionExportProject->setObjectName(QString::fromUtf8("actionExportProject"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionAlignLeft = new QAction(MainWindow);
        actionAlignLeft->setObjectName(QString::fromUtf8("actionAlignLeft"));
        actionAlignRight = new QAction(MainWindow);
        actionAlignRight->setObjectName(QString::fromUtf8("actionAlignRight"));
        actionAlignTop = new QAction(MainWindow);
        actionAlignTop->setObjectName(QString::fromUtf8("actionAlignTop"));
        actionAlignBottom = new QAction(MainWindow);
        actionAlignBottom->setObjectName(QString::fromUtf8("actionAlignBottom"));
        actionAlignHCenter = new QAction(MainWindow);
        actionAlignHCenter->setObjectName(QString::fromUtf8("actionAlignHCenter"));
        actionAlignVCenter = new QAction(MainWindow);
        actionAlignVCenter->setObjectName(QString::fromUtf8("actionAlignVCenter"));
        actionEqualWidth = new QAction(MainWindow);
        actionEqualWidth->setObjectName(QString::fromUtf8("actionEqualWidth"));
        actionEqualHeight = new QAction(MainWindow);
        actionEqualHeight->setObjectName(QString::fromUtf8("actionEqualHeight"));
        actionBringToFront = new QAction(MainWindow);
        actionBringToFront->setObjectName(QString::fromUtf8("actionBringToFront"));
        actionSendToBack = new QAction(MainWindow);
        actionSendToBack->setObjectName(QString::fromUtf8("actionSendToBack"));
        actionZoomIn = new QAction(MainWindow);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        actionZoomOut = new QAction(MainWindow);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        actionZoomReset = new QAction(MainWindow);
        actionZoomReset->setObjectName(QString::fromUtf8("actionZoomReset"));
        actionSaveCustomStyle = new QAction(MainWindow);
        actionSaveCustomStyle->setObjectName(QString::fromUtf8("actionSaveCustomStyle"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralView = new DesignView(centralwidget);
        centralView->setObjectName(QString::fromUtf8("centralView"));
        centralView->setFrameShape(QFrame::StyledPanel);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1280, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuAlign = new QMenu(menuEdit);
        menuAlign->setObjectName(QString::fromUtf8("menuAlign"));
        menuOrder = new QMenu(menuEdit);
        menuOrder->setObjectName(QString::fromUtf8("menuOrder"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        styleLabel = new QLabel(mainToolBar);
        styleLabel->setObjectName(QString::fromUtf8("styleLabel"));
        mainToolBar->addWidget(styleLabel);
        styleCombo = new QComboBox(mainToolBar);
        styleCombo->setObjectName(QString::fromUtf8("styleCombo"));
        styleCombo->setMinimumSize(QSize(280, 0));
        mainToolBar->addWidget(styleCombo);
        MainWindow->addToolBar(mainToolBar);
        widgetsDock = new QDockWidget(MainWindow);
        widgetsDock->setObjectName(QString::fromUtf8("widgetsDock"));
        widgetsPalette = new WidgetPalette();
        widgetsPalette->setObjectName(QString::fromUtf8("widgetsPalette"));
        widgetsDock->setWidget(widgetsPalette);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, widgetsDock);
        propertyDock = new QDockWidget(MainWindow);
        propertyDock->setObjectName(QString::fromUtf8("propertyDock"));
        propertyPanel = new PropertyPanel();
        propertyPanel->setObjectName(QString::fromUtf8("propertyPanel"));
        propertyDock->setWidget(propertyPanel);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, propertyDock);
        styleDock = new QDockWidget(MainWindow);
        styleDock->setObjectName(QString::fromUtf8("styleDock"));
        qssEditor = new QssEditor();
        qssEditor->setObjectName(QString::fromUtf8("qssEditor"));
        styleDock->setWidget(qssEditor);
        MainWindow->addDockWidget(Qt::BottomDockWidgetArea, styleDock);
        previewDock = new QDockWidget(MainWindow);
        previewDock->setObjectName(QString::fromUtf8("previewDock"));
        previewDock->setMinimumSize(QSize(380, 0));
        previewWidget = new PreviewWidget();
        previewWidget->setObjectName(QString::fromUtf8("previewWidget"));
        previewDock->setWidget(previewWidget);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, previewDock);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusLabel = new QLabel(statusbar);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionExportUi);
        menuFile->addAction(actionExportProject);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionDelete);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuAlign->addAction(actionAlignLeft);
        menuAlign->addAction(actionAlignRight);
        menuAlign->addAction(actionAlignTop);
        menuAlign->addAction(actionAlignBottom);
        menuAlign->addSeparator();
        menuAlign->addAction(actionAlignHCenter);
        menuAlign->addAction(actionAlignVCenter);
        menuAlign->addSeparator();
        menuAlign->addAction(actionEqualWidth);
        menuAlign->addAction(actionEqualHeight);
        menuOrder->addAction(actionBringToFront);
        menuOrder->addAction(actionSendToBack);
        menuView->addAction(actionZoomIn);
        menuView->addAction(actionZoomOut);
        menuView->addAction(actionZoomReset);
        menuHelp->addAction(actionAbout);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionExportUi);
        mainToolBar->addAction(actionExportProject);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSaveCustomStyle);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "QssDesigner - \345\217\257\350\247\206\345\214\226 QSS \347\225\214\351\235\242\350\256\276\350\256\241\345\231\250", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "\346\226\260\345\273\272(&N)", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExportUi->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272 .ui \346\226\207\344\273\266...", nullptr));
#if QT_CONFIG(shortcut)
        actionExportUi->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExportProject->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272 Qt \345\267\245\347\250\213...", nullptr));
#if QT_CONFIG(shortcut)
        actionExportProject->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272(&X)", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDelete->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244(&D)", nullptr));
#if QT_CONFIG(shortcut)
        actionDelete->setShortcut(QCoreApplication::translate("MainWindow", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCopy->setText(QCoreApplication::translate("MainWindow", "\345\244\215\345\210\266(&C)", nullptr));
#if QT_CONFIG(shortcut)
        actionCopy->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPaste->setText(QCoreApplication::translate("MainWindow", "\347\262\230\350\264\264(&P)", nullptr));
#if QT_CONFIG(shortcut)
        actionPaste->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAlignLeft->setText(QCoreApplication::translate("MainWindow", "\345\267\246\345\257\271\351\275\220", nullptr));
        actionAlignRight->setText(QCoreApplication::translate("MainWindow", "\345\217\263\345\257\271\351\275\220", nullptr));
        actionAlignTop->setText(QCoreApplication::translate("MainWindow", "\351\241\266\351\203\250\345\257\271\351\275\220", nullptr));
        actionAlignBottom->setText(QCoreApplication::translate("MainWindow", "\345\272\225\351\203\250\345\257\271\351\275\220", nullptr));
        actionAlignHCenter->setText(QCoreApplication::translate("MainWindow", "\346\260\264\345\271\263\345\261\205\344\270\255", nullptr));
        actionAlignVCenter->setText(QCoreApplication::translate("MainWindow", "\345\236\202\347\233\264\345\261\205\344\270\255", nullptr));
        actionEqualWidth->setText(QCoreApplication::translate("MainWindow", "\347\255\211\345\256\275", nullptr));
        actionEqualHeight->setText(QCoreApplication::translate("MainWindow", "\347\255\211\351\253\230", nullptr));
        actionBringToFront->setText(QCoreApplication::translate("MainWindow", "\347\275\256\351\241\266", nullptr));
        actionSendToBack->setText(QCoreApplication::translate("MainWindow", "\347\275\256\345\272\225", nullptr));
        actionZoomIn->setText(QCoreApplication::translate("MainWindow", "\346\224\276\345\244\247", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomIn->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl++", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomOut->setText(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomOut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+-", nullptr));
#endif // QT_CONFIG(shortcut)
        actionZoomReset->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256\347\274\251\346\224\276", nullptr));
#if QT_CONFIG(shortcut)
        actionZoomReset->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+0", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSaveCustomStyle->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\350\207\252\345\256\232\344\271\211\346\240\267\345\274\217", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216(&A)", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221(&E)", nullptr));
        menuAlign->setTitle(QCoreApplication::translate("MainWindow", "\345\257\271\351\275\220(&A)", nullptr));
        menuOrder->setTitle(QCoreApplication::translate("MainWindow", "\351\241\272\345\272\217(&O)", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "\350\247\206\345\233\276(&V)", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", nullptr));
        styleLabel->setText(QCoreApplication::translate("MainWindow", "\346\240\267\345\274\217\357\274\232", nullptr));
        widgetsDock->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\216\247\344\273\266\345\272\223", nullptr));
        propertyDock->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\261\236\346\200\247", nullptr));
        styleDock->setWindowTitle(QCoreApplication::translate("MainWindow", "QSS \346\240\267\345\274\217\347\274\226\350\276\221\345\231\250", nullptr));
        previewDock->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\216\247\344\273\266\351\242\204\350\247\210\357\274\210Ctrl+\346\213\226\346\213\275\345\210\260\347\224\273\345\270\203\357\274\211", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\345\260\261\347\273\252", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

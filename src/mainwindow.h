#ifndef MAINWINDOW_H
#pragma execution_character_set("utf-8")
#define MAINWINDOW_H

#include <QMainWindow>

#include "ui_mainwindow.h"
#include "designscene.h"

class DesignScene;
class DesignView;
class DesignItem;
class WidgetPalette;
class PropertyPanel;
class StyleManager;
class QssEditor;
class PreviewWidget;

class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool exportToDir(const QString &dir, QString *error = 0);
    void addAllWidgetTypes();
    DesignView *designView() const { return m_view; }
    PreviewWidget *previewWidget() const { return ui.previewWidget; }
    int sceneItemCount() const { return m_scene ? m_scene->allItems().size() : 0; }
    int styledItemCount() const;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onSelectionChanged();
    void applyCurrentStyle();
    void saveCustomStyle();
    void exportUi();
    void exportProject();
    void addWidgetFromPalette(const QString &typeName, const QVariant &variant);
    void newDesign();

private:
    void populateStyleCombo();
    void updateStatusBar();

    Ui::MainWindow ui;
    DesignScene *m_scene;
    DesignView *m_view;
    WidgetPalette *m_palette;
    PropertyPanel *m_propertyPanel;
    StyleManager *m_styleManager;
    QComboBox *m_styleCombo;
    QLabel *m_statusLabel;
    QssEditor *m_qssEditor;
    PreviewWidget *m_previewWidget;
    QString m_exportPath;
    QTimer *m_applyTimer;
};

#endif // MAINWINDOW_H

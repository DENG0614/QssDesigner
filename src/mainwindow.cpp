#pragma execution_character_set("utf-8")

#include "mainwindow.h"

#include "designitem.h"
#include "designscene.h"
#include "designview.h"
#include "propertypanel.h"
#include "previewwidget.h"
#include "projectwriter.h"
#include "qsseditor.h"
#include "stylemanager.h"
#include "uiwriter.h"
#include "widgetpalette.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QTimer>

namespace {

QString appStylesDir()
{
    const QString appDir = QCoreApplication::applicationDirPath();
    QStringList candidates;
    candidates << appDir + QStringLiteral("/styles");
    // 从构建目录向上找源码目录（build/release -> 工程根目录）
    QString up = QFileInfo(appDir).absolutePath();
    for (int i = 0; i < 3 && !up.isEmpty(); ++i) {
        candidates << up + QStringLiteral("/styles");
        up = QFileInfo(up).absolutePath();
    }
    candidates << QDir::currentPath() + QStringLiteral("/styles");

    Q_FOREACH (const QString &dir, candidates) {
        if (QDir(dir).exists())
            return dir;
    }
    return appDir + QStringLiteral("/styles");
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_scene(0)
    , m_view(0)
    , m_palette(0)
    , m_propertyPanel(0)
    , m_styleManager(0)
    , m_styleCombo(0)
    , m_statusLabel(0)
    , m_qssEditor(0)
    , m_previewWidget(0)
    , m_applyTimer(0)
{
    ui.setupUi(this);

    m_view = ui.centralView;
    m_scene = m_view->designScene();
    m_palette = ui.widgetsPalette;
    m_propertyPanel = ui.propertyPanel;
    m_qssEditor = ui.qssEditor;
    m_previewWidget = ui.previewWidget;
    m_styleCombo = ui.styleCombo;
    m_statusLabel = ui.statusLabel;

    m_styleManager = new StyleManager(appStylesDir());

    // 画布 / 控件面板信号
    connect(m_scene, &DesignScene::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(m_scene, &DesignScene::itemPropertyChanged, this, [this](DesignItem *) {
        updateStatusBar();
    });
    connect(m_scene, &DesignScene::sceneChanged, this, [this]() {
        updateStatusBar();
    });
    connect(m_palette, &WidgetPalette::widgetPicked, this, &MainWindow::addWidgetFromPalette);
    connect(m_view, &DesignView::zoomChanged, this, [this](double) {
        updateStatusBar();
    });

    // 样式下拉框：选择后把样式内容载入编辑器并应用
    connect(m_styleCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [this](int) {
                const QString name = m_styleCombo->currentData().toString();
                m_qssEditor->setPlainText(name.isEmpty() ? QString()
                                                        : m_styleManager->loadStyle(name));
                applyCurrentStyle();
            });

    // QSS 编辑器修改后防抖刷新
    m_applyTimer = new QTimer(this);
    m_applyTimer->setSingleShot(true);
    m_applyTimer->setInterval(400);
    connect(m_applyTimer, &QTimer::timeout, this, &MainWindow::applyCurrentStyle);
    connect(m_qssEditor, &QPlainTextEdit::textChanged, this, [this]() {
        m_applyTimer->start();
    });

    // 菜单 / 工具栏动作
    connect(ui.actionNew, &QAction::triggered, this, &MainWindow::newDesign);
    connect(ui.actionExportUi, &QAction::triggered, this, &MainWindow::exportUi);
    connect(ui.actionExportProject, &QAction::triggered, this, &MainWindow::exportProject);
    connect(ui.actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui.actionDelete, &QAction::triggered, m_scene, &DesignScene::deleteSelected);
    connect(ui.actionCopy, &QAction::triggered, m_scene, &DesignScene::copySelected);
    connect(ui.actionPaste, &QAction::triggered, m_scene, &DesignScene::paste);
    connect(ui.actionAlignLeft, &QAction::triggered, m_scene, &DesignScene::alignLeft);
    connect(ui.actionAlignRight, &QAction::triggered, m_scene, &DesignScene::alignRight);
    connect(ui.actionAlignTop, &QAction::triggered, m_scene, &DesignScene::alignTop);
    connect(ui.actionAlignBottom, &QAction::triggered, m_scene, &DesignScene::alignBottom);
    connect(ui.actionAlignHCenter, &QAction::triggered, m_scene, &DesignScene::alignHCenter);
    connect(ui.actionAlignVCenter, &QAction::triggered, m_scene, &DesignScene::alignVCenter);
    connect(ui.actionEqualWidth, &QAction::triggered, m_scene, &DesignScene::equalizeWidth);
    connect(ui.actionEqualHeight, &QAction::triggered, m_scene, &DesignScene::equalizeHeight);
    connect(ui.actionBringToFront, &QAction::triggered, m_scene, &DesignScene::bringToFront);
    connect(ui.actionSendToBack, &QAction::triggered, m_scene, &DesignScene::sendToBack);
    connect(ui.actionZoomIn, &QAction::triggered, m_view, [this]() { m_view->scale(1.2, 1.2); });
    connect(ui.actionZoomOut, &QAction::triggered, m_view, [this]() { m_view->scale(1.0 / 1.2, 1.0 / 1.2); });
    connect(ui.actionZoomReset, &QAction::triggered, m_view, [this]() {
        m_view->resetTransform();
        updateStatusBar();
    });
    connect(ui.actionSaveCustomStyle, &QAction::triggered, this, &MainWindow::saveCustomStyle);
    connect(ui.actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, QStringLiteral("关于 QssDesigner"),
                           QStringLiteral("<b>QssDesigner</b><br><br>"
                                          "可视化拖拽布局设计器 + 集成 QSS 样式系统。<br>"
                                          "兼容 Qt 5.6.3 (MSVC) 与 Qt 5.15.2。<br><br>"
                                          "左侧控件库拖拽控件到画布，选择样式实时预览，"
                                          "可导出 .ui / .qss / 完整 Qt C++ 工程。"));
    });

    populateStyleCombo();

    // 初始示例设计
    m_scene->addWidget(QStringLiteral("QPushButton"), QPointF(150, 90));
    m_scene->addWidget(QStringLiteral("QLineEdit"), QPointF(360, 90));
    m_scene->addWidget(QStringLiteral("QCheckBox"), QPointF(150, 170));
    m_scene->addWidget(QStringLiteral("QComboBox"), QPointF(360, 170));
    m_scene->addWidget(QStringLiteral("QProgressBar"), QPointF(255, 260));

    updateStatusBar();
}

void MainWindow::populateStyleCombo()
{
    m_styleCombo->clear();
    m_styleCombo->addItem(QStringLiteral("（无样式）"), QString());
    QMap<QString, QStringList> groups;
    Q_FOREACH (const StyleManager::Style &s, m_styleManager->styles())
        groups[s.group].append(s.name);
    QMapIterator<QString, QStringList> it(groups);
    while (it.hasNext()) {
        it.next();
        QStringList names = it.value();
        names.sort();
        Q_FOREACH (const QString &name, names)
            m_styleCombo->addItem(it.key() + QStringLiteral(" / ") + name, name);
    }

    // 默认选中 Aqua，否则选中第一个真实样式
    int defaultIndex = 1;
    const int aqua = m_styleCombo->findData(QStringLiteral("Aqua"));
    if (aqua >= 0)
        defaultIndex = aqua;
    if (m_styleCombo->count() > defaultIndex)
        m_styleCombo->setCurrentIndex(defaultIndex);
    else if (m_styleCombo->count() > 0)
        m_styleCombo->setCurrentIndex(0);
}

void MainWindow::onSelectionChanged()
{
    const QList<DesignItem *> sel = m_scene->selectedItems();
    m_propertyPanel->setItem(sel.isEmpty() ? 0 : sel.first());
    updateStatusBar();
}

void MainWindow::applyCurrentStyle()
{
    const QString name = m_styleCombo->currentData().toString();
    const QString qss = name.isEmpty() ? QString()
                                       : m_styleManager->prepareForPreview(name,m_qssEditor->toPlainText());
    // 全局应用：整个程序界面（菜单/工具栏/停靠/对话框/预览/画布）统一换肤
    qApp->setStyleSheet(qss);
    m_scene->applyStyleSheet(qss);
    m_previewWidget->applyStyleSheet(qss);
    updateStatusBar();
}

void MainWindow::saveCustomStyle()
{
    bool ok = false;
    const QString name = QInputDialog::getText(this, QStringLiteral("保存自定义样式"),
                                               QStringLiteral("样式名称："),
                                               QLineEdit::Normal, QString(), &ok);
    if (!ok || name.trimmed().isEmpty())
        return;

    QString error;
    if (!m_styleManager->saveCustomStyle(name, m_qssEditor->toPlainText(), &error)) {
        QMessageBox::warning(this, QStringLiteral("保存样式"), error);
        return;
    }
    populateStyleCombo();
    const QString full = name.trimmed();
    const int idx = m_styleCombo->findData(full.endsWith(QLatin1String(".qss"))
                                               ? full.left(full.length() - 4)
                                               : full);
    if (idx >= 0)
        m_styleCombo->setCurrentIndex(idx);
    statusBar()->showMessage(QStringLiteral("自定义样式已保存。"), 3000);
}

void MainWindow::exportUi()
{
    const QString file = QFileDialog::getSaveFileName(this, QStringLiteral("导出 .ui 文件"),
                                                      m_exportPath + QStringLiteral("/MyProject.ui"),
                                                      QStringLiteral("Qt Designer 界面文件 (*.ui)"));
    if (file.isEmpty())
        return;
    m_exportPath = QFileInfo(file).absolutePath();
    QString error;
    if (!UiWriter::writeUi(file, m_scene->allItems(), QStringLiteral("我的窗体"), &error))
    {
        QMessageBox::warning(this, QStringLiteral("导出 UI"), error);
        return;
    }
    statusBar()->showMessage(QStringLiteral("UI 已导出：%1").arg(file), 5000);
    QMessageBox::information(this, QStringLiteral("导出 UI"),
                             QStringLiteral("UI 导出成功。\n%1").arg(file));
}

void MainWindow::exportProject()
{
    const QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("导出 Qt 工程"),
                                                          m_exportPath);
    if (dir.isEmpty())
        return;
    m_exportPath = dir;
    QString error;
    if (!ProjectWriter::writeProject(dir, m_scene->allItems(),
                                     m_qssEditor->toPlainText(),
                                     QStringLiteral("我的窗体"),
                                     m_styleManager->stylesDir(),
                                     &error))
    {
        QMessageBox::warning(this, QStringLiteral("导出工程"), error);
        return;
    }
    statusBar()->showMessage(QStringLiteral("工程已导出：%1").arg(dir), 5000);
    QMessageBox::information(this, QStringLiteral("导出工程"),
                             QStringLiteral("Qt 工程导出成功。\n%1\n\n"
                                            "构建命令：\n"
                                            "  qmake MyProject.pro && make (或 nmake)").arg(dir));
}

void MainWindow::addWidgetFromPalette(const QString &typeName, const QVariant &variant)
{
    if (m_scene)
        m_scene->addWidget(typeName, m_view->mapToScene(m_view->viewport()->rect().center()),
                           variant);
}

void MainWindow::newDesign()
{
    if (!m_scene->allItems().isEmpty())
    {
        const QMessageBox::StandardButton ret =
            QMessageBox::question(this, QStringLiteral("新建设计"),
                                  QStringLiteral("确定清空当前设计吗？"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret != QMessageBox::Yes)
            return;
    }
    m_scene->clearAll();
    m_qssEditor->clear();
    statusBar()->showMessage(QStringLiteral("已创建新设计。"), 3000);
}

void MainWindow::updateStatusBar()
{
    const int count = m_scene ? m_scene->allItems().size() : 0;
    const int sel = m_scene ? m_scene->selectedItems().size() : 0;
    QString text = QStringLiteral("控件数量：%1    已选中：%2").arg(count).arg(sel);
    if (m_scene && sel == 1)
    {
        DesignItem *item = m_scene->selectedItems().first();
        const QRectF g = item->itemGeometry();
        text += QStringLiteral("    %1 (%2, %3) %4 x %5")
                    .arg(item->typeName())
                    .arg((int)g.x()).arg((int)g.y())
                    .arg((int)g.width()).arg((int)g.height());
    }
    m_statusLabel->setText(text);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

bool MainWindow::exportToDir(const QString &dir, QString *error)
{
    if (!ProjectWriter::writeProject(dir, m_scene->allItems(),
                                     m_qssEditor->toPlainText(),
                                     QStringLiteral("我的窗体"),
                                     m_styleManager->stylesDir(),
                                     error))
        return false;
    return true;
}

void MainWindow::addAllWidgetTypes()
{
    // 逐个生成控件库中的全部控件，验证每个都能放入画布并导出
    const QList<WidgetPalette::PaletteItem> items = WidgetPalette::paletteItems();
    const int perRow = 6;
    int i = 0;
    Q_FOREACH (const WidgetPalette::PaletteItem &it, items)
    {
        const int row = i / perRow;
        const int col = i % perRow;
        m_scene->addWidget(it.type, QPointF(100 + col * 190, 80 + row * 110),
                           QVariant(it.variant));
        ++i;
    }
}

int MainWindow::styledItemCount() const
{
    int count = 0;
    if (!m_scene)
        return 0;
    Q_FOREACH (DesignItem *item, m_scene->allItems())
    {
        if (item->widget() && !item->widget()->styleSheet().isEmpty())
            ++count;
    }
    return count;
}

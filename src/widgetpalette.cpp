#pragma execution_character_set("utf-8")

#include "widgetpalette.h"

#include "widgetdrag.h"

#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>

QList<WidgetPalette::PaletteItem> WidgetPalette::paletteItems()
{
    QList<PaletteItem> items;
#define ADD(g, t, n, v, i) \
    do { PaletteItem it; it.group = QStringLiteral(g); it.type = QStringLiteral(t); \
         it.name = QStringLiteral(n); it.variant = (v); it.icon = (i); items.append(it); } while (0)

    // ---- 按钮 ----
    ADD("按钮", "QPushButton", "按钮", 0, QStyle::SP_DialogOkButton);
    ADD("按钮", "QToolButton", "工具按钮", 0, QStyle::SP_DialogApplyButton);
    ADD("按钮", "QRadioButton", "单选按钮", 0, QStyle::SP_DialogYesButton);
    ADD("按钮", "QCheckBox", "复选框", 0, QStyle::SP_DialogNoButton);
    ADD("按钮", "QCommandLinkButton", "命令链接按钮", 0, QStyle::SP_ArrowForward);
    ADD("按钮", "QDialogButtonBox", "对话框按钮组", 0, QStyle::SP_DialogResetButton);

    // ---- 输入控件 ----
    ADD("输入控件", "QComboBox", "下拉框", 0, QStyle::SP_ArrowDown);
    ADD("输入控件", "QFontComboBox", "字体下拉框", 0, QStyle::SP_FileDialogDetailedView);
    ADD("输入控件", "QLineEdit", "行编辑", 0, QStyle::SP_LineEditClearButton);
    ADD("输入控件", "QTextEdit", "文本编辑", 0, QStyle::SP_FileDialogContentsView);
    ADD("输入控件", "QPlainTextEdit", "纯文本编辑", 0, QStyle::SP_FileDialogContentsView);
    ADD("输入控件", "QTextBrowser", "文本浏览器", 0, QStyle::SP_FileDialogInfoView);
    ADD("输入控件", "QSpinBox", "数字输入框", 0, QStyle::SP_MediaVolume);
    ADD("输入控件", "QDoubleSpinBox", "双精度输入框", 0, QStyle::SP_MediaVolume);
    ADD("输入控件", "QTimeEdit", "时间编辑", 0, QStyle::SP_MediaSeekBackward);
    ADD("输入控件", "QDateEdit", "日期编辑", 0, QStyle::SP_MediaSeekBackward);
    ADD("输入控件", "QDateTimeEdit", "日期时间编辑", 0, QStyle::SP_MediaSeekForward);
    ADD("输入控件", "QKeySequenceEdit", "快捷键编辑", 0, QStyle::SP_FileDialogNewFolder);

    // ---- 滑块 / 进度 ----
    ADD("滑块/进度", "QSlider", "滑动条", 0, QStyle::SP_MediaVolume);
    ADD("滑块/进度", "QScrollBar", "滚动条", 0, QStyle::SP_ArrowUp);
    ADD("滑块/进度", "QDial", "旋钮", 0, QStyle::SP_MediaPlay);
    ADD("滑块/进度", "QProgressBar", "进度条", 0, QStyle::SP_MediaPlay);

    // ---- 列表 / 表格 / 树 ----
    ADD("列表/表格/树", "QListWidget", "列表控件", 0, QStyle::SP_FileDialogListView);
    ADD("列表/表格/树", "QListWidget", "图标模式列表", 1, QStyle::SP_FileDialogListView);
    ADD("列表/表格/树", "QListView", "列表视图", 0, QStyle::SP_FileDialogListView);
    ADD("列表/表格/树", "QTableWidget", "表格控件", 0, QStyle::SP_FileDialogDetailedView);
    ADD("列表/表格/树", "QTableView", "表格视图", 0, QStyle::SP_FileDialogDetailedView);
    ADD("列表/表格/树", "QTreeWidget", "树控件", 0, QStyle::SP_FileDialogContentsView);
    ADD("列表/表格/树", "QTreeView", "树视图", 0, QStyle::SP_FileDialogContentsView);
    ADD("列表/表格/树", "QTreeView", "文件系统树", 1, QStyle::SP_DirIcon);
    ADD("列表/表格/树", "QColumnView", "列视图", 0, QStyle::SP_FileDialogContentsView);

    // ---- 容器 ----
    ADD("容器", "QGroupBox", "分组框", 0, QStyle::SP_DirOpenIcon);
    ADD("容器", "QTabWidget", "标签页", 0, QStyle::SP_FileDialogToParent);
    ADD("容器", "QToolBox", "工具箱", 0, QStyle::SP_FileDialogContentsView);
    ADD("容器", "QStackedWidget", "堆叠窗口", 0, QStyle::SP_FileDialogContentsView);
    ADD("容器", "QScrollArea", "滚动区域", 0, QStyle::SP_FileDialogToParent);
    ADD("容器", "QFrame", "框架", 0, QStyle::SP_FileDialogInfoView);
    ADD("容器", "QFrame", "水平线", 4, QStyle::SP_LineEditClearButton);
    ADD("容器", "QFrame", "垂直线", 5, QStyle::SP_LineEditClearButton);
    ADD("容器", "QWidget", "控件容器", 0, QStyle::SP_DesktopIcon);
    ADD("容器", "QMainWindow", "完整主窗口", 0, QStyle::SP_TitleBarNormalButton);
    ADD("容器", "QMdiArea", "MDI 区域", 0, QStyle::SP_DesktopIcon);
    ADD("容器", "QDockWidget", "停靠窗口", 0, QStyle::SP_TitleBarNormalButton);
    ADD("容器", "QSplitter", "分割器", 0, QStyle::SP_TitleBarNormalButton);

    // ---- 显示控件 ----
    ADD("显示控件", "QLabel", "标签", 0, QStyle::SP_MessageBoxInformation);
    ADD("显示控件", "QLabel", "图片标签", 1, QStyle::SP_ComputerIcon);
    ADD("显示控件", "QCalendarWidget", "日历", 0, QStyle::SP_ComputerIcon);
    ADD("显示控件", "QLCDNumber", "LCD 数字", 0, QStyle::SP_FileDialogInfoView);
    ADD("显示控件", "QGraphicsView", "图形视图", 0, QStyle::SP_DesktopIcon);

#undef ADD
    return items;
}

WidgetPalette::WidgetPalette(QWidget *parent)
    : QTreeWidget(parent)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
    setDefaultDropAction(Qt::CopyAction);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setIconSize(QSize(20, 20));
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setHeaderHidden(true);

    const QList<PaletteItem> items = paletteItems();
    QMap<QString, QTreeWidgetItem *> groups;
    Q_FOREACH (const PaletteItem &it, items) {
        QTreeWidgetItem *groupItem = groups.value(it.group);
        if (!groupItem) {
            groupItem = new QTreeWidgetItem(this, QStringList(it.group));
            QFont f = groupItem->font(0);
            f.setBold(true);
            groupItem->setFont(0, f);
            groupItem->setFlags(Qt::ItemIsEnabled);
            groups.insert(it.group, groupItem);
        }
        QTreeWidgetItem *leaf = new QTreeWidgetItem(groupItem, QStringList(it.name));
        leaf->setData(0, Qt::UserRole, it.type);
        leaf->setData(0, Qt::UserRole + 1, it.variant);
        leaf->setIcon(0, style()->standardIcon((QStyle::StandardPixmap)it.icon));
        leaf->setSizeHint(0, QSize(0, 28));
    }
    expandAll();
}

void WidgetPalette::startDrag(Qt::DropActions supportedActions)
{
    QTreeWidgetItem *item = currentItem();
    if (!item || !item->parent()) // 分组标题不可拖拽
        return;

    const QString type = item->data(0, Qt::UserRole).toString();
    const QVariant variant = item->data(0, Qt::UserRole + 1);
    QMimeData *mime = new QMimeData;
    QList<WidgetDragFilter::DragSource> sources;
    sources << WidgetDragFilter::DragSource(type, variant);
    mime->setData(QLatin1String(WidgetDragFilter::mimeType()),
                  WidgetDragFilter::mimePayload(sources));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mime);

    QPixmap pix(64, 40);
    pix.fill(QColor(230, 240, 255));
    QPainter p(&pix);
    p.setPen(QPen(QColor(0, 120, 215), 1));
    p.drawRect(QRect(1, 1, 62, 38));
    p.setPen(QColor(40, 40, 40));
    p.drawText(pix.rect(), Qt::AlignCenter, item->text(0));
    p.end();
    drag->setPixmap(pix);
    drag->setHotSpot(QPoint(32, 20));
    drag->exec(supportedActions, Qt::CopyAction);
}

void WidgetPalette::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeWidgetItem *item = itemAt(event->pos());
    if (item && item->parent()) {
        emit widgetPicked(item->data(0, Qt::UserRole).toString(),
                          item->data(0, Qt::UserRole + 1));
    }
    QTreeWidget::mouseDoubleClickEvent(event);
}

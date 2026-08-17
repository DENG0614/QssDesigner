#ifndef WIDGETPALETTE_H
#pragma execution_character_set("utf-8")
#define WIDGETPALETTE_H

#include <QList>
#include <QString>
#include <QTreeWidget>
#include <QVariant>

// 左侧控件库：分组树形式列出全部可拖拽控件（覆盖 QssStylesheetEditor v1.8
// 预览面板中出现过的所有控件类型）。每个叶子节点均可拖拽/双击放到画布。
class WidgetPalette : public QTreeWidget
{
    Q_OBJECT

public:
    struct PaletteItem {
        QString type;   // Qt 类名（同时用于 .ui 导出）
        QString name;   // 中文显示名
        int variant;    // 变体（如图标模式列表、文件系统树、图片标签）
        int icon;       // QStyle::StandardPixmap
        QString group;  // 分组名
    };

    explicit WidgetPalette(QWidget *parent = 0);

    static QList<PaletteItem> paletteItems();

signals:
    void widgetPicked(const QString &typeName, const QVariant &variant);

protected:
    void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // WIDGETPALETTE_H

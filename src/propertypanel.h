#ifndef PROPERTYPANEL_H
#pragma execution_character_set("utf-8")
#define PROPERTYPANEL_H

#include <QTreeWidget>

class DesignItem;

// Right side property panel. Shows editable properties of the currently
// selected item; edits are applied to the widget immediately.
class PropertyPanel : public QTreeWidget
{
    Q_OBJECT

public:
    explicit PropertyPanel(QWidget *parent = 0);

    void setItem(DesignItem *item);

private slots:
    void onItemChanged(QTreeWidgetItem *item, int column);

private:
    void addProperty(const QString &key, const QVariant &value, const QString &editorType);
    void reload();

    DesignItem *m_item;
};

#endif // PROPERTYPANEL_H

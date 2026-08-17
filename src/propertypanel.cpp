#pragma execution_character_set("utf-8")

#include "propertypanel.h"

#include "designitem.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QSpinBox>

namespace {

QString displayName(const QString &key)
{
    if (key == QLatin1String("objectName")) return QStringLiteral("对象名");
    if (key == QLatin1String("x")) return QStringLiteral("X 坐标");
    if (key == QLatin1String("y")) return QStringLiteral("Y 坐标");
    if (key == QLatin1String("width")) return QStringLiteral("宽度");
    if (key == QLatin1String("height")) return QStringLiteral("高度");
    if (key == QLatin1String("enabled")) return QStringLiteral("启用");
    if (key == QLatin1String("visible")) return QStringLiteral("可见");
    if (key == QLatin1String("checked")) return QStringLiteral("选中");
    if (key == QLatin1String("checkable")) return QStringLiteral("可勾选");
    if (key == QLatin1String("readOnly")) return QStringLiteral("只读");
    if (key == QLatin1String("editable")) return QStringLiteral("可编辑");
    if (key == QLatin1String("text")) return QStringLiteral("文本");
    if (key == QLatin1String("title")) return QStringLiteral("标题");
    if (key == QLatin1String("toolTip")) return QStringLiteral("提示");
    if (key == QLatin1String("placeholderText")) return QStringLiteral("占位文本");
    if (key == QLatin1String("plainText")) return QStringLiteral("内容");
    if (key == QLatin1String("value")) return QStringLiteral("数值");
    if (key == QLatin1String("minimum")) return QStringLiteral("最小值");
    if (key == QLatin1String("maximum")) return QStringLiteral("最大值");
    if (key == QLatin1String("currentIndex")) return QStringLiteral("当前索引");
    if (key == QLatin1String("orientation")) return QStringLiteral("方向");
    if (key == QLatin1String("frameShape")) return QStringLiteral("框架形状");
    return key;
}

} // namespace

PropertyPanel::PropertyPanel(QWidget *parent)
    : QTreeWidget(parent)
    , m_item(0)
{
    setColumnCount(2);
    QStringList headers;
    headers << QStringLiteral("属性") << QStringLiteral("值");
    setHeaderLabels(headers);
    setRootIsDecorated(false);
    setAlternatingRowColors(true);
    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(1, QHeaderView::Stretch);
    connect(this, &QTreeWidget::itemChanged, this, &PropertyPanel::onItemChanged);
}

void PropertyPanel::setItem(DesignItem *item)
{
    m_item = item;
    reload();
}

void PropertyPanel::reload()
{
    clear();
    if (!m_item)
        return;

    const QMap<QString, QVariant> props = m_item->exportProperties();
    const QRectF g = m_item->itemGeometry();

    // object name + geometry
    QTreeWidgetItem *nameItem = new QTreeWidgetItem(this);
    nameItem->setText(0, QStringLiteral("对象名"));
    nameItem->setFlags(nameItem->flags() | Qt::ItemIsEditable);
    nameItem->setData(1, Qt::UserRole, QLatin1String("objectName"));
    nameItem->setText(1, props.value(QLatin1String("objectName")).toString());

    addProperty(QLatin1String("x"), (int)g.x(), QLatin1String("spin"));
    addProperty(QLatin1String("y"), (int)g.y(), QLatin1String("spin"));
    addProperty(QLatin1String("width"), (int)g.width(), QLatin1String("spin"));
    addProperty(QLatin1String("height"), (int)g.height(), QLatin1String("spin"));

    QMapIterator<QString, QVariant> it(props);
    while (it.hasNext()) {
        it.next();
        const QString key = it.key();
        if (key == QLatin1String("objectName") || key == QLatin1String("geometry")
            || key == QLatin1String("items") || key == QLatin1String("rowCount")
            || key == QLatin1String("columnCount") || key == QLatin1String("echoMode"))
            continue;

        if (key == QLatin1String("enabled") || key == QLatin1String("visible")
            || key == QLatin1String("checked") || key == QLatin1String("checkable")
            || key == QLatin1String("readOnly") || key == QLatin1String("editable")) {
            addProperty(key, it.value(), QLatin1String("bool"));
        } else if (key == QLatin1String("minimum") || key == QLatin1String("maximum")
                   || key == QLatin1String("value") || key == QLatin1String("currentIndex")) {
            addProperty(key, it.value(), QLatin1String("spin"));
        } else if (key == QLatin1String("decimals")) {
            continue;
        } else if (key == QLatin1String("orientation")) {
            addProperty(key, it.value().toInt() == 1 ? QStringLiteral("水平") : QStringLiteral("垂直"),
                        QLatin1String("orient"));
        } else {
            addProperty(key, it.value(), QLatin1String("text"));
        }
    }

    expandAll();
}

void PropertyPanel::addProperty(const QString &key, const QVariant &value, const QString &editorType)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(this);
    item->setText(0, displayName(key));
    item->setData(1, Qt::UserRole, key);

    if (editorType == QLatin1String("spin")) {
        QSpinBox *spin = new QSpinBox(this);
        spin->setRange(-100000, 100000);
        spin->setValue(value.toInt());
        spin->setKeyboardTracking(false);
        setItemWidget(item, 1, spin);
        connect(spin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                this, [this, key](int v) {
                    if (m_item)
                        m_item->applyProperty(key, v);
                });
    } else if (editorType == QLatin1String("bool")) {
        QComboBox *combo = new QComboBox(this);
        combo->addItem(QStringLiteral("是"));
        combo->addItem(QStringLiteral("否"));
        combo->setCurrentIndex(value.toBool() ? 0 : 1);
        setItemWidget(item, 1, combo);
        connect(combo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, [this, key](int idx) {
                    if (m_item)
                        m_item->applyProperty(key, idx == 0);
                });
    } else if (editorType == QLatin1String("orient")) {
        QComboBox *combo = new QComboBox(this);
        combo->addItem(QStringLiteral("水平"));
        combo->addItem(QStringLiteral("垂直"));
        combo->setCurrentIndex(value.toString().contains(QLatin1String("Vertical")) ? 1 : 0);
        setItemWidget(item, 1, combo);
        connect(combo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                this, [this, key](int idx) {
                    if (m_item)
                        m_item->applyProperty(key, idx == 0 ? 1 : 2);
                });
    } else {
        QLineEdit *edit = new QLineEdit(value.toString(), this);
        setItemWidget(item, 1, edit);
        connect(edit, &QLineEdit::textEdited, this, [this, key](const QString &text) {
            if (m_item)
                m_item->applyProperty(key, text);
        });
    }
}

void PropertyPanel::onItemChanged(QTreeWidgetItem *item, int column)
{
    if (!m_item || column != 1 || !item)
        return;
    const QString key = item->data(1, Qt::UserRole).toString();
    if (key.isEmpty() || key == QLatin1String("objectName")) {
        if (key == QLatin1String("objectName"))
            m_item->setObjectName(item->text(1));
        return;
    }
    // text-type properties edited directly in the tree
    if (key == QLatin1String("text") || key == QLatin1String("title")
        || key == QLatin1String("plainText") || key == QLatin1String("placeholderText")
        || key == QLatin1String("toolTip"))
        m_item->applyProperty(key, item->text(1));
}

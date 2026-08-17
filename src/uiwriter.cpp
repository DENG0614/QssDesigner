#pragma execution_character_set("utf-8")

#include "uiwriter.h"

#include "designitem.h"

#include <QFile>
#include <QXmlStreamWriter>

namespace {

void writeRect(QXmlStreamWriter &out, const QRectF &r)
{
    out.writeStartElement(QStringLiteral("rect"));
    out.writeTextElement(QStringLiteral("x"), QString::number((int)r.x()));
    out.writeTextElement(QStringLiteral("y"), QString::number((int)r.y()));
    out.writeTextElement(QStringLiteral("width"), QString::number((int)r.width()));
    out.writeTextElement(QStringLiteral("height"), QString::number((int)r.height()));
    out.writeEndElement(); // rect
}

void writePropertyRect(QXmlStreamWriter &out, const QString &name, const QRectF &r)
{
    out.writeStartElement(QStringLiteral("property"));
    out.writeAttribute(QStringLiteral("name"), name);
    writeRect(out, r);
    out.writeEndElement();
}

void writePropertyString(QXmlStreamWriter &out, const QString &name, const QString &value)
{
    out.writeStartElement(QStringLiteral("property"));
    out.writeAttribute(QStringLiteral("name"), name);
    out.writeTextElement(QStringLiteral("string"), value);
    out.writeEndElement();
}

void writePropertyBool(QXmlStreamWriter &out, const QString &name, bool value)
{
    out.writeStartElement(QStringLiteral("property"));
    out.writeAttribute(QStringLiteral("name"), name);
    out.writeTextElement(QStringLiteral("bool"), value ? QStringLiteral("true") : QStringLiteral("false"));
    out.writeEndElement();
}

void writePropertyNumber(QXmlStreamWriter &out, const QString &name, const QVariant &value)
{
    out.writeStartElement(QStringLiteral("property"));
    out.writeAttribute(QStringLiteral("name"), name);
    out.writeTextElement(QStringLiteral("number"), value.toString());
    out.writeEndElement();
}

void writePropertyEnum(QXmlStreamWriter &out, const QString &name, const QString &value)
{
    out.writeStartElement(QStringLiteral("property"));
    out.writeAttribute(QStringLiteral("name"), name);
    out.writeTextElement(QStringLiteral("enum"), value);
    out.writeEndElement();
}

void writeItems(QXmlStreamWriter &out, const QStringList &items)
{
    Q_FOREACH (const QString &text, items) {
        out.writeStartElement(QStringLiteral("item"));
        writePropertyString(out, QStringLiteral("text"), text);
        out.writeEndElement();
    }
}

void writeWidgetElement(QXmlStreamWriter &out, DesignItem *item, const QPointF &origin)
{
    const QMap<QString, QVariant> props = item->exportProperties();
    const QRectF g = item->itemGeometry().translated(-origin);

    out.writeStartElement(QStringLiteral("widget"));
    // QMainWindow 在自由画布中以普通容器形式导出，保证 Designer 可解析
    const QString cls = item->typeName() == QLatin1String("QMainWindow")
                            ? QStringLiteral("QWidget")
                            : item->typeName();
    out.writeAttribute(QStringLiteral("class"), cls);
    out.writeAttribute(QStringLiteral("name"), item->objectName());

    writePropertyRect(out, QStringLiteral("geometry"), g);

    // strings
    if (props.contains(QLatin1String("text")))
        writePropertyString(out, QStringLiteral("text"), props.value(QLatin1String("text")).toString());
    if (props.contains(QLatin1String("title")))
        writePropertyString(out, QStringLiteral("title"), props.value(QLatin1String("title")).toString());
    if (props.contains(QLatin1String("toolTip")))
        writePropertyString(out, QStringLiteral("toolTip"), props.value(QLatin1String("toolTip")).toString());
    if (props.contains(QLatin1String("placeholderText")))
        writePropertyString(out, QStringLiteral("placeholderText"),
                            props.value(QLatin1String("placeholderText")).toString());
    if (props.contains(QLatin1String("plainText")))
        writePropertyString(out, QStringLiteral("plainText"), props.value(QLatin1String("plainText")).toString());

    // booleans
    if (props.contains(QLatin1String("enabled")))
        writePropertyBool(out, QStringLiteral("enabled"), props.value(QLatin1String("enabled")).toBool());
    if (props.contains(QLatin1String("visible")))
        writePropertyBool(out, QStringLiteral("visible"), props.value(QLatin1String("visible")).toBool());
    if (props.contains(QLatin1String("checkable")))
        writePropertyBool(out, QStringLiteral("checkable"), props.value(QLatin1String("checkable")).toBool());
    if (props.contains(QLatin1String("checked")))
        writePropertyBool(out, QStringLiteral("checked"), props.value(QLatin1String("checked")).toBool());
    if (props.contains(QLatin1String("readOnly")))
        writePropertyBool(out, QStringLiteral("readOnly"), props.value(QLatin1String("readOnly")).toBool());
    if (props.contains(QLatin1String("editable")))
        writePropertyBool(out, QStringLiteral("editable"), props.value(QLatin1String("editable")).toBool());

    // numbers
    if (props.contains(QLatin1String("minimum")))
        writePropertyNumber(out, QStringLiteral("minimum"), props.value(QLatin1String("minimum")));
    if (props.contains(QLatin1String("maximum")))
        writePropertyNumber(out, QStringLiteral("maximum"), props.value(QLatin1String("maximum")));
    if (props.contains(QLatin1String("value")))
        writePropertyNumber(out, QStringLiteral("value"), props.value(QLatin1String("value")));
    if (props.contains(QLatin1String("currentIndex")))
        writePropertyNumber(out, QStringLiteral("currentIndex"), props.value(QLatin1String("currentIndex")));

    // enums
    if (props.contains(QLatin1String("orientation"))) {
        writePropertyEnum(out, QStringLiteral("orientation"),
                          props.value(QLatin1String("orientation")).toInt() == 1
                              ? QStringLiteral("Qt::Horizontal")
                              : QStringLiteral("Qt::Vertical"));
    }
    if (props.contains(QLatin1String("frameShape"))) {
        writePropertyEnum(out, QStringLiteral("frameShape"),
                          QStringLiteral("QFrame::") + props.value(QLatin1String("frameShape")).toString());
    }

    // item lists
    if (props.contains(QLatin1String("items")))
        writeItems(out, props.value(QLatin1String("items")).toStringList());

    out.writeEndElement(); // widget
}

bool isContainerType(const QString &type)
{
    return type == QLatin1String("QGroupBox");
}

bool rectContains(const QRectF &outer, const QRectF &inner)
{
    return outer.adjusted(2, 2, -2, -2).contains(inner);
}

// 递归写出控件及其（位于容器内的）子控件，容器只写一次，子控件坐标相对容器
void writeItemTree(QXmlStreamWriter &out, DesignItem *item,
                   const QPointF &origin,
                   const QList<DesignItem *> &containers,
                   const QList<DesignItem *> &allItems)
{
    writeWidgetElement(out, item, origin);
    if (!isContainerType(item->typeName()))
        return;

    const QPointF childOrigin = item->itemGeometry().topLeft();
    Q_FOREACH (DesignItem *child, allItems) {
        if (child == item)
            continue;
        if (!rectContains(item->itemGeometry(), child->itemGeometry()))
            continue;
        bool inDeeper = false;
        Q_FOREACH (DesignItem *other, containers) {
            if (other != item && rectContains(other->itemGeometry(), child->itemGeometry())) {
                inDeeper = true;
                break;
            }
        }
        if (inDeeper)
            continue;
        writeItemTree(out, child, childOrigin, containers, allItems);
    }
}

} // namespace

bool UiWriter::writeUi(const QString &filePath,
                       const QList<DesignItem *> &items,
                       const QString &windowTitle,
                       QString *error)
{
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (error)
            *error = QStringLiteral("无法打开文件写入：") + filePath;
        return false;
    }

    QXmlStreamWriter out(&f);
    out.setAutoFormatting(true);
    out.setAutoFormattingIndent(1);
    out.writeStartDocument(QStringLiteral("1.0"));
    out.writeStartElement(QStringLiteral("ui"));
    out.writeAttribute(QStringLiteral("version"), QStringLiteral("4.0"));

    out.writeTextElement(QStringLiteral("class"), QStringLiteral("MainWindow"));

    out.writeStartElement(QStringLiteral("widget"));
    out.writeAttribute(QStringLiteral("class"), QStringLiteral("QMainWindow"));
    out.writeAttribute(QStringLiteral("name"), QStringLiteral("MainWindow"));
    writePropertyRect(out, QStringLiteral("geometry"), QRectF(0, 0, 800, 600));
    writePropertyString(out, QStringLiteral("windowTitle"), windowTitle);

    out.writeStartElement(QStringLiteral("widget"));
    out.writeAttribute(QStringLiteral("class"), QStringLiteral("QWidget"));
    out.writeAttribute(QStringLiteral("name"), QStringLiteral("centralwidget"));

    // group containers
    QList<DesignItem *> containers;
    Q_FOREACH (DesignItem *item, items) {
        if (isContainerType(item->typeName()))
            containers.append(item);
    }

    Q_FOREACH (DesignItem *item, items) {
        bool inside = false;
        Q_FOREACH (DesignItem *container, containers) {
            if (container != item && rectContains(container->itemGeometry(), item->itemGeometry())) {
                inside = true;
                break;
            }
        }
        if (inside)
            continue;
        writeItemTree(out, item, QPointF(0, 0), containers, items);
    }

    out.writeEndElement(); // centralwidget
    out.writeEndElement(); // MainWindow widget

    out.writeEmptyElement(QStringLiteral("resources"));
    out.writeEmptyElement(QStringLiteral("connections"));
    out.writeEndElement(); // ui
    out.writeEndDocument();
    f.close();
    return true;
}

#pragma execution_character_set("utf-8")

#include "widgetdrag.h"

#include "designitem.h"

#include <QApplication>
#include <QDrag>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMimeData>
#include <QMouseEvent>
#include <QWidget>

namespace {

void installRecursive(QWidget *w, QObject *filter)
{
    w->installEventFilter(filter);
    Q_FOREACH (QWidget *child, w->findChildren<QWidget *>())
        child->installEventFilter(filter);
}

QJsonObject propsToJson(const QMap<QString, QVariant> &props)
{
    QJsonObject obj;
    QMapIterator<QString, QVariant> it(props);
    while (it.hasNext()) {
        it.next();
        const QVariant &v = it.value();
        switch (v.type()) {
        case QVariant::Bool:
            obj.insert(it.key(), QJsonValue(v.toBool()));
            break;
        case QVariant::Int:
            obj.insert(it.key(), QJsonValue(v.toInt()));
            break;
        case QVariant::Double:
            obj.insert(it.key(), QJsonValue(v.toDouble()));
            break;
        case QVariant::StringList: {
            QJsonArray arr;
            Q_FOREACH (const QString &s, v.toStringList())
                arr.append(QJsonValue(s));
            obj.insert(it.key(), arr);
            break;
        }
        default:
            obj.insert(it.key(), QJsonValue(v.toString()));
            break;
        }
    }
    return obj;
}

} // namespace

WidgetDragFilter::WidgetDragFilter(QWidget *target, const QList<DragSource> &sources,
                                   QWidget *grabSource)
    : QObject(target)
    , m_sources(sources)
    , m_grabSource(grabSource)
    , m_pressed(false)
{
    if (target) {
        // 递归安装：复合控件（表格/树/标签页/MDI/日历等）内部子控件也可发起拖拽
        installRecursive(target, this);
        target->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
}

WidgetDragFilter::WidgetDragFilter(QWidget *target, const QString &type, const QVariant &variant,
                                   QWidget *grabSource)
    : QObject(target)
    , m_grabSource(grabSource)
    , m_pressed(false)
{
    m_sources.append(DragSource(type, variant, target));
    if (target) {
        installRecursive(target, this);
        target->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
}

const char *WidgetDragFilter::mimeType()
{
    return "application/x-qssdesigner-widget";
}

QByteArray WidgetDragFilter::mimePayload(const QList<DragSource> &sources)
{
    QJsonArray arr;
    Q_FOREACH (const DragSource &s, sources) {
        QJsonObject obj;
        obj.insert(QStringLiteral("type"), s.type);
        obj.insert(QStringLiteral("variant"), s.variant.toInt());
        QJsonObject props;
        if (s.source)
            props = propsToJson(collectWidgetProperties(s.source));
        if (!s.dialogAction.isEmpty())
            props.insert(QStringLiteral("dialogAction"), s.dialogAction);
        obj.insert(QStringLiteral("props"), props);
        arr.append(obj);
    }
    return QJsonDocument(arr).toJson(QJsonDocument::Compact);
}

bool WidgetDragFilter::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *widget = qobject_cast<QWidget *>(obj);
    if (!widget)
        return QObject::eventFilter(obj, event);

    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        // 仅 Ctrl+按住 才拦截并进入拖拽；普通点击/拖动完全交给控件原生行为，
        // 避免与文本选择、表格选择、滚动、按钮点击、调色标签等冲突。
        if (me->button() == Qt::LeftButton && (me->modifiers() & Qt::ControlModifier)) {
            m_pressed = true;
            m_pressPos = me->pos();
            return true;
        }
        break;
    }
    case QEvent::MouseButtonRelease:
        if (m_pressed) {
            m_pressed = false;
            return true;
        }
        break;
    case QEvent::MouseMove: {
        if (m_pressed) {
            QMouseEvent *me = static_cast<QMouseEvent *>(event);
            if ((me->modifiers() & Qt::ControlModifier)
                && (me->pos() - m_pressPos).manhattanLength() >= QApplication::startDragDistance()) {
                m_pressed = false;
                startDrag(me->globalPos());
                return true;
            }
        }
        break;
    }
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}

void WidgetDragFilter::startDrag(const QPoint &globalPos)
{
    QWidget *target = qobject_cast<QWidget *>(parent());
    QWidget *src = m_grabSource ? m_grabSource : target;
    if (!target || !src || m_sources.isEmpty())
        return;

    QMimeData *mime = new QMimeData;
    mime->setData(QLatin1String(mimeType()), mimePayload(m_sources));

    QDrag *drag = new QDrag(target);
    drag->setMimeData(mime);
    QPixmap pix = src->grab();
    if (pix.width() > 240 || pix.height() > 180)
        pix = pix.scaled(240, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    drag->setPixmap(pix);
    drag->setHotSpot(src->mapFromGlobal(globalPos));
    drag->exec(Qt::CopyAction);
}

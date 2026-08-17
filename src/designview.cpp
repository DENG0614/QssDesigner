#pragma execution_character_set("utf-8")

#include "designview.h"

#include "designitem.h"
#include "designscene.h"
#include "dialogs.h"
#include "widgetdrag.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QKeyEvent>
#include <QMimeData>
#include <QPushButton>
#include <QVariant>
#include <QWheelEvent>

DesignView::DesignView(QWidget *parent)
    : QGraphicsView(parent)
    , m_designScene(0)
    , m_zoomFactor(1.0)
{
    m_designScene = new DesignScene(this);
    setScene(m_designScene);
    setAcceptDrops(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing, false);
    setRenderHint(QPainter::TextAntialiasing, true);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setBackgroundBrush(QColor(0xF2, 0xF2, 0xF2));
    setFrameShape(QFrame::StyledPanel);
}

DesignScene *DesignView::designScene() const
{
    return m_designScene;
}

void DesignView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(QLatin1String(WidgetDragFilter::mimeType())))
        event->acceptProposedAction();
    else
        QGraphicsView::dragEnterEvent(event);
}

void DesignView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(QLatin1String(WidgetDragFilter::mimeType()))) {
        event->acceptProposedAction();
        return;
    }
    QGraphicsView::dragMoveEvent(event);
}

void DesignView::dropEvent(QDropEvent *event)
{
    if (dropWidget(event->mimeData(), event->pos())) {
        event->acceptProposedAction();
        return;
    }
    QGraphicsView::dropEvent(event);
}

bool DesignView::dropWidget(const QMimeData *mime, const QPoint &viewportPos)
{
    if (!mime || !mime->hasFormat(QLatin1String(WidgetDragFilter::mimeType())))
        return false;
    const QByteArray payload = mime->data(QLatin1String(WidgetDragFilter::mimeType()));
    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(payload, &err);
    if (err.error != QJsonParseError::NoError || !doc.isArray())
        return false;
    const QJsonArray arr = doc.array();
    if (arr.isEmpty())
        return false;

    QPointF pos = mapToScene(viewportPos);
    Q_FOREACH (const QJsonValue &val, arr) {
        const QJsonObject obj = val.toObject();
        const QString type = obj.value(QStringLiteral("type")).toString();
        const int variant = obj.value(QStringLiteral("variant")).toInt();
        DesignItem *item = m_designScene->addWidget(type, pos, QVariant(variant));

        const QJsonObject props = obj.value(QStringLiteral("props")).toObject();
        Q_FOREACH (const QString &key, props.keys()) {
            const QJsonValue v = props.value(key);
            if (key == QLatin1String("items")) {
                QStringList list;
                Q_FOREACH (const QJsonValue &iv, v.toArray())
                    list << iv.toString();
                item->applyProperty(key, list);
            } else {
                item->applyProperty(key, v.toVariant());
            }
        }

        // 拖出的对话框按钮：点击后打开对应标准对话框
        if (props.contains(QLatin1String("dialogAction"))) {
            const QString action = props.value(QLatin1String("dialogAction")).toString();
            if (QPushButton *btn = qobject_cast<QPushButton *>(item->widget())) {
                connect(btn, &QPushButton::clicked, btn, [btn, action]() {
                    showStandardDialog(btn, action);
                });
            }
        }

        // 组合控件（如 标签+输入框）依次排布在右侧
        const QRectF g = item->itemGeometry();
        pos = QPointF(g.right() + 10 + g.width() / 2, g.center().y());
    }
    return true;
}

void DesignView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        const double factor = event->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15;
        scale(factor, factor);
        m_zoomFactor *= factor;
        emit zoomChanged(m_zoomFactor);
        event->accept();
        return;
    }
    QGraphicsView::wheelEvent(event);
}

void DesignView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        m_designScene->deleteSelected();
        event->accept();
        return;
    }
    if (event->matches(QKeySequence::Copy)) {
        m_designScene->copySelected();
        event->accept();
        return;
    }
    if (event->matches(QKeySequence::Paste)) {
        m_designScene->paste();
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right
        || event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        const int dx = event->key() == Qt::Key_Left ? -1 : (event->key() == Qt::Key_Right ? 1 : 0);
        const int dy = event->key() == Qt::Key_Up ? -1 : (event->key() == Qt::Key_Down ? 1 : 0);
        Q_FOREACH (DesignItem *item, m_designScene->selectedItems()) {
            QRectF g = item->itemGeometry();
            g.translate(dx, dy);
            item->setItemGeometry(g);
        }
        event->accept();
        return;
    }
    QGraphicsView::keyPressEvent(event);
}

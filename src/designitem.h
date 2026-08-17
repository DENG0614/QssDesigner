#ifndef DESIGNITEM_H
#pragma execution_character_set("utf-8")
#define DESIGNITEM_H

#include <QGraphicsProxyWidget>
#include <QMap>
#include <QVariant>

class QWidget;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;

// 提取控件可序列化属性（拖拽到画布与 .ui 导出共用）。
// 不含 objectName / geometry，避免拖出时复制对象名。
QMap<QString, QVariant> collectWidgetProperties(QWidget *widget);

// A widget placed on the design canvas. Wraps a real QWidget inside a
// QGraphicsProxyWidget and draws resize handles while selected.
class DesignItem : public QGraphicsProxyWidget
{
    Q_OBJECT

public:
    enum { Type = UserType + 1 };

    enum Handle {
        NoHandle = 0,
        TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left
    };

    explicit DesignItem(const QString &typeName,
                        const QVariant &variant = QVariant(),
                        QGraphicsItem *parent = 0);

    QString typeName() const;
    QVariant variant() const;
    QString objectName() const;
    void setObjectName(const QString &name);

    QMap<QString, QVariant> exportProperties() const;
    void applyProperty(const QString &key, const QVariant &value);

    // 应用设计样式：当前 QSS + 控件自身局部样式（局部样式后置可覆盖）
    void setDesignStyleSheet(const QString &qss);

    int type() const Q_DECL_OVERRIDE { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;

    // scene coordinate helpers
    void setItemGeometry(const QRectF &rect);
    QRectF itemGeometry() const;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:
    void updateCursor(Handle handle);
    Handle handleAt(const QPointF &scenePos) const;
    QRectF resizeRect(const QRectF &original, Handle handle, const QPointF &delta) const;
    void ensureUniqueObjectName();
    void updateToolTip();
    class DesignScene *designScene() const;

    QString m_typeName;
    QVariant m_variant;
    QString m_localStyleSheet;
    Handle m_activeHandle;
    QRectF m_pressGeometry;
    QPointF m_pressPos;
    bool m_resizing;
};

#endif // DESIGNITEM_H

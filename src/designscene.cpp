#pragma execution_character_set("utf-8")

#include "designscene.h"

#include "designitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QSet>
#include <QWidget>

DesignScene::DesignScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(-2000, -2000, 4000, 4000);
}

QString DesignScene::nextObjectName(const QString &baseName) const
{
    QString clean = baseName;
    if (clean.startsWith(QLatin1String("Q")))
        clean = clean.mid(1);
    if (clean.isEmpty())
        clean = QStringLiteral("Widget");
    clean[0] = clean.at(0).toLower();

    QSet<QString> used;
    Q_FOREACH (DesignItem *item, allItems())
        used.insert(item->objectName());

    int index = 1;
    QString name = clean + QString::number(index);
    while (used.contains(name))
        name = clean + QString::number(++index);
    return name;
}

DesignItem *DesignScene::addWidget(const QString &typeName, const QPointF &scenePos,
                                   const QVariant &variant)
{
    DesignItem *item = new DesignItem(typeName, variant);
    item->setObjectName(nextObjectName(typeName));
    addItem(item);
    const QRectF r = item->boundingRect();
    item->setPos(scenePos - QPointF(r.width() / 2, r.height() / 2));
    item->setSelected(true);
    clearSelection();
    item->setSelected(true);
    // 立即应用当前样式，保证拖出/添加的控件与预览一致
    item->setDesignStyleSheet(m_currentQss);
    emit sceneChanged();
    emit itemPropertyChanged(item);
    return item;
}

QList<DesignItem *> DesignScene::selectedItems() const
{
    QList<DesignItem *> result;
    Q_FOREACH (QGraphicsItem *it, QGraphicsScene::selectedItems())
        if (DesignItem *di = qgraphicsitem_cast<DesignItem *>(it))
            result.append(di);
    return result;
}

QList<DesignItem *> DesignScene::allItems() const
{
    QList<DesignItem *> result;
    Q_FOREACH (QGraphicsItem *it, items())
        if (DesignItem *di = qgraphicsitem_cast<DesignItem *>(it))
            result.append(di);
    return result;
}

void DesignScene::deleteSelected()
{
    const QList<DesignItem *> sel = selectedItems();
    Q_FOREACH (DesignItem *item, sel) {
        removeItem(item);
        delete item;
    }
    if (!sel.isEmpty())
        emit sceneChanged();
}

void DesignScene::copySelected()
{
    m_clipboardTypes.clear();
    m_clipboardGeometries.clear();
    Q_FOREACH (DesignItem *item, selectedItems()) {
        m_clipboardTypes.append(item->typeName());
        m_clipboardGeometries.append(item->itemGeometry());
    }
}

void DesignScene::paste()
{
    if (m_clipboardTypes.isEmpty())
        return;
    clearSelection();
    for (int i = 0; i < m_clipboardTypes.size(); ++i) {
        DesignItem *item = new DesignItem(m_clipboardTypes.at(i));
        item->setObjectName(nextObjectName(m_clipboardTypes.at(i)));
        addItem(item);
        QRectF g = m_clipboardGeometries.at(i).translated(20, 20);
        item->setItemGeometry(g);
        item->setDesignStyleSheet(m_currentQss);
        item->setSelected(true);
    }
    emit sceneChanged();
}

void DesignScene::clearAll()
{
    const QList<DesignItem *> all = allItems();
    Q_FOREACH (DesignItem *item, all) {
        removeItem(item);
        delete item;
    }
    emit sceneChanged();
}

void DesignScene::alignLeft()
{
    const QList<DesignItem *> sel = selectedItems();
    if (sel.size() < 2)
        return;
    qreal left = 1e9;
    Q_FOREACH (DesignItem *it, sel)
        left = qMin(left, it->itemGeometry().left());
    Q_FOREACH (DesignItem *it, sel) {
        QRectF g = it->itemGeometry();
        g.moveLeft(left);
        it->setItemGeometry(g);
    }
}

void DesignScene::alignRight()
{
    const QList<DesignItem *> sel = selectedItems();
    if (sel.size() < 2)
        return;
    qreal right = -1e9;
    Q_FOREACH (DesignItem *it, sel)
        right = qMax(right, it->itemGeometry().right());
    Q_FOREACH (DesignItem *it, sel) {
        QRectF g = it->itemGeometry();
        g.moveRight(right);
        it->setItemGeometry(g);
    }
}

void DesignScene::alignTop()
{
    const QList<DesignItem *> sel = selectedItems();
    if (sel.size() < 2)
        return;
    qreal top = 1e9;
    Q_FOREACH (DesignItem *it, sel)
        top = qMin(top, it->itemGeometry().top());
    Q_FOREACH (DesignItem *it, sel) {
        QRectF g = it->itemGeometry();
        g.moveTop(top);
        it->setItemGeometry(g);
    }
}

void DesignScene::alignBottom()
{
    const QList<DesignItem *> sel = selectedItems();
    if (sel.size() < 2)
        return;
    qreal bottom = -1e9;
    Q_FOREACH (DesignItem *it, sel)
        bottom = qMax(bottom, it->itemGeometry().bottom());
    Q_FOREACH (DesignItem *it, sel) {
        QRectF g = it->itemGeometry();
        g.moveBottom(bottom);
        it->setItemGeometry(g);
    }
}

void DesignScene::alignHCenter()
{
    const QList<DesignItem *> sel = selectedItems();
    if (sel.size() < 2)
        return;
    qreal cx = 0;
    Q_FOREACH (DesignItem *it, sel)
        cx += it->itemGeometry().center().x();
    cx /= sel.size();
    Q_FOREACH (DesignItem *it, sel) {
        QRectF g = it->itemGeometry();
        g.moveCenter(QPointF(cx, g.center().y()));
        it->setItemGeometry(g);
    }
}

void DesignScene::alignVCenter()
{
    const QList<DesignItem *> sel = selectedItems();
    if (sel.size() < 2)
        return;
    qreal cy = 0;
    Q_FOREACH (DesignItem *it, sel)
        cy += it->itemGeometry().center().y();
    cy /= sel.size();
    Q_FOREACH (DesignItem *it, sel) {
        QRectF g = it->itemGeometry();
        g.moveCenter(QPointF(g.center().x(), cy));
        it->setItemGeometry(g);
    }
}

void DesignScene::equalizeWidth()
{
    const QList<DesignItem *> sel = selectedItems();
    if (sel.size() < 2)
        return;
    qreal w = -1;
    Q_FOREACH (DesignItem *it, sel)
        w = qMax(w, it->itemGeometry().width());
    Q_FOREACH (DesignItem *it, sel) {
        QRectF g = it->itemGeometry();
        g.setWidth(w);
        it->setItemGeometry(g);
    }
}

void DesignScene::equalizeHeight()
{
    const QList<DesignItem *> sel = selectedItems();
    if (sel.size() < 2)
        return;
    qreal h = -1;
    Q_FOREACH (DesignItem *it, sel)
        h = qMax(h, it->itemGeometry().height());
    Q_FOREACH (DesignItem *it, sel) {
        QRectF g = it->itemGeometry();
        g.setHeight(h);
        it->setItemGeometry(g);
    }
}

void DesignScene::bringToFront()
{
    qreal maxZ = 0;
    Q_FOREACH (DesignItem *it, allItems())
        maxZ = qMax(maxZ, it->zValue());
    Q_FOREACH (DesignItem *it, selectedItems())
        it->setZValue(maxZ + 1);
}

void DesignScene::sendToBack()
{
    qreal minZ = 0;
    Q_FOREACH (DesignItem *it, allItems())
        minZ = qMin(minZ, it->zValue());
    Q_FOREACH (DesignItem *it, selectedItems())
        it->setZValue(minZ - 1);
}

void DesignScene::applyStyleSheet(const QString &qss)
{
    m_currentQss = qss;
    Q_FOREACH (DesignItem *item, allItems()) {
        item->setDesignStyleSheet(qss);
    }
}

QString DesignScene::currentStyleSheet() const
{
    return m_currentQss;
}

void DesignScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if (event->button() == Qt::LeftButton && itemAt(event->scenePos(), QTransform()) == 0)
        clearSelection();
}

#ifndef DESIGNSCENE_H
#pragma execution_character_set("utf-8")
#define DESIGNSCENE_H

#include <QGraphicsScene>
#include <QVariant>

class DesignItem;

// Central design canvas. Owns the placed widgets and provides alignment,
// ordering, deletion and style application operations.
class DesignScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit DesignScene(QObject *parent = 0);

    DesignItem *addWidget(const QString &typeName, const QPointF &scenePos,
                          const QVariant &variant = QVariant());
    QList<DesignItem *> selectedItems() const;

    void deleteSelected();
    void copySelected();
    void paste();
    void clearAll();

    void alignLeft();
    void alignRight();
    void alignTop();
    void alignBottom();
    void alignHCenter();
    void alignVCenter();
    void equalizeWidth();
    void equalizeHeight();
    void bringToFront();
    void sendToBack();

    void applyStyleSheet(const QString &qss);
    QString currentStyleSheet() const;
    QList<DesignItem *> allItems() const;

    QString nextObjectName(const QString &baseName) const;

signals:
    void itemPropertyChanged(DesignItem *item);
    void sceneChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QList<QString> m_clipboardTypes;
    QList<QRectF> m_clipboardGeometries;
    QString m_currentQss;
};

#endif // DESIGNSCENE_H

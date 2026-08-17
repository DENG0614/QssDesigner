#ifndef DESIGNVIEW_H
#pragma execution_character_set("utf-8")
#define DESIGNVIEW_H

#include <QGraphicsView>

class DesignScene;
class QMimeData;

// QGraphicsView with drag & drop support from the widget palette and
// Ctrl+wheel zoom. Owns the design scene so it can be instantiated directly
// from a .ui form (promoted widget).
class DesignView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit DesignView(QWidget *parent = 0);

    DesignScene *designScene() const;

    // 解析控件拖拽载荷并在场景中创建控件（dropEvent 与自测共用）
    bool dropWidget(const QMimeData *mime, const QPoint &viewportPos);

signals:
    void zoomChanged(double factor);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    DesignScene *m_designScene;
    double m_zoomFactor;
};

#endif // DESIGNVIEW_H

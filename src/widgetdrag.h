#ifndef WIDGETDRAG_H
#pragma execution_character_set("utf-8")
#define WIDGETDRAG_H

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QPoint>
#include <QString>
#include <QVariant>

class QWidget;

// 统一控件拖拽：控件库与预览面板共用同一 mime 载荷。
// 载荷为 JSON 数组，每个元素含 type / variant / props（源控件属性），
// 支持一次拖出多个控件（如输入控件连同前面的标签）与对话框动作。
class WidgetDragFilter : public QObject
{
    Q_OBJECT

public:
    struct DragSource {
        QString type;       // Qt 类名
        QVariant variant;   // 变体
        QWidget *source;    // 属性来源控件（可为 0）
        QString dialogAction; // 对话框动作（可选）

        DragSource(const QString &t, const QVariant &v, QWidget *s = 0,
                   const QString &action = QString())
            : type(t)
            , variant(v)
            , source(s)
            , dialogAction(action)
        {
        }
    };

    WidgetDragFilter(QWidget *target, const QList<DragSource> &sources,
                     QWidget *grabSource = 0);
    WidgetDragFilter(QWidget *target, const QString &type, const QVariant &variant,
                     QWidget *grabSource = 0);

    static const char *mimeType();
    static QByteArray mimePayload(const QList<DragSource> &sources);

protected:
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private:
    void startDrag(const QPoint &hotspot);

    QList<DragSource> m_sources;
    QWidget *m_grabSource;
    bool m_pressed;
    QPoint m_pressPos;
};

#endif // WIDGETDRAG_H

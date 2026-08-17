#pragma execution_character_set("utf-8")

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QTabWidget>

// 控件预览面板：按 QssStylesheetEditor v1.8 预览的 7 个页签逐项移植，
// 覆盖全部 Qt 常用控件，跟随当前 QSS 实时换肤。
// 所有控件均可 Ctrl+拖拽到画布（带标签的控件连同标签成组拖出）。
class PreviewWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget *parent = 0);

    void applyStyleSheet(const QString &qss);

private:
    QWidget *makeTabBasic();      // 常用组件
    QWidget *makeTabSpecial();    // 特别组件
    QWidget *makeTabDrawing();    // 绘制组件
    QWidget *makeTabLayout();     // 布局组件
    QWidget *makeTabContainer();  // 容器组件
    QWidget *makeTabAdvance();    // 复杂组件
    QWidget *makeTabCustom();     // 自定义（完整界面）
};

#endif // PREVIEWWIDGET_H

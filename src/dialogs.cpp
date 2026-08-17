#pragma execution_character_set("utf-8")

#include "dialogs.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QWidget>

void showStandardDialog(QWidget *parent, const QString &action)
{
    // 仅当父窗口是当前活动窗口时才作为模态框父对象；
    // 否则使用无父对话框，确保任何情况下都能正常弹出。
    QWidget *win = parent ? parent->window() : 0;
    if (win && QApplication::activeWindow() != win)
        win = 0;

    // 注意：中文动作名比较必须用 QStringLiteral（QLatin1String 无法匹配中文）
    if (action == QStringLiteral("信息")) {
        QMessageBox::information(win, QStringLiteral("信息"), QStringLiteral("这是一条信息。"));
    } else if (action == QStringLiteral("提问")) {
        QMessageBox::question(win, QStringLiteral("提问"), QStringLiteral("你确定吗？"));
    } else if (action == QStringLiteral("警告")) {
        QMessageBox::warning(win, QStringLiteral("警告"), QStringLiteral("这是一个警告。"));
    } else if (action == QStringLiteral("错误")) {
        QMessageBox::critical(win, QStringLiteral("错误"), QStringLiteral("发生了一个错误。"));
    } else if (action == QStringLiteral("关于")) {
        QMessageBox::about(win, QStringLiteral("关于"), QStringLiteral("QssDesigner 控件预览演示。"));
    } else if (action == QStringLiteral("输入整数")) {
        QInputDialog::getInt(win, QStringLiteral("输入"), QStringLiteral("请输入整数："));
    } else if (action == QStringLiteral("输入浮点")) {
        QInputDialog::getDouble(win, QStringLiteral("输入"), QStringLiteral("请输入浮点数："));
    } else if (action == QStringLiteral("输入选择")) {
        QStringList items;
        items << QStringLiteral("选项A") << QStringLiteral("选项B") << QStringLiteral("选项C");
        QInputDialog::getItem(win, QStringLiteral("选择"), QStringLiteral("请选择："), items);
    } else if (action == QStringLiteral("颜色")) {
        QColorDialog::getColor(Qt::white, win, QStringLiteral("颜色"));
    } else if (action == QStringLiteral("字体")) {
        QFontDialog::getFont(0, win);
    } else if (action == QStringLiteral("打开文件")) {
        QFileDialog::getOpenFileName(win, QStringLiteral("打开文件"), QString(),
                                     QStringLiteral("文本文件 (*.txt *.text);;所有文件 (*.*)"));
    } else if (action == QStringLiteral("保存文件")) {
        QFileDialog::getSaveFileName(win, QStringLiteral("保存文件"));
    }
}

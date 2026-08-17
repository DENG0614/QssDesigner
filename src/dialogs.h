#ifndef DIALOGS_H
#pragma execution_character_set("utf-8")
#define DIALOGS_H

#include <QString>

class QWidget;

// 移植自 QssStylesheetEditor v1.8 Preview 的对话框演示。
// 预览面板与画布拖出的按钮共用同一套动作。
void showStandardDialog(QWidget *parent, const QString &action);

#endif // DIALOGS_H

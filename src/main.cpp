#pragma execution_character_set("utf-8")

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMessageLogContext>
#include <QMimeData>
#include <QPushButton>
#include <QTimer>
#include <QImage>

#include <cstdio>

#include "designitem.h"
#include "previewwidget.h"
#include "mainwindow.h"
#include "widgetpalette.h"
#include "widgetdrag.h"

namespace {

QtMessageHandler g_defaultHandler = 0;

// 过滤 Qt QSS 解析器对未知属性的逐条刷屏警告（如 border-bottom-radius）
void filteredMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (msg.contains(QLatin1String("Unknown property")))
        return;
    if (g_defaultHandler)
        g_defaultHandler(type, context, msg);
    else
        fprintf(stderr, "%s\n", qPrintable(msg));
}

} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("QssDesigner"));
    app.setOrganizationName(QStringLiteral("QssDesigner"));
    g_defaultHandler = qInstallMessageHandler(filteredMessageHandler);

    MainWindow w;
    if (argc >= 3 && qstrcmp(argv[1], "--selftest") == 0) {
        const QString dir = QString::fromLocal8Bit(argv[2]);
        QDir().mkpath(dir);
        w.addAllWidgetTypes();

        // 模拟“从预览面板拖到画布”的 drop：验证同一 mime 载荷能被画布接收并创建控件
        const int beforeDrop = w.sceneItemCount();
        {
            const QStringList types = QStringList()
                << QStringLiteral("QPushButton") << QStringLiteral("QCheckBox");
            Q_FOREACH (const QString &type, types) {
                QMimeData mime;
                QList<WidgetDragFilter::DragSource> sources;
                sources << WidgetDragFilter::DragSource(type, QVariant(0));
                mime.setData(QLatin1String(WidgetDragFilter::mimeType()),
                             WidgetDragFilter::mimePayload(sources));
                const bool ok = w.designView()->dropWidget(&mime, QPoint(420, 80));
                if (!ok)
                    return 3; // drop 载荷解析失败
            }
        }
        // 验证“拖出的控件与预览一致”：属性随载荷传递
        {
            QPushButton srcBtn(QStringLiteral("拖出按钮"));
            srcBtn.setEnabled(false);
            QMimeData mime;
            QList<WidgetDragFilter::DragSource> s;
            s << WidgetDragFilter::DragSource(QStringLiteral("QPushButton"), QVariant(0), &srcBtn);
            mime.setData(QLatin1String(WidgetDragFilter::mimeType()),
                         WidgetDragFilter::mimePayload(s));
            if (!w.designView()->dropWidget(&mime, QPoint(420, 160)))
                return 4;
        }
        // 验证“输入控件连同前面的标签一起拖出”
        {
            QLabel srcLabel(QStringLiteral("测试标签："));
            QLineEdit srcEdit;
            srcEdit.setPlaceholderText(QStringLiteral("请输入..."));
            QMimeData mime;
            QList<WidgetDragFilter::DragSource> s;
            s << WidgetDragFilter::DragSource(QStringLiteral("QLabel"), QVariant(0), &srcLabel);
            s << WidgetDragFilter::DragSource(QStringLiteral("QLineEdit"), QVariant(0), &srcEdit);
            mime.setData(QLatin1String(WidgetDragFilter::mimeType()),
                         WidgetDragFilter::mimePayload(s));
            if (!w.designView()->dropWidget(&mime, QPoint(420, 240)))
                return 5;
        }

        const int afterDrop = w.sceneItemCount();
        qDebug() << "DROP-TEST items" << beforeDrop << "->" << afterDrop;
        if (afterDrop - beforeDrop != 5) // 2 单控件 + 1 带属性按钮 + 2 标签+输入框
            return 2; // 拖拽 drop 验证失败
        // 验证拖出的控件立即应用了当前样式（与预览一致）
        if (w.styledItemCount() != w.sceneItemCount())
            return 6; // 有控件未应用样式

        // 验证预览中的对话框按钮点击后确实弹出对话框
        {
            w.show();
            QApplication::processEvents();
            // 先直接验证 QMessageBox 本身能否弹出（父对象为 0）
            bool directShown = false;
            QTimer::singleShot(300, [&directShown]() {
                Q_FOREACH (QWidget *tlw, QApplication::topLevelWidgets()) {
                    if (qobject_cast<QMessageBox *>(tlw)) {
                        directShown = true;
                        tlw->close();
                    }
                }
            });
            QMessageBox::information(0, QStringLiteral("信息"), QStringLiteral("这是一条信息。"));
            if (!directShown)
                return 9;

            QPushButton *infoBtn = 0;
            Q_FOREACH (QPushButton *b, w.previewWidget()->findChildren<QPushButton *>()) {
                if (b->text() == QStringLiteral("消息对话框")) {
                    infoBtn = b;
                    break;
                }
            }
            if (!infoBtn)
                return 8; // 未找到对话框按钮
            bool shown = false;
            QTimer::singleShot(500, [&shown]() {
                Q_FOREACH (QWidget *tlw, QApplication::topLevelWidgets()) {
                    if (qobject_cast<QMessageBox *>(tlw)) {
                        shown = true;
                        tlw->close();
                    }
                }
            });
            infoBtn->click();
            if (!shown)
                return 7; // 对话框未弹出
        }

        // 验证“预览与画布样式一致”：对比预览按钮与画布按钮的渲染像素
        {
            QPushButton *previewBtn = 0;
            Q_FOREACH (QPushButton *b, w.previewWidget()->findChildren<QPushButton *>()) {
                if (b->text() == QStringLiteral("Button")) {
                    previewBtn = b;
                    break;
                }
            }
            if (!previewBtn)
                return 10;
            DesignItem *canvasBtn = w.designView()->designScene()->addWidget(
                QStringLiteral("QPushButton"), QPointF(80, 60));
            canvasBtn->applyProperty(QStringLiteral("text"), QStringLiteral("Button"));
            QApplication::processEvents();
            const QImage imgA = previewBtn->grab().toImage();
            const QImage imgB = canvasBtn->widget()->grab().toImage();
            if (imgA.isNull() || imgB.isNull())
                return 11;
            const QImage a = imgA.scaled(240, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
                                 .convertToFormat(QImage::Format_RGB32);
            const QImage b2 = imgB.scaled(240, 70, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
                                  .convertToFormat(QImage::Format_RGB32);
            int diff = 0;
            for (int y = 0; y < a.height(); ++y) {
                for (int x = 0; x < a.width(); ++x) {
                    const QRgb pa = a.pixel(x, y);
                    const QRgb pb = b2.pixel(x, y);
                    if (qAbs(qRed(pa) - qRed(pb)) > 40
                        || qAbs(qGreen(pa) - qGreen(pb)) > 40
                        || qAbs(qBlue(pa) - qBlue(pb)) > 40)
                        ++diff;
                }
            }
            const double ratio = double(diff) / (a.width() * a.height());
            qDebug() << "PIXEL-DIFF ratio" << ratio;
            if (ratio > 0.25)
                return 12; // 预览与画布渲染差异过大
        }
        w.hide();

        QString error;
        const bool ok = w.exportToDir(dir, &error);
        qDebug() << (ok ? "SELFTEST OK" : "SELFTEST FAILED") << error;
        return ok ? 0 : 1;
    }
    w.show();
    return app.exec();
}

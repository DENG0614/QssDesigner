#pragma execution_character_set("utf-8")

#include "qsseditor.h"

#include "qsshighlighter.h"

#include <QFontMetricsF>
#include <QKeyEvent>
#include <QtGlobal>

QssEditor::QssEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_highlighter(0)
{
    setLineWrapMode(QPlainTextEdit::NoWrap);
    setFont(QFont(QStringLiteral("Consolas"), 10));
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    setTabStopDistance(4 * QFontMetricsF(font()).horizontalAdvance(QLatin1Char(' ')));
#else
    setTabStopWidth(4 * fontMetrics().width(QLatin1Char(' ')));
#endif
    m_highlighter = new QssHighlighter(document());
}

void QssEditor::setHighlightingEnabled(bool enabled)
{
    if (enabled && !m_highlighter)
        m_highlighter = new QssHighlighter(document());
    else if (!enabled && m_highlighter) {
        delete m_highlighter;
        m_highlighter = 0;
    }
}

void QssEditor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        textCursor().insertText(QString(4, QLatin1Char(' ')));
        event->accept();
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}

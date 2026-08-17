#ifndef QSSEDITOR_H
#pragma execution_character_set("utf-8")
#define QSSEDITOR_H

#include <QPlainTextEdit>

class QssHighlighter;

// QSS source editor with syntax highlighting, used for customizing the
// currently selected style.
class QssEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit QssEditor(QWidget *parent = 0);

    void setHighlightingEnabled(bool enabled);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    QssHighlighter *m_highlighter;
};

#endif // QSSEDITOR_H

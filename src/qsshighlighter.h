#ifndef QSSHIGHLIGHTER_H
#pragma execution_character_set("utf-8")
#define QSSHIGHLIGHTER_H

#include <QColor>
#include <QSyntaxHighlighter>

class QTextDocument;

// Syntax highlighter for QSS / QSST files.
// Style ids mirror the original QsciLexerQSS from the Python version:
// Default, Tag, IDSelector, ClassSelector, PseudoElement, PseudoClass,
// Attribute, Operator, Property, Value, Comment, strings, Variable, Param.
class QssHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum Style {
        SDefault = 0,
        STag,
        SIDSelector,
        SClassSelector,
        SPseudoElement,
        SPseudoClass,
        SAttribute,
        SOperator,
        SProperty,
        SValue,
        SComment,
        SDoubleQuotedString,
        SSingleQuotedString,
        SVariable,
        SParam,
        StyleCount
    };

    explicit QssHighlighter(QTextDocument *parent = 0);

    void setPaper(const QColor &color);
    QColor paper() const;

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    void setFormatRange(int start, int length, Style style);

    QColor m_colors[StyleCount];
    QColor m_paper;
};

#endif // QSSHIGHLIGHTER_H

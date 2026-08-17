#pragma execution_character_set("utf-8")

#include "qsshighlighter.h"

#include <QRegularExpression>

namespace {

// continuation bits stored in block user state (depth << 2 | continuation)
enum Continuation {
    CNone = 0,
    CComment = 1,
    CDoubleString = 2,
    CSingleString = 3
};

// syntax states
enum State {
    StDefault = 0,
    StTag,
    StProperty,
    StValue,
    StPseudoClass,
    StPseudoElement,
    StIDSelector,
    StClassSelector,
    StAttribute,
    StComment,
    StDoubleString,
    StSingleString,
    StVariable,
    StParam,
    StateCount
};

struct Token {
    QString text;
    int length;
};

QList<Token> tokenize(const QString &text)
{
    QList<Token> tokens;
    static const QRegularExpression re(
        "(/\\*|\\*/|::|\"|'|\\$|=|\\{|\\}|:|#|\\[|\\]|\\(|\\)|,|;|\\s+|\\w+|[^\\s\\w{}[\\](),:;#=$%\"']+)");
    QRegularExpressionMatchIterator it = re.globalMatch(text);
    while (it.hasNext()) {
        const QRegularExpressionMatch m = it.next();
        Token t;
        t.text = m.captured(1);
        t.length = t.text.length();
        tokens.append(t);
    }
    return tokens;
}

bool isOperator(const QString &t)
{
    static const QStringList ops = QStringList()
        << QLatin1String("{") << QLatin1String("}") << QLatin1String("[") << QLatin1String("]")
        << QLatin1String("(") << QLatin1String(")") << QLatin1String("::") << QLatin1String(".")
        << QLatin1String(":") << QLatin1String(";") << QLatin1String(",") << QLatin1String("/*")
        << QLatin1String("*/") << QLatin1String("#") << QLatin1String("$") << QLatin1String("=")
        << QLatin1String("\"") << QLatin1String("'");
    return ops.contains(t);
}

QssHighlighter::Style styleForState(int state)
{
    switch (state) {
    case StTag: return QssHighlighter::STag;
    case StProperty: return QssHighlighter::SProperty;
    case StValue: return QssHighlighter::SValue;
    case StPseudoClass: return QssHighlighter::SPseudoClass;
    case StPseudoElement: return QssHighlighter::SPseudoElement;
    case StIDSelector: return QssHighlighter::SIDSelector;
    case StClassSelector: return QssHighlighter::SClassSelector;
    case StAttribute: return QssHighlighter::SAttribute;
    case StComment: return QssHighlighter::SComment;
    case StDoubleString: return QssHighlighter::SDoubleQuotedString;
    case StSingleString: return QssHighlighter::SSingleQuotedString;
    case StVariable: return QssHighlighter::SVariable;
    case StParam: return QssHighlighter::SParam;
    default: return QssHighlighter::SDefault;
    }
}

} // namespace

QssHighlighter::QssHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , m_paper(QColor(QLatin1String("#ffffff")))
{
    m_colors[SDefault] = QColor(QLatin1String("#000000"));
    m_colors[STag] = QColor(QLatin1String("#009999"));
    m_colors[SIDSelector] = QColor(QLatin1String("#aa1111"));
    m_colors[SClassSelector] = QColor(QLatin1String("#aa1111"));
    m_colors[SPseudoElement] = QColor(QLatin1String("#aa1111"));
    m_colors[SPseudoClass] = QColor(QLatin1String("#aa1111"));
    m_colors[SAttribute] = QColor(QLatin1String("#aaaaaa"));
    m_colors[SOperator] = QColor(QLatin1String("gray"));
    m_colors[SProperty] = QColor(QLatin1String("#0044ee"));
    m_colors[SValue] = QColor(QLatin1String("#880088"));
    m_colors[SComment] = QColor(QLatin1String("gray"));
    m_colors[SDoubleQuotedString] = QColor(QLatin1String("#669900"));
    m_colors[SSingleQuotedString] = QColor(QLatin1String("#669900"));
    m_colors[SVariable] = QColor(QLatin1String("#aa6600"));
    m_colors[SParam] = QColor(QLatin1String("#aaaaaa"));
}

void QssHighlighter::setPaper(const QColor &color)
{
    m_paper = color;
    rehighlight();
}

QColor QssHighlighter::paper() const
{
    return m_paper;
}

void QssHighlighter::setFormatRange(int start, int length, Style style)
{
    if (length <= 0)
        return;
    QTextCharFormat fmt;
    fmt.setForeground(m_colors[style]);
    setFormat(start, length, fmt);
}

void QssHighlighter::highlightBlock(const QString &text)
{
    const int prevState = previousBlockState();
    int depth = qMax(0, prevState / 4);
    int cont = prevState % 4;
    if (cont < 0)
        cont = CNone;

    bool inBrace = depth > 0;
    int newDepth = depth;
    bool inBracket = false;
    bool inParentheses = false;
    int state = inBrace ? StProperty : StTag;
    int lastState = state;
    QString opPrev = QLatin1String(" ");

    const QList<Token> tokens = tokenize(text);
    int pos = 0;

    Q_FOREACH (const Token &tok, tokens) {
        const QString &token = tok.text;
        const int count = tok.length;

        if (state == StComment) {
            if (token == QLatin1String("*/")) {
                setFormatRange(pos, count, SOperator);
                state = inBrace ? StProperty : StTag;
            } else {
                setFormatRange(pos, count, SComment);
            }
        } else if (state == StDoubleString) {
            if (token == QLatin1String("\"")) {
                state = lastState;
                setFormatRange(pos, count, SDoubleQuotedString);
            } else {
                setFormatRange(pos, count, SDoubleQuotedString);
            }
        } else if (state == StSingleString) {
            if (token == QLatin1String("'")) {
                state = lastState;
                setFormatRange(pos, count, SSingleQuotedString);
            } else {
                setFormatRange(pos, count, SSingleQuotedString);
            }
        } else if (inParentheses) {
            if (token == QLatin1String(")")) {
                inParentheses = false;
                state = lastState;
                setFormatRange(pos, count, SOperator);
            } else {
                setFormatRange(pos, count, SParam);
            }
        } else if (isOperator(token)) {
            bool opStyle = true;
            lastState = state;

            if (token == QLatin1String("/*")) {
                state = StComment;
            } else if (token == QLatin1String("*/")) {
                state = inBrace ? StProperty : StTag;
            } else if (token == QLatin1String("\"")) {
                state = StDoubleString;
                opStyle = false;
            } else if (token == QLatin1String("'")) {
                state = StSingleString;
                opStyle = false;
            } else if (token == QLatin1String("$")) {
                state = StVariable;
                opStyle = false;
            } else if (token == QLatin1String("=")) {
                state = inBracket ? StAttribute : StValue;
            } else if (token == QLatin1String("{")) {
                state = StProperty;
                inBrace = true;
                ++newDepth;
            } else if (token == QLatin1String("}")) {
                state = StTag;
                inBrace = false;
                newDepth = qMax(0, newDepth - 1);
            } else if (token == QLatin1String(":")) {
                state = inBrace ? StValue : StPseudoClass;
            } else if (token == QLatin1String("::")) {
                lastState = StProperty;
                state = StPseudoElement;
            } else if (token == QLatin1String("#")) {
                state = inBrace ? StParam : StIDSelector;
            } else if (token == QLatin1String("[")) {
                lastState = StProperty;
                state = StAttribute;
                inBracket = true;
            } else if (token == QLatin1String("]")) {
                state = StTag;
                inBracket = false;
            } else if (token == QLatin1String("(")) {
                state = StParam;
                inParentheses = true;
            } else if (token == QLatin1String(")")) {
                state = lastState;
                inParentheses = false;
            } else if (token == QLatin1String(",")) {
                if (!inBrace) {
                    lastState = state;
                    state = StTag;
                }
            } else if (token == QLatin1String(";")) {
                state = opPrev == QLatin1String("=") ? StTag : StProperty;
            } else {
                state = lastState;
            }
            opPrev = token;
            setFormatRange(pos, count, opStyle ? SOperator : styleForState(state));
        } else {
            setFormatRange(pos, count, styleForState(state));
        }
        pos += count;
    }

    int nextCont = CNone;
    if (state == StComment)
        nextCont = CComment;
    else if (state == StDoubleString)
        nextCont = CDoubleString;
    else if (state == StSingleString)
        nextCont = CSingleString;

    setCurrentBlockState(newDepth * 4 + nextCont);
}

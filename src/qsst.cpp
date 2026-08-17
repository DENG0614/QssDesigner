#pragma execution_character_set("utf-8")

#include "qsst.h"

#include <QColor>
#include <QRegularExpression>
#include <QSet>

#include <functional>

namespace {

QString regexSub(const QString &text, const QRegularExpression &re,
                 const std::function<QString(const QRegularExpressionMatch &)> &fn)
{
    QString out;
    int last = 0;
    QRegularExpressionMatchIterator it = re.globalMatch(text);
    while (it.hasNext()) {
        QRegularExpressionMatch m = it.next();
        out += text.mid(last, m.capturedStart() - last);
        out += fn(m);
        last = m.capturedEnd();
    }
    out += text.mid(last);
    return out;
}

} // namespace

Qsst::Qsst()
{
}

bool Qsst::loadVars(const QString &qssStr)
{
    const QString src = qssStr.isEmpty() ? srctext : qssStr;

    // variables used inside declarations: ":$name" or ": ... $name"
    QSet<QString> used;
    QRegularExpression reUsed(":[ \\t\\w,.:()]*\\$([\\w]+)");
    QRegularExpressionMatchIterator itUsed = reUsed.globalMatch(src);
    while (itUsed.hasNext())
        used.insert(itUsed.next().captured(1));
    varUsed = used.values();

    // variable definitions: "$name = value;" / "$name = value\n"
    QSet<QString> defined;
    QMap<QString, QString> vars;
    QRegularExpression reDef("\\$(\\w+)\\s*=[ \\t]*([#(),.\\w ]*)[\\t ]*[\\r\\n;/]+");
    QRegularExpressionMatchIterator itDef = reDef.globalMatch(src);
    while (itDef.hasNext()) {
        QRegularExpressionMatch m = itDef.next();
        defined.insert(m.captured(1));
        vars.insert(m.captured(1), m.captured(2).trimmed());
    }

    varDict.clear();
    bool valerr = false;
    QRegularExpression reValid("#[0-9A-Fa-f]{1,8}|rgb\\(\\s*[0-9]*\\s*(,\\s*[0-9]*\\s*){2}\\)|"
                               "rgba\\(\\s*[0-9]*\\s*(,\\s*[0-9]*\\s*){3}\\)|[\\w]*px");
    QMapIterator<QString, QString> it(vars);
    while (it.hasNext()) {
        it.next();
        if (valerr)
            break;
        const QString val = it.value();
        if (QColor::colorNames().contains(val)) {
            varDict.insert(it.key(), val);
        } else {
            const QRegularExpressionMatch vm = reValid.match(val);
            if (!vm.hasMatch())
                valerr = true;
            else
                varDict.insert(it.key(), val);
        }
    }

    varUndefined.clear();
    Q_FOREACH (const QString &varused, varUsed) {
        if (!varDict.contains(varused)) {
            varDict.insert(varused, QString());
            varUndefined.append(varused);
        }
    }

    return !valerr;
}

QString Qsst::addColorOpacity(const QString &color, const QString &opacity) const
{
    if (!color.contains(QLatin1Char('#')))
        return color;
    if (color.length() != 4 && color.length() != 7)
        return color;

    QString expanded = color;
    if (color.length() == 4) {
        expanded.clear();
        for (int i = 0; i < color.length(); ++i)
            expanded += QString(color.at(i)) + color.at(i); // "#8BF" -> "##88BBFF"
        expanded = expanded.mid(1);                         // -> "#88BBFF"
    }

    int alpha = qRound(opacity.toInt() / 100.0 * 255.0 + 0.5);
    if (alpha < 0)
        alpha = 0;
    if (alpha > 255)
        alpha = 255;

    return QLatin1String("#") + QString::number(alpha, 16).rightJustified(2, QLatin1Char('0'))
            + expanded.mid(1);
}

void Qsst::convertQss()
{
    const QMap<QString, QString> varDictOld = varDict;
    loadVars();

    QString qssStr = srctext;

    // remove variable definition lines
    QRegularExpression reDef("\\$(\\w+)\\s*=[ \\t]*([#(),.\\w ]*)[ \\t;]*[\\r\\n]{0,2}");
    qssStr.remove(reDef);

    Q_FOREACH (const QString &v, varDict.keys()) {
        if (varDictOld.contains(v)) {
            // "$var%opacity"
            QRegularExpression reOpacity("\\$(\\w+)[%](\\w+)([\\s;]*)");
            qssStr = regexSub(qssStr, reOpacity, [&](const QRegularExpressionMatch &m) {
                return addColorOpacity(varDictOld.value(m.captured(1)), m.captured(2)) + m.captured(3);
            });
            // "$var"
            QRegularExpression reVar("\\$(\\w+)([\\s;]*)");
            qssStr = regexSub(qssStr, reVar, [&](const QRegularExpressionMatch &m) {
                return varDictOld.value(m.captured(1)) + m.captured(2);
            });
        } else {
            if (!varUndefined.contains(v))
                varUndefined.append(v);
            QRegularExpression reVar("\\$(\\w+)([\\s;]*)");
            qssStr = regexSub(qssStr, reVar, [&](const QRegularExpressionMatch &m) {
                return QString(QLatin1String(" ")) + m.captured(2);
            });
        }
    }

    // remove code blocks: ~~~ ... ~~~ or ``` ... ```
    QRegularExpression reBlock("~{3}.*[\\r\\n]+(.*)[\\r\\n]+~{3}|`{3}.*[\\r\\n]+(.*)[\\r\\n]+`{3}",
                               QRegularExpression::DotMatchesEverythingOption);
    qssStr.remove(reBlock);

    qss = qssStr;
}

void Qsst::writeVars()
{
    const QMap<QString, QString> varDictNew = varDict;
    loadVars();

    QRegularExpression reDef("\\$(\\w+)\\s*=[ \\t]*([#(),.\\w]*)[\\t ]*[;]?");

    if (!varDict.isEmpty()) {
        // update existing definitions with the new values
        QString src = regexSub(srctext, reDef, [&](const QRegularExpressionMatch &m) {
            const QString v = varDictNew.value(m.captured(1));
            return QString(QLatin1String("$%1 = %2;")).arg(m.captured(1), v);
        });

        // insert definitions for previously undefined variables before the first definition
        if (!varUndefined.isEmpty()) {
            QString s;
            QMapIterator<QString, QString> it(varDictNew);
            while (it.hasNext()) {
                it.next();
                if (varUndefined.contains(it.key()))
                    s += QString(QLatin1String("$%1 = %2;\n")).arg(it.key(), it.value());
            }
            const QRegularExpressionMatch first = reDef.match(src);
            if (first.hasMatch())
                src.insert(first.capturedStart(), s);
            else
                src.prepend(s);
        }
        srctext = src;
    } else {
        QString s;
        QMapIterator<QString, QString> it(varDictNew);
        while (it.hasNext()) {
            it.next();
            s += QString(QLatin1String("$%1 = %2;\n")).arg(it.key(), it.value());
        }
        srctext = s + QLatin1String("\n") + srctext;
    }

    loadVars();
}

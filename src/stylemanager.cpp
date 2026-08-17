#pragma execution_character_set("utf-8")

#include "stylemanager.h"

#include "designscene.h"
#include "qsst.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

StyleManager::StyleManager(const QString &stylesDir)
    : m_stylesDir(stylesDir)
{
    refresh();
}

void StyleManager::refresh()
{
    m_styles.clear();
    scanDir(m_stylesDir, QStringLiteral("styles"));

    // group by top level directory name for readability
    QList<Style> grouped;
    Q_FOREACH (const Style &s, m_styles) {
        Style st = s;
        const QString rel = QDir(m_stylesDir).relativeFilePath(s.path);
        const QString top = rel.section(QLatin1Char('/'), 0, 0);
        if (top == QLatin1String("qsst"))
            st.group = QStringLiteral("QssStylesheetEditor (variables)");
        else if (top == QLatin1String("custom"))
            st.group = QStringLiteral("Custom");
        else
            st.group = QStringLiteral("QSS-master");
        grouped.append(st);
    }
    m_styles = grouped;
}

void StyleManager::scanDir(const QString &dir, const QString &group)
{
    QDir d(dir);
    const QStringList filters = QStringList() << QStringLiteral("*.qss") << QStringLiteral("*.qsst");
    Q_FOREACH (const QFileInfo &fi, d.entryInfoList(filters, QDir::Files)) {
        Style s;
        s.name = fi.completeBaseName();
        s.path = fi.absoluteFilePath();
        s.group = group;
        m_styles.append(s);
    }
    Q_FOREACH (const QFileInfo &sub, d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (sub.fileName() == QLatin1String("QSS_IMG"))
            continue;
        scanDir(sub.absoluteFilePath(), sub.fileName());
    }
}

QList<StyleManager::Style> StyleManager::styles() const
{
    return m_styles;
}

QString StyleManager::loadStyle(const QString &name) const
{
    Q_FOREACH (const Style &s, m_styles) {
        if (s.name == name) {
            QFile f(s.path);
            if (f.open(QIODevice::ReadOnly))
                return QString::fromUtf8(f.readAll());
        }
    }
    return QString();
}

QString StyleManager::fixUrls(const QString &qss, const QString &baseDir) const
{
    QString result = qss;
    static const QRegularExpression re("url\\([\\s]*([^)]*?)[\\s]*\\)");
    QRegularExpressionMatchIterator it = re.globalMatch(qss);
    QStringList replacements;
    while (it.hasNext()) {
        const QRegularExpressionMatch m = it.next();
        QString path = m.captured(1).trimmed();
        if (path.startsWith(QLatin1Char('"')))
            path = path.mid(1);
        if (path.endsWith(QLatin1Char('"')))
            path.chop(1);
        path = path.trimmed();

        QString resolved;
        if (path.startsWith(QLatin1String("/usr/"))) {
            // linux theme icon paths -> bundled icons
            const QString icon = QFileInfo(path).fileName();
            const QString candidate = m_stylesDir + QLatin1String("/QSS_IMG/") + icon;
            if (QFile::exists(candidate))
                resolved = candidate;
        } else if (QFileInfo(path).isAbsolute()) {
            resolved = QFileInfo(path).absoluteFilePath();
        } else if (QFile::exists(baseDir + QLatin1Char('/') + path)) {
            resolved = QFileInfo(baseDir + QLatin1Char('/') + path).absoluteFilePath();
        } else {
            resolved = path; // leave as-is
        }

        const QString replacement = QStringLiteral("url(\"") + resolved.replace(QLatin1Char('\\'), QLatin1String("/"))
                                   + QLatin1String("\")");
        replacements.append(replacement);
    }

    // apply replacements in order
    int offset = 0;
    QRegularExpressionMatchIterator it2 = re.globalMatch(result);
    int i = 0;
    while (it2.hasNext()) {
        const QRegularExpressionMatch m = it2.next();
        if (i >= replacements.size())
            break;
        result.replace(m.capturedStart() + offset, m.capturedLength(),
                       replacements.at(i));
        offset += replacements.at(i).length() - m.capturedLength();
        ++i;
    }
    return result;
}

QString StyleManager::prepareForPreview(const QString &name, const QString &rawText) const
{
    QString text = rawText;
    QString baseDir = m_stylesDir;
    Q_FOREACH (const Style &s, m_styles) {
        if (s.name == name) {
            baseDir = QFileInfo(s.path).absolutePath();
            if (text.isEmpty())
                text = loadStyle(name);
            break;
        }
    }
    if (text.isEmpty())
        return QString();

    // process $variables for .qsst files
    bool isQsst = false;
    Q_FOREACH (const Style &s, m_styles) {
        if (s.name == name && s.path.endsWith(QLatin1String(".qsst"))) {
            isQsst = true;
            break;
        }
    }
    if (isQsst) {
        Qsst qsst;
        qsst.srctext = text;
        if (qsst.loadVars())
            qsst.convertQss();
        text = qsst.qss.isEmpty() ? text : qsst.qss;
    }

    return fixUrls(text, baseDir);
}

bool StyleManager::saveCustomStyle(const QString &name, const QString &text, QString *error)
{
    QString clean = name.trimmed();
    if (clean.isEmpty()) {
        if (error)
            *error = QStringLiteral("样式名称不能为空。");
        return false;
    }
    if (!clean.endsWith(QLatin1String(".qss")))
        clean += QLatin1String(".qss");

    const QString dir = m_stylesDir + QLatin1String("/custom");
    if (!QDir().mkpath(dir)) {
        if (error)
            *error = QStringLiteral("无法创建自定义样式目录。");
        return false;
    }
    QFile f(dir + QLatin1Char('/') + clean);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (error)
            *error = QStringLiteral("无法写入样式文件。");
        return false;
    }
    f.write(text.toUtf8());
    f.close();
    refresh();
    return true;
}

QString StyleManager::stylesDir() const
{
    return m_stylesDir;
}

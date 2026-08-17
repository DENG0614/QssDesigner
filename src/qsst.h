#ifndef QSST_H
#pragma execution_character_set("utf-8")
#define QSST_H

#include <QMap>
#include <QString>
#include <QStringList>

// QssStylesheet template processor:
// - "$name = value;" variable definitions
// - "$name" and "$name%opacity" references
// - "```" / "~~~" code blocks (removed during conversion)
class Qsst
{
public:
    Qsst();

    bool loadVars(const QString &qssStr = QString());
    void convertQss();
    void writeVars();
    QString addColorOpacity(const QString &color, const QString &opacity) const;

    QString srctext;
    QString qss;
    QMap<QString, QString> varDict;
    QStringList varUsed;
    QStringList varUndefined;
    QStringList codeBlocks;
};

#endif // QSST_H

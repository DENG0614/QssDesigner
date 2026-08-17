#ifndef UIWRITER_H
#pragma execution_character_set("utf-8")
#define UIWRITER_H

#include <QList>
#include <QString>

class DesignItem;

// Exports the current design as a Qt Designer .ui XML file.
class UiWriter
{
public:
    static bool writeUi(const QString &filePath,
                        const QList<DesignItem *> &items,
                        const QString &windowTitle,
                        QString *error = 0);
};

#endif // UIWRITER_H

#ifndef PROJECTWRITER_H
#pragma execution_character_set("utf-8")
#define PROJECTWRITER_H

#include <QList>
#include <QString>

class DesignItem;

// Exports a complete, compilable Qt C++ project (main.cpp, mainwindow.cpp/h,
// .pro, .ui and .qss) for the current design.
class ProjectWriter
{
public:
    static bool writeProject(const QString &dir,
                             const QList<DesignItem *> &items,
                             const QString &styleText,
                             const QString &windowTitle,
                             const QString &stylesDir,
                             QString *error = 0);
};

#endif // PROJECTWRITER_H

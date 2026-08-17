#ifndef STYLEMANAGER_H
#pragma execution_character_set("utf-8")
#define STYLEMANAGER_H

#include <QList>
#include <QString>
#include <QStringList>

class DesignScene;
class Qsst;

// Scans the bundled styles directory for .qss/.qsst files, applies a selected
// style to the design scene (with $variable conversion and url() fixup) and
// saves user-defined custom styles.
class StyleManager
{
public:
    struct Style {
        QString name;
        QString path;
        QString group; // "QSS-master", "QssStylesheetEditor", "custom", ...
    };

    explicit StyleManager(const QString &stylesDir);

    QList<Style> styles() const;
    QString loadStyle(const QString &name) const;
    bool saveCustomStyle(const QString &name, const QString &text, QString *error = 0);

    // returns the processed (converted, url-fixed) stylesheet
    QString prepareForPreview(const QString &name, const QString &rawText = QString()) const;

    QString stylesDir() const;
    void refresh();

private:
    void scanDir(const QString &dir, const QString &group);
    QString fixUrls(const QString &qss, const QString &baseDir) const;

    QString m_stylesDir;
    QList<Style> m_styles;
};

#endif // STYLEMANAGER_H

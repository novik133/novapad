/**
 * @file color_scheme.h
 * @brief Data structure representing a complete syntax-highlighting colour scheme.
 *
 * A ColorScheme defines all the colours used by the editor — background,
 * foreground, gutter, selection, and per-token-type syntax colours.
 * Schemes can be serialised to/from JSON for user customisation.
 */

#ifndef NOVAPAD_THEMES_COLOR_SCHEME_H
#define NOVAPAD_THEMES_COLOR_SCHEME_H

#include <QColor>
#include <QJsonObject>
#include <QString>

namespace NovaPad::Themes {

/**
 * @struct ColorScheme
 * @brief Complete set of colours for the editor and syntax highlighting.
 */
struct ColorScheme
{
    // -- Identity ------------------------------------------------------------
    QString id;
    QString name;

    // -- Editor chrome -------------------------------------------------------
    QColor background          = QColor("#FFFFFF");
    QColor foreground          = QColor("#333333");
    QColor editorBackground    = QColor("#FFFFFF");
    QColor editorForeground    = QColor("#333333");
    QColor lineHighlight       = QColor("#FFFDE7");
    QColor selectionBackground = QColor("#B3D4FC");
    QColor selectionForeground = QColor("#000000");
    QColor gutterBackground    = QColor("#F0F0F0");
    QColor gutterForeground    = QColor("#999999");

    // -- Syntax token colours ------------------------------------------------
    QColor keyword      = QColor("#0000FF");
    QColor string       = QColor("#A31515");
    QColor comment      = QColor("#008000");
    QColor number       = QColor("#098658");
    QColor function     = QColor("#795E26");
    QColor type         = QColor("#267F99");
    QColor preprocessor = QColor("#AF00DB");
    QColor operator_    = QColor("#000000");

    // -- Serialisation -------------------------------------------------------

    /** @brief Creates a ColorScheme from a JSON object. */
    [[nodiscard]] static ColorScheme fromJson(const QJsonObject &obj);

    /** @brief Serialises this ColorScheme to a JSON object. */
    [[nodiscard]] QJsonObject toJson() const;
};

} // namespace NovaPad::Themes

#endif // NOVAPAD_THEMES_COLOR_SCHEME_H

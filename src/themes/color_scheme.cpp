/**
 * @file color_scheme.cpp
 * @brief Implementation of ColorScheme JSON serialisation.
 */

#include "color_scheme.h"

namespace NovaPad::Themes {

// ---------------------------------------------------------------------------
// Helper: read a QColor from a JSON string value
// ---------------------------------------------------------------------------

static QColor readColor(const QJsonObject &obj, const QString &key, const QColor &fallback)
{
    if (obj.contains(key)) {
        return QColor(obj.value(key).toString());
    }
    return fallback;
}

// ---------------------------------------------------------------------------
// fromJson
// ---------------------------------------------------------------------------

ColorScheme ColorScheme::fromJson(const QJsonObject &obj)
{
    ColorScheme cs;

    cs.id   = obj.value("id").toString();
    cs.name = obj.value("name").toString(cs.id);

    cs.background          = readColor(obj, "background",          cs.background);
    cs.foreground          = readColor(obj, "foreground",          cs.foreground);
    cs.editorBackground    = readColor(obj, "editorBackground",    cs.editorBackground);
    cs.editorForeground    = readColor(obj, "editorForeground",    cs.editorForeground);
    cs.lineHighlight       = readColor(obj, "lineHighlight",       cs.lineHighlight);
    cs.selectionBackground = readColor(obj, "selectionBackground", cs.selectionBackground);
    cs.selectionForeground = readColor(obj, "selectionForeground", cs.selectionForeground);
    cs.gutterBackground    = readColor(obj, "gutterBackground",    cs.gutterBackground);
    cs.gutterForeground    = readColor(obj, "gutterForeground",    cs.gutterForeground);

    cs.keyword      = readColor(obj, "keyword",      cs.keyword);
    cs.string       = readColor(obj, "string",       cs.string);
    cs.comment      = readColor(obj, "comment",      cs.comment);
    cs.number       = readColor(obj, "number",       cs.number);
    cs.function     = readColor(obj, "function",     cs.function);
    cs.type         = readColor(obj, "type",         cs.type);
    cs.preprocessor = readColor(obj, "preprocessor", cs.preprocessor);
    cs.operator_    = readColor(obj, "operator",     cs.operator_);

    return cs;
}

// ---------------------------------------------------------------------------
// toJson
// ---------------------------------------------------------------------------

QJsonObject ColorScheme::toJson() const
{
    QJsonObject obj;

    obj["id"]   = id;
    obj["name"] = name;

    obj["background"]          = background.name();
    obj["foreground"]          = foreground.name();
    obj["editorBackground"]    = editorBackground.name();
    obj["editorForeground"]    = editorForeground.name();
    obj["lineHighlight"]       = lineHighlight.name();
    obj["selectionBackground"] = selectionBackground.name();
    obj["selectionForeground"] = selectionForeground.name();
    obj["gutterBackground"]    = gutterBackground.name();
    obj["gutterForeground"]    = gutterForeground.name();

    obj["keyword"]      = keyword.name();
    obj["string"]       = string.name();
    obj["comment"]      = comment.name();
    obj["number"]       = number.name();
    obj["function"]     = function.name();
    obj["type"]         = type.name();
    obj["preprocessor"] = preprocessor.name();
    obj["operator"]     = operator_.name();

    return obj;
}

} // namespace NovaPad::Themes

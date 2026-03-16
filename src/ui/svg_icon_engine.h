/**
 * @file svg_icon_engine.h
 * @brief Custom QIconEngine that renders SVG icons with runtime colour overrides.
 *
 * Allows toolbar and menu icons to adapt their fill colour to the current
 * palette, ensuring visibility in both light and dark themes without
 * shipping separate icon sets.
 */

#ifndef NOVAPAD_UI_SVG_ICON_ENGINE_H
#define NOVAPAD_UI_SVG_ICON_ENGINE_H

#include <QIconEngine>
#include <QString>

namespace NovaPad::UI {

/**
 * @class SvgIconEngine
 * @brief QIconEngine subclass that loads an SVG and recolours it on the fly.
 *
 * The engine replaces a placeholder colour token in the SVG markup with the
 * current palette's window-text colour each time the icon is painted, so a
 * single monochrome SVG works for any theme.
 */
class SvgIconEngine : public QIconEngine
{
public:
    /**
     * @brief Constructs the engine from an SVG resource or file path.
     * @param svgPath  Path or Qt resource URI to the SVG file.
     */
    explicit SvgIconEngine(const QString &svgPath);

    // -- QIconEngine interface -----------------------------------------------
    void paint(QPainter *painter, const QRect &rect,
               QIcon::Mode mode, QIcon::State state) override;
    [[nodiscard]] QIconEngine *clone() const override;
    [[nodiscard]] QPixmap pixmap(const QSize &size,
                                 QIcon::Mode mode,
                                 QIcon::State state) override;

private:
    /** @brief Reads and caches the raw SVG markup. */
    void loadSvgData();

    /** @brief Returns SVG markup with the fill colour replaced. */
    [[nodiscard]] QByteArray recolouredSvg(const QColor &colour) const;

    QString    m_svgPath;
    QByteArray m_svgData;   ///< Cached raw SVG content
};

} // namespace NovaPad::UI

#endif // NOVAPAD_UI_SVG_ICON_ENGINE_H

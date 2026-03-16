/**
 * @file svg_icon_engine.cpp
 * @brief Implementation of the palette-aware SVG icon engine.
 */

#include "svg_icon_engine.h"

#include <QApplication>
#include <QFile>
#include <QPainter>
#include <QPalette>
#include <QSvgRenderer>

namespace NovaPad::UI {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

SvgIconEngine::SvgIconEngine(const QString &svgPath)
    : m_svgPath(svgPath)
{
    loadSvgData();
}

// ---------------------------------------------------------------------------
// QIconEngine interface
// ---------------------------------------------------------------------------

void SvgIconEngine::paint(QPainter *painter, const QRect &rect,
                          QIcon::Mode mode, QIcon::State /*state*/)
{
    // Determine the fill colour from the current palette
    QColor colour = QApplication::palette().color(QPalette::WindowText);

    if (mode == QIcon::Disabled) {
        colour.setAlpha(100);
    } else if (mode == QIcon::Selected) {
        colour = QApplication::palette().color(QPalette::HighlightedText);
    }

    const QByteArray svgData = recolouredSvg(colour);
    QSvgRenderer renderer(svgData);
    renderer.render(painter, QRectF(rect));
}

QIconEngine *SvgIconEngine::clone() const
{
    return new SvgIconEngine(m_svgPath);
}

QPixmap SvgIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
    QPixmap pm(size);
    pm.fill(Qt::transparent);

    QPainter painter(&pm);
    paint(&painter, QRect(QPoint(0, 0), size), mode, state);
    return pm;
}

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

void SvgIconEngine::loadSvgData()
{
    QFile file(m_svgPath);
    if (file.open(QIODevice::ReadOnly)) {
        m_svgData = file.readAll();
    }
}

QByteArray SvgIconEngine::recolouredSvg(const QColor &colour) const
{
    if (m_svgData.isEmpty()) return {};

    // Replace common fill colour placeholders with the target colour.
    // The bundled icons use "currentColor" or "#000000" as the fill.
    QByteArray result = m_svgData;
    const QByteArray hexColour = colour.name().toLatin1();

    result.replace("currentColor", hexColour);
    result.replace("#000000", hexColour);
    result.replace("#000", hexColour);

    return result;
}

} // namespace NovaPad::UI

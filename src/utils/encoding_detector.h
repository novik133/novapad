/**
 * @file encoding_detector.h
 * @brief Automatic text encoding detection from raw byte data.
 *
 * Detects encoding by checking for BOM markers first, then applying
 * heuristic analysis.  Supports UTF-8, UTF-16 (BE/LE), UTF-32 (BE/LE),
 * and common single-byte encodings (Latin-1, Windows-1252, etc.).
 */

#ifndef NOVAPAD_UTILS_ENCODING_DETECTOR_H
#define NOVAPAD_UTILS_ENCODING_DETECTOR_H

#include <QByteArray>
#include <QTextCodec>

namespace NovaPad::Utils {

/**
 * @struct EncodingResult
 * @brief Result of an encoding detection attempt.
 */
struct EncodingResult
{
    QTextCodec *codec  = nullptr;  ///< Detected codec (never null after detect())
    bool        hasBom = false;    ///< True if a BOM was found
};

/**
 * @class EncodingDetector
 * @brief Static utility for detecting text encoding from raw bytes.
 */
class EncodingDetector final
{
    Q_DISABLE_COPY_MOVE(EncodingDetector)
    EncodingDetector() = delete;

public:
    /**
     * @brief Detects the encoding of @p data by inspecting BOM and content.
     * @return EncodingResult with the detected codec and BOM flag.
     */
    [[nodiscard]] static EncodingResult detect(const QByteArray &data);

    /**
     * @brief Returns the BOM bytes for the given codec (empty if none).
     */
    [[nodiscard]] static QByteArray bomForCodec(const QTextCodec *codec);
};

} // namespace NovaPad::Utils

#endif // NOVAPAD_UTILS_ENCODING_DETECTOR_H

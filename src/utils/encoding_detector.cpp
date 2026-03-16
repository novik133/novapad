/**
 * @file encoding_detector.cpp
 * @brief Implementation of the automatic encoding detection engine.
 *
 * Detection order:
 *   1. UTF-32 BE BOM (00 00 FE FF)
 *   2. UTF-32 LE BOM (FF FE 00 00)
 *   3. UTF-8 BOM (EF BB BF)
 *   4. UTF-16 BE BOM (FE FF)
 *   5. UTF-16 LE BOM (FF FE)
 *   6. Heuristic: valid UTF-8 multi-byte sequences -> UTF-8
 *   7. Fallback: UTF-8
 */

#include "encoding_detector.h"

namespace NovaPad::Utils {

EncodingResult EncodingDetector::detect(const QByteArray &data)
{
    EncodingResult result;

    if (data.isEmpty()) {
        result.codec = QTextCodec::codecForName("UTF-8");
        return result;
    }

    const int size = data.size();

    // --- BOM detection (most specific first) --------------------------------

    // UTF-32 BE: 00 00 FE FF
    if (size >= 4
        && data[0] == '\x00' && data[1] == '\x00'
        && data[2] == '\xFE' && data[3] == '\xFF') {
        result.codec  = QTextCodec::codecForName("UTF-32BE");
        result.hasBom = true;
        return result;
    }

    // UTF-32 LE: FF FE 00 00
    if (size >= 4
        && data[0] == '\xFF' && data[1] == '\xFE'
        && data[2] == '\x00' && data[3] == '\x00') {
        result.codec  = QTextCodec::codecForName("UTF-32LE");
        result.hasBom = true;
        return result;
    }

    // UTF-8 BOM: EF BB BF
    if (size >= 3
        && data[0] == '\xEF' && data[1] == '\xBB' && data[2] == '\xBF') {
        result.codec  = QTextCodec::codecForName("UTF-8");
        result.hasBom = true;
        return result;
    }

    // UTF-16 BE BOM: FE FF
    if (size >= 2
        && data[0] == '\xFE' && data[1] == '\xFF') {
        result.codec  = QTextCodec::codecForName("UTF-16BE");
        result.hasBom = true;
        return result;
    }

    // UTF-16 LE BOM: FF FE
    if (size >= 2
        && data[0] == '\xFF' && data[1] == '\xFE') {
        result.codec  = QTextCodec::codecForName("UTF-16LE");
        result.hasBom = true;
        return result;
    }

    // --- Heuristic: check if data is valid UTF-8 ----------------------------
    // Scan for multi-byte UTF-8 sequences.  If we find at least one valid
    // multi-byte sequence and no invalid ones, declare UTF-8.
    bool hasMultibyte = false;
    bool invalidUtf8  = false;

    for (int i = 0; i < size && !invalidUtf8; ) {
        unsigned char ch = static_cast<unsigned char>(data[i]);
        int expectedCont = 0;

        if (ch <= 0x7F) {
            ++i;
            continue;
        } else if ((ch & 0xE0) == 0xC0) {
            expectedCont = 1;
        } else if ((ch & 0xF0) == 0xE0) {
            expectedCont = 2;
        } else if ((ch & 0xF8) == 0xF0) {
            expectedCont = 3;
        } else {
            invalidUtf8 = true;
            break;
        }

        if (i + expectedCont >= size) {
            invalidUtf8 = true;
            break;
        }

        for (int j = 1; j <= expectedCont; ++j) {
            unsigned char cont = static_cast<unsigned char>(data[i + j]);
            if ((cont & 0xC0) != 0x80) {
                invalidUtf8 = true;
                break;
            }
        }

        if (!invalidUtf8) {
            hasMultibyte = true;
            i += 1 + expectedCont;
        }
    }

    if (!invalidUtf8) {
        result.codec = QTextCodec::codecForName("UTF-8");
        return result;
    }

    // --- Try Qt's statistical detection -------------------------------------
    QTextCodec *detected = QTextCodec::codecForUtfText(data, nullptr);
    if (detected) {
        result.codec = detected;
        return result;
    }

    // --- Ultimate fallback: Latin-1 (superset of ASCII, never fails) --------
    result.codec = QTextCodec::codecForName("ISO-8859-1");
    return result;
}

QByteArray EncodingDetector::bomForCodec(const QTextCodec *codec)
{
    if (!codec) return {};

    const QByteArray name = codec->name().toLower();

    if (name == "utf-8")    return QByteArray("\xEF\xBB\xBF", 3);
    if (name == "utf-16be") return QByteArray("\xFE\xFF", 2);
    if (name == "utf-16le") return QByteArray("\xFF\xFE", 2);
    if (name == "utf-32be") return QByteArray("\x00\x00\xFE\xFF", 4);
    if (name == "utf-32le") return QByteArray("\xFF\xFE\x00\x00", 4);

    return {};
}

} // namespace NovaPad::Utils

/**
 * @file session_manager.cpp
 * @brief Implementation of session save/restore using XML serialisation.
 *
 * Session XML structure:
 *   <session version="1">
 *     <tabWidget index="0">
 *       <tab filePath="..." cursorLine="..." cursorCol="..."
 *            scrollLeft="..." scrollTop="..." language="..."
 *            encoding="..." bom="..." eol="..." />
 *     </tabWidget>
 *   </session>
 */

#include "session_manager.h"
#include "ui/editor_tab_widget.h"
#include "ui/top_editor_container.h"

#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace NovaPad::Sessions {

// ---------------------------------------------------------------------------
// saveSession
// ---------------------------------------------------------------------------

bool SessionManager::saveSession(NovaPad::Utils::DocEngine * /*docEngine*/,
                                 NovaPad::UI::TopEditorContainer *container,
                                 const QString &filePath)
{
    if (!container) return false;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("session");
    xml.writeAttribute("version", "1");

    for (int twIdx = 0; twIdx < container->count(); ++twIdx) {
        auto *tw = container->tabWidget(twIdx);
        if (!tw) continue;

        xml.writeStartElement("tabWidget");
        xml.writeAttribute("index", QString::number(twIdx));
        xml.writeAttribute("currentTab", QString::number(tw->currentIndex()));

        for (int tabIdx = 0; tabIdx < tw->count(); ++tabIdx) {
            auto editor = tw->editor(tabIdx);
            if (!editor) continue;

            xml.writeStartElement("tab");
            xml.writeAttribute("filePath",
                               editor->filePath().isValid()
                                   ? editor->filePath().toLocalFile()
                                   : QString());
            xml.writeAttribute("tabTitle", tw->tabText(tabIdx));

            auto pos = editor->cursorPos();
            xml.writeAttribute("cursorLine", QString::number(pos.line));
            xml.writeAttribute("cursorCol",  QString::number(pos.column));

            auto scroll = editor->scrollPosition();
            xml.writeAttribute("scrollLeft", QString::number(scroll.first));
            xml.writeAttribute("scrollTop",  QString::number(scroll.second));

            const auto *lang = editor->currentLanguage();
            xml.writeAttribute("language", lang ? lang->id : QString());

            if (editor->codec()) {
                xml.writeAttribute("encoding", QString::fromLatin1(editor->codec()->name()));
            }
            xml.writeAttribute("bom", editor->hasBom() ? "1" : "0");
            xml.writeAttribute("eol", editor->eolSequence());

            // Cache unsaved content
            if (!editor->isClean() && !editor->filePath().isValid()) {
                xml.writeStartElement("content");
                xml.writeCDATA(editor->content());
                xml.writeEndElement(); // content
            }

            xml.writeEndElement(); // tab
        }

        xml.writeEndElement(); // tabWidget
    }

    xml.writeEndElement(); // session
    xml.writeEndDocument();
    file.close();

    return true;
}

// ---------------------------------------------------------------------------
// loadSession
// ---------------------------------------------------------------------------

bool SessionManager::loadSession(NovaPad::Utils::DocEngine * /*docEngine*/,
                                 NovaPad::UI::TopEditorContainer *container,
                                 const QString &filePath)
{
    if (!container) return false;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader xml(&file);

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("session")) {
            while (xml.readNextStartElement()) {
                if (xml.name() == QLatin1String("tabWidget")) {
                    // Ensure a tab widget exists at this index
                    auto *tw = container->currentTabWidget();
                    if (!tw) tw = container->addTabWidget();

                    const int savedCurrentTab =
                        xml.attributes().value("currentTab").toInt();

                    while (xml.readNextStartElement()) {
                        if (xml.name() == QLatin1String("tab")) {
                            const QString fp =
                                xml.attributes().value("filePath").toString();
                            const QString title =
                                xml.attributes().value("tabTitle").toString();
                            const int cursorLine =
                                xml.attributes().value("cursorLine").toInt();
                            const int cursorCol =
                                xml.attributes().value("cursorCol").toInt();
                            const int scrollLeft =
                                xml.attributes().value("scrollLeft").toInt();
                            const int scrollTop =
                                xml.attributes().value("scrollTop").toInt();
                            const QString langId =
                                xml.attributes().value("language").toString();

                            int tabIdx = tw->addEditorTab(false, title);
                            auto editor = tw->editor(tabIdx);
                            if (editor) {
                                // Load file content if path exists
                                if (!fp.isEmpty() && QFileInfo::exists(fp)) {
                                    QFile f(fp);
                                    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                                        editor->setContent(f.readAll());
                                        editor->setFilePath(QUrl::fromLocalFile(fp));
                                        editor->markClean();
                                    }
                                }

                                // Restore cursor and scroll
                                editor->setCursorPos(cursorLine, cursorCol);
                                editor->setScrollPosition(scrollLeft, scrollTop);

                                // Restore language
                                if (!langId.isEmpty()) {
                                    editor->setLanguage(langId);
                                } else if (!fp.isEmpty()) {
                                    editor->setLanguageFromFilePath();
                                }
                            }

                            // Read cached content for unsaved documents
                            while (xml.readNextStartElement()) {
                                if (xml.name() == QLatin1String("content")) {
                                    if (editor) {
                                        editor->setContent(xml.readElementText());
                                        editor->markDirty();
                                    }
                                } else {
                                    xml.skipCurrentElement();
                                }
                            }
                        } else {
                            xml.skipCurrentElement();
                        }
                    }

                    // Restore the active tab
                    if (savedCurrentTab >= 0 && savedCurrentTab < tw->count()) {
                        tw->setCurrentIndex(savedCurrentTab);
                    }
                } else {
                    xml.skipCurrentElement();
                }
            }
        } else {
            xml.skipCurrentElement();
        }
    }

    file.close();
    return !xml.hasError();
}

} // namespace NovaPad::Sessions

/**
 * @file test_language_manager.cpp
 * @brief Unit tests for the LanguageManager singleton.
 */

#include "editor/language_manager.h"

#include <QTest>

using NovaPad::Editor::LanguageManager;

class TestLanguageManager : public QObject
{
    Q_OBJECT

private slots:
    void testSingletonExists()
    {
        const auto &mgr = LanguageManager::instance();
        QVERIFY(!mgr.languages().isEmpty());
    }

    void testFindById()
    {
        const auto *lang = LanguageManager::instance().lookupById("cpp");
        QVERIFY(lang != nullptr);
        QCOMPARE(lang->id, QString("cpp"));
        QVERIFY(!lang->name.isEmpty());
    }

    void testFindByIdNotFound()
    {
        const auto *lang = LanguageManager::instance().lookupById("nonexistent_xyz_999");
        QVERIFY(lang == nullptr);
    }

    void testFindByExtension()
    {
        const auto *lang = LanguageManager::instance().lookupByExtension("py");
        QVERIFY(lang != nullptr);
        QCOMPARE(lang->id, QString("python"));
    }

    void testFindByFileName()
    {
        const auto *lang = LanguageManager::instance().lookupByFileName("Makefile");
        QVERIFY(lang != nullptr);
    }

    void testFindByContent()
    {
        const auto *lang = LanguageManager::instance().lookupByContent("#!/usr/bin/env python3\n");
        QVERIFY(lang != nullptr);
        QCOMPARE(lang->id, QString("python"));
    }

    void testFindByContentBash()
    {
        const auto *lang = LanguageManager::instance().lookupByContent("#!/bin/bash\necho hello\n");
        QVERIFY(lang != nullptr);
    }

    void testPlainTextExists()
    {
        const auto *lang = LanguageManager::instance().lookupById("plaintext");
        QVERIFY(lang != nullptr);
        QCOMPARE(lang->name, QString("Plain Text"));
    }

    void testMultipleExtensions()
    {
        // C++ has multiple extensions (.cpp, .cxx, .cc, .hpp, .hxx, .h)
        const auto *lang1 = LanguageManager::instance().lookupByExtension("cpp");
        const auto *lang2 = LanguageManager::instance().lookupByExtension("cxx");
        const auto *lang3 = LanguageManager::instance().lookupByExtension("hpp");
        QVERIFY(lang1 != nullptr);
        QVERIFY(lang2 != nullptr);
        QVERIFY(lang3 != nullptr);
        QCOMPARE(lang1->id, lang2->id);
        QCOMPARE(lang1->id, lang3->id);
    }
};

QTEST_MAIN(TestLanguageManager)
#include "test_language_manager.moc"

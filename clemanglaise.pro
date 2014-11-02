######################################################################
# Automatically generated by qmake (3.0) ven. janv. 17 14:46:54 2014
######################################################################

TEMPLATE = app
TARGET = clemanglaise
INCLUDEPATH += . include

# Input
HEADERS += \
    include/AnswerView.h \
    include/EditView.h \
    include/HandwritingArea.h \
    include/HomeView.h \
    include/LanguageButtons.h \
    include/NetworkReplyReader.h \
    include/Parser.h \
    include/QuestionView.h \
    include/SearchView.h \
    include/string_utils.h \
    include/Test.h \
    include/TestView.h \
    include/WordView.h \
    include/DatabaseManager.h \
    include/AddListView.h \
    include/Switcher.h \
    include/ClickableLabel.h \
    include/import_wizard/DstListPage.h \
    include/import_wizard/DuplicatePage.h \
    include/import_wizard/SingleImportWizard.h \
    include/import_wizard/ListImportWizard.h \
    include/import_wizard/BehaviorPage.h \
    include/import_wizard/ProgressPage.h \
    include/import_wizard/Importer.h \
    include/duplicate_utils.h \
    include/import_wizard/ImportBehavior.h \
    include/AddTagView.h
SOURCES += \
    src/AnswerView.cpp \
    src/EditView.cpp \
    src/HandwritingArea.cpp \
    src/HomeView.cpp \
    src/LanguageButtons.cpp \
    src/main.cpp \
    src/NetworkReplyReader.cpp \
    src/Parser.cpp \
    src/QuestionView.cpp \
    src/SearchView.cpp \
    src/string_utils.cpp \
    src/Test.cpp \
    src/TestView.cpp \
    src/WordView.cpp \
    src/DatabaseManager.cpp \
    src/AddListView.cpp \
    src/Switcher.cpp \
    src/ClickableLabel.cpp \
    src/import_wizard/DstListPage.cpp \
    src/import_wizard/DuplicatePage.cpp \
    src/import_wizard/SingleImportWizard.cpp \
    src/import_wizard/ListImportWizard.cpp \
    src/import_wizard/BehaviorPage.cpp \
    src/import_wizard/ProgressPage.cpp \
    src/import_wizard/Importer.cpp \
    src/duplicate_utils.cpp \
    src/AddTagView.cpp

RESOURCES += clemanglaise.qrc

QT += network sql
android {
     QT += androidextras
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): cache()

OTHER_FILES += \
    TODO.rst \
    programming_style.rst

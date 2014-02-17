######################################################################
# Automatically generated by qmake (3.0) ven. janv. 17 14:46:54 2014
######################################################################

TEMPLATE = app
TARGET = clemanglaise
INCLUDEPATH += . include

# Input
HEADERS += include/AnswerView.h \
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
           include/ThemeView.h \
           include/WordView.h \
           include/DatabaseManager.h \
           include/AddListView.h \
           include/Switcher.h \
    include/ClickableLabel.h
SOURCES += src/AnswerView.cpp \
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
           src/ThemeView.cpp \
           src/WordView.cpp \
           src/DatabaseManager.cpp \
           src/AddListView.cpp \
           src/Switcher.cpp \
    src/ClickableLabel.cpp

RESOURCES += clemanglaise.qrc

QT += network sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): cache()

OTHER_FILES += \
    TODO.rst

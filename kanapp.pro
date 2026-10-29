CONFIG += qt lrelease embed_translations
QT += widgets
QT += multimedia

RC_FILE = kanapp.rc
ICON = kanapp.icns
SOURCES += \
    main.cpp \
    Kana.cpp \
    KanaList.cpp \
    KanaWidget.cpp \
    MainWindow.cpp \
    MainMenuWidget.cpp \
    QuizWidget.cpp \
    FaderWidget.cpp \
    CountdownWidget.cpp \
    QuizEndWidget.cpp \
    ScoreStorage.cpp \
    Language.cpp

HEADERS += \
    Kana.h \
    KanaList.h \
    KanaWidget.h \
    MainWindow.h \
    MainMenuWidget.h \
    QuizWidget.h \
    FaderWidget.h \
    CountdownWidget.h \
    QuizEndWidget.h \
    ScoreStorage.h \
    Language.h

# English is the source language, the .qm files are embedded under :/i18n/
TRANSLATIONS += \
    translations/kanapp_fr.ts \
    translations/kanapp_ru.ts

RESOURCES += \
    Images.qrc \
    Sons.qrc

OTHER_FILES += \
    PagePrincipale.qml \
    Button.qml

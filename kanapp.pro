CONFIG += qt lrelease embed_translations
QT += widgets
QT += multimedia

RC_FILE = kanapp.rc
ICON = kanapp.icns
SOURCES += \
    main.cpp \
    Kana.cpp \
    ListeKana.cpp \
    WidgetKana.cpp \
    FenetreKana.cpp \
    WidgetPrincipal.cpp \
    WidgetQuizz.cpp \
    FaderWidget.cpp \
    TimeCompteur.cpp \
    WidgetFinQuizz.cpp \
    GestionScore.cpp \
    GestionLangue.cpp

HEADERS += \
    Kana.h \
    ListeKana.h \
    WidgetKana.h \
    FenetreKana.h \
    WidgetPrincipal.h \
    WidgetQuizz.h \
    FaderWidget.h \
    TimeCompteur.h \
    WidgetFinQuizz.h \
    GestionScore.h \
    GestionLangue.h

# le français est la langue des sources, les .qm sont intégrés sous :/i18n/
TRANSLATIONS += \
    traductions/kanapp_en.ts \
    traductions/kanapp_ru.ts

RESOURCES += \
    Images.qrc \
    Sons.qrc

OTHER_FILES += \
    PagePrincipale.qml \
    Button.qml

cmake_minimum_required(VERSION 3.21)
project(kanapp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

qt_add_executable(kanapp WIN32 MACOSX_BUNDLE
    main.cpp
    Kana.cpp Kana.h
    ListeKana.cpp ListeKana.h
    WidgetKana.cpp WidgetKana.h
    FenetreKana.cpp FenetreKana.h
    WidgetPrincipal.cpp WidgetPrincipal.h
    WidgetQuizz.cpp WidgetQuizz.h
    FaderWidget.cpp FaderWidget.h
    TimeCompteur.cpp TimeCompteur.h
    WidgetFinQuizz.cpp WidgetFinQuizz.h
    GestionScore.cpp GestionScore.h
    Images.qrc
    Sons.qrc
)

if(WIN32)
    target_sources(kanapp PRIVATE kanapp.rc)
endif()

if(APPLE)
    set_target_properties(kanapp PROPERTIES MACOSX_BUNDLE_ICON_FILE kanapp.icns)
    set_source_files_properties(kanapp.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    target_sources(kanapp PRIVATE kanapp.icns)
endif()

target_link_libraries(kanapp PRIVATE Qt6::Widgets Qt6::Multimedia)

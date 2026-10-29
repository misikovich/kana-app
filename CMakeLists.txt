cmake_minimum_required(VERSION 3.21)
project(kanapp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)
# lupdate/lrelease run on the build machine: when cross-compiling, take them from
# the host Qt (the Windows Qt archives only contain qtbase and qtmultimedia)
if(CMAKE_CROSSCOMPILING)
    find_package(Qt6LinguistTools REQUIRED
        PATHS "${QT_HOST_PATH_CMAKE_DIR}" NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
else()
    find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
endif()

qt_add_executable(kanapp WIN32 MACOSX_BUNDLE
    main.cpp
    Kana.cpp Kana.h
    KanaList.cpp KanaList.h
    KanaWidget.cpp KanaWidget.h
    MainWindow.cpp MainWindow.h
    MainMenuWidget.cpp MainMenuWidget.h
    QuizWidget.cpp QuizWidget.h
    FaderWidget.cpp FaderWidget.h
    CountdownWidget.cpp CountdownWidget.h
    QuizEndWidget.cpp QuizEndWidget.h
    ScoreStorage.cpp ScoreStorage.h
    Language.cpp Language.h
    Images.qrc
    Sons.qrc
)

# English is the source language; the .qm files are embedded under :/i18n/
qt_add_translations(kanapp
    TS_FILES translations/kanapp_fr.ts translations/kanapp_ru.ts
    RESOURCE_PREFIX /i18n
    LUPDATE_OPTIONS -no-obsolete
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

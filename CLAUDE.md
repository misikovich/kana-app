# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Kanapp: a Qt Widgets desktop app (C++) for learning Japanese kana. It has a browse mode (step through hiragana/katakana and play each one's pronunciation) a practice mode (type the romaji of the kana shown) and a quiz (pick the right romaji out of 4). It was written as a learning project. Code, comments and source UI strings are in **English**; French and Russian are translations. A few leftovers keep their French names: the resource folders and files (`ressources/`, `sons/`, `Sons.qrc`) and the dead files listed at the end.

## Build & run

There are no tests and no linter. The code builds with **Qt 6** (Widgets + Multimedia). `CMakeLists.txt` is the main build; `kanapp.pro` (qmake) still works for native builds, and each must list any new source file.

```sh
# Native Linux build (apt: qt6-base-dev qt6-multimedia-dev qt6-l10n-tools qt6-tools-dev cmake ninja-build)
cmake -S . -B build-linux -G Ninja && cmake --build build-linux && ./build-linux/kanapp

# Windows x86_64 cross-build + portable package -> dist/kanapp-windows.zip
./build-windows.sh
```

How the Windows cross-build works (`build-windows.sh`, `cmake/mingw-w64-x86_64.cmake`):
- The compiler is Ubuntu's `x86_64-w64-mingw32-g++-posix`. It targets msvcrt, the same runtime as Qt's official MinGW builds, so their DLLs are ABI-compatible.
- The Windows Qt comes from Qt's official MinGW binaries (only the `qtbase` and `qtmultimedia` .7z archives), extracted into `~/qt-win` (override with `QT_WIN`). They are under `https://download.qt.io/online/qtsdkrepository/windows_x86/desktop/qt6_<ver>/qt6_<ver>_mingw/`. `aqtinstall` failed to parse that repo, so the archives were fetched by hand.
- Cross-building with Qt 6 needs a **host Qt of exactly the same version** for moc/rcc. That is the distro Qt in `/usr` (`QT_HOST_PATH`), currently 6.11.2. After a distro Qt upgrade, download the matching Windows Qt.
- Qt's `windeployqt` only runs on Windows, so the script does the deployment itself. It copies the plugins (platforms, styles, multimedia, imageformats), then walks the PE imports with `objdump` to collect the Qt, FFmpeg and MinGW runtime DLLs.
- To smoke-test the package: `xvfb-run wine dist/kanapp-windows/kanapp.exe`.

## Architecture

- `main.cpp` creates `MainWindow` (a `QMainWindow`). It owns a `QStackedWidget` with fixed page indices:
  - `0` `MainMenuWidget`: menu with the "learn", "practice" and "quiz" buttons
  - `1` `KanaWidget`: kana browser
  - `2` `PracticeWidget`: the practice (settings page, then the game)
  - `3` `PracticeEndWidget`: end-of-game score screen, shared by practice and quiz
  - `4` `PracticeWidget(true)`: the quiz

  Navigation calls `setCurrentIndex(n)` with these hard-coded indices, so changing the page order means updating every one of those call sites. Each page has its own toolbar (`kanaToolBar` / `practiceToolBar`), and visibility is toggled per page. Every page change runs a fade animation through `FaderWidget`, and `PracticeWidget` uses the same pattern between its settings and game pages.
- Keyboard input for the browser is handled centrally in `MainWindow::keyPressEvent`, not in `KanaWidget`: Left/Right steps through kana, Up/Down switches between Hiragana and Katakana, Space reveals the romaji.
- **Kana data** lives in `KanaList`, which holds parallel hard-coded arrays (hiragana, katakana, and romaji shared by both scripts, 46 each per `HIRAGANA_SIZE`/`KATAKANA_SIZE`). `Kana` is a plain value holder.
- **Audio**: `KanaWidget::pronounce()` plays, through a `QSoundEffect`, `qrc:/sons/ressources/sons/<romaji>.wav`. Each romaji therefore needs a matching `.wav` in `ressources/sons/` and an entry in `Sons.qrc`. (This is why `wi`/`we` are left out.)
- **Practice flow** (`PracticeWidget`): the settings page picks the scripts (hiragana/katakana checkboxes, at least one stays checked), the time per kana (`time_group`, button id = seconds, `0` = unlimited) and the number of cards (`count_group`, 1/2/4). They are saved in `QSettings` under `practice/*`. `startGame()` builds one `KanaCard` per kana. Each card is a clickable QFrame with its own 1 s QTimer, and it emits `timeUp()` when its time runs out. The player clicks a card (or types 1–4) to select it; answers are checked against the selected card on each keystroke (`textEdited` → `handleAnswer`). A timeout counts as a mistake and replaces only that card. In unlimited mode there is no timer: a complete wrong answer is a mistake, and the Skip button (or Esc) fades in once the selected card has been shown for 10 s. After more than 5 mistakes, `endGame()` saves the best score and emits `gameOver()`, and `MainWindow` then shows page 3.
- **Quiz** (`PracticeWidget(true)`, `quiz_mode`): the same widget with a single card and 4 answer buttons (`choice_group`, also keys 1–4 through `QShortcut`s) instead of the line edit. The "kana shown" row and the Skip button are hidden. `fillChoices()` puts the right romaji and 3 different wrong ones from the chosen scripts on the buttons. A wrong choice or a timeout is a mistake. In quiz mode the keyboard focus must stay off the hidden line edit, which would otherwise swallow the digit keys. Its settings are saved under `quiz/*`.
- **Score persistence** (`ScoreStorage`): free functions that read and write a single integer best score in a plain-text file at `scoreFilePath(unlimited)`, which is `QStandardPaths::AppDataLocation/score_data` for practice and `quiz_score_data` for the quiz, each with an `_unlimited` variant for unlimited-time games (e.g. `%APPDATA%/Najimi/Kana Learning/` on Windows). The repo's `score_data` file is unused.
- **Translations** (Qt Linguist): English is the source language. Every UI string goes through `tr()`, and `translations/kanapp_fr.ts` / `kanapp_ru.ts` hold the translations. Both builds compile them with `lrelease` and embed them under `:/i18n/` (CMake `qt_add_translations`, qmake `embed_translations`). `Language` installs the `QTranslator` and stores the choice in `QSettings` (key `language`). On first launch it uses the system language if available, otherwise English. The "Language" menu in `MainWindow` switches language live. Each widget handles `QEvent::LanguageChange` in `changeEvent()` and resets its texts in `retranslate()`, so set UI text there rather than in constructors. Text that depends on state (score, show/hide romaji) has to be rebuilt from member variables. The kana combo box is index-based (0 hiragana, 1 katakana) because its item texts are translated. After changing strings, run `cmake --build build-linux --target update_translations`, fill in the new `<translation>` entries, and keep the `.ts` files in git. When cross-compiling, `LinguistTools` comes from the host Qt (`QT_HOST_PATH_CMAKE_DIR`), because the Windows Qt archives don't include qttools. Needs apt `qt6-l10n-tools qt6-tools-dev`.
- Connections use the old string-based `SIGNAL()`/`SLOT()` macros, which the compiler does not check. After renaming a signal or slot, update the macros and watch the console for `QObject::connect: No such signal/slot` warnings at runtime.
- Resource paths keep the full on-disk prefix: `:/images/ressources/images/...`, `:/sons/ressources/sons/...`.

## Dead / unwired files

These files are not listed in `kanapp.pro` and are not used by the build: `KanaLabel.*`, `ListeDeroulante.h`, `Score.qrc` (empty), and `PagePrincipale.qml`/`Button.qml` (listed only as `OTHER_FILES`). `KanaLabel.*` does not compile.

#!/usr/bin/env bash
# Cross-compile Kanapp for Windows x86_64 from Linux and package a portable zip.
#
# Needs: mingw-w64 (posix), cmake, ninja, zip, a host Qt 6 (same version as the
# Windows Qt, for moc/rcc) and the Windows MinGW Qt 6 (qtbase + qtmultimedia)
# extracted into $QT_WIN. See CLAUDE.md.
set -euo pipefail

SRC="$(cd "$(dirname "$0")" && pwd)"
QT_WIN="${QT_WIN:-$HOME/qt-win}"
QT_HOST="${QT_HOST:-/usr}"
QT_HOST_CMAKE="${QT_HOST_CMAKE:-/usr/lib/x86_64-linux-gnu/cmake}"
BUILD="$SRC/build-windows"
DIST="$SRC/dist/kanapp-windows"
OBJDUMP=x86_64-w64-mingw32-objdump

cmake -S "$SRC" -B "$BUILD" -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE="$SRC/cmake/mingw-w64-x86_64.cmake" \
    -DCMAKE_PREFIX_PATH="$QT_WIN" -DCMAKE_FIND_ROOT_PATH="$QT_WIN" \
    -DQT_HOST_PATH="$QT_HOST" -DQT_HOST_PATH_CMAKE_DIR="$QT_HOST_CMAKE"
cmake --build "$BUILD"

rm -rf "$DIST"
mkdir -p "$DIST"/{platforms,styles,multimedia,imageformats}
x86_64-w64-mingw32-strip -o "$DIST/kanapp.exe" "$BUILD/kanapp.exe"
cp "$QT_WIN/plugins/platforms/qwindows.dll"            "$DIST/platforms/"
cp "$QT_WIN/plugins/styles/qmodernwindowsstyle.dll"    "$DIST/styles/"
cp "$QT_WIN/plugins/multimedia/"*.dll                  "$DIST/multimedia/"
cp "$QT_WIN/plugins/imageformats/qico.dll"             "$DIST/imageformats/"

# Copy every non-system DLL the exe and plugins import, recursively.
GCC_DLLS="$(dirname "$(x86_64-w64-mingw32-g++-posix -print-libgcc-file-name)")"
SEARCH=("$QT_WIN/bin" "$GCC_DLLS" /usr/x86_64-w64-mingw32/lib)
queue=("$DIST/kanapp.exe" "$DIST"/*/*.dll)
while ((${#queue[@]})); do
    bin="${queue[0]}"; queue=("${queue[@]:1}")
    for dll in $("$OBJDUMP" -p "$bin" | awk '/DLL Name:/ {print $3}'); do
        [[ -e "$DIST/$dll" ]] && continue
        for dir in "${SEARCH[@]}"; do
            if [[ -e "$dir/$dll" ]]; then
                cp "$dir/$dll" "$DIST/"
                queue+=("$DIST/$dll")
                break
            fi
        done
    done
done

(cd "$SRC/dist" && rm -f kanapp-windows.zip && zip -qr kanapp-windows.zip kanapp-windows)
echo "Built $SRC/dist/kanapp-windows.zip"

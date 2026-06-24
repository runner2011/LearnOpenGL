#!/usr/bin/env sh
set -eu

BUILD_DIR="${BUILD_DIR:-build}"
CONFIG="${CONFIG:-Debug}"
TARGET="LearnOpenGL"
RUN_AFTER_BUILD=0

for arg in "$@"; do
    case "$arg" in
        Debug|Release|RelWithDebInfo|MinSizeRel)
            CONFIG="$arg"
            ;;
        clean)
            TARGET="clean"
            ;;
        run)
            RUN_AFTER_BUILD=1
            ;;
        *)
            echo "Unknown argument: $arg" >&2
            echo "Usage: ./build.sh [Debug|Release|RelWithDebInfo|MinSizeRel] [clean] [run]" >&2
            exit 2
            ;;
    esac
done

cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$CONFIG"
cmake --build "$BUILD_DIR" --config "$CONFIG" --target "$TARGET"

if [ "$RUN_AFTER_BUILD" = "1" ] && [ "$TARGET" != "clean" ]; then
    if [ -x "$BUILD_DIR/bin/LearnOpenGL" ]; then
        (cd "$BUILD_DIR/bin" && ./LearnOpenGL)
    elif [ -x "$BUILD_DIR/bin/$CONFIG/LearnOpenGL" ]; then
        (cd "$BUILD_DIR/bin/$CONFIG" && ./LearnOpenGL)
    else
        echo "Build succeeded, but executable was not found under $BUILD_DIR/bin" >&2
        exit 1
    fi
fi

#!/bin/bash

# ----------------------------------------------------
# ROS 2 워크스페이스 정리 스크립트 (make clean 역할)
# ----------------------------------------------------

# 워크스페이스 루트 확인
if [ ! -d "src" ]; then
    echo "ERROR: 이 스크립트는 ROS 2 워크스페이스 루트에서 실행해야 합니다."
    exit 1
fi

echo "🧹 워크스페이스 정리를 시작합니다..."

# build 디렉토리 삭제
BUILD_DIR="build"

if [ -d "$BUILD_DIR" ]; then
    echo "📦 $BUILD_DIR 디렉토리를 삭제합니다..."
    rm -rf "$BUILD_DIR"
    echo "✅ $BUILD_DIR 디렉토리가 삭제되었습니다."
else
    echo "ℹ️  $BUILD_DIR 디렉토리가 없습니다. (이미 정리됨)"
fi

# build 디렉토리 삭제
CACHE_DIR=".cache"

if [ -d "$CACHE_DIR" ]; then
    echo "📦 $CACHE_DIR 디렉토리를 삭제합니다..."
    rm -rf "$CACHE_DIR"
    echo "✅ $CACHE_DIR 디렉토리가 삭제되었습니다."
else
    echo "ℹ️  $CACHE_DIR 디렉토리가 없습니다. (이미 정리됨)"
fi

# log 디렉토리 삭제
LOG_DIR="log"

if [ -d "$LOG_DIR" ]; then
    echo "📋 $LOG_DIR 디렉토리를 삭제합니다..."
    rm -rf "$LOG_DIR"
    echo "✅ $LOG_DIR 디렉토리가 삭제되었습니다."
else
    echo "ℹ️  $LOG_DIR 디렉토리가 없습니다. (이미 정리됨)"
fi

# install 디렉토리 삭제
INSTALL_DIR="install"

if [ -d "$INSTALL_DIR" ]; then
    echo "📥 $INSTALL_DIR 디렉토리를 삭제합니다..."
    rm -rf "$INSTALL_DIR"
    echo "✅ $INSTALL_DIR 디렉토리가 삭제되었습니다."
else
    echo "ℹ️  $INSTALL_DIR 디렉토리가 없습니다. (이미 정리됨)"
fi

# compile_commands.json도 삭제 (선택사항)
if [ -f "compile_commands.json" ]; then
    echo "📄 compile_commands.json 파일을 삭제합니다..."
    rm -f "compile_commands.json"
    echo "✅ compile_commands.json 파일이 삭제되었습니다."
fi

echo "========================================================================"
echo "✅ 워크스페이스 정리가 완료되었습니다."
echo "   다음 빌드를 위해 'make_catkin.sh'를 실행하세요."
echo "========================================================================"


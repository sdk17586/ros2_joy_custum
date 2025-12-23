#!/bin/bash

# ----------------------------------------------------
# ROS 2 워크스페이스 빌드 및 개발 환경 설정 스크립트
# ----------------------------------------------------

# 워크스페이스 루트 확인
if [ ! -f "src/CMakeLists.txt" ] && [ ! -d "src" ]; then
    echo "ERROR: 이 스크립트는 ROS 2 워크스페이스 루트에서 실행해야 합니다."
    exit 1
fi

# ROS2 환경 소스 (Python 모듈 경로 설정을 위해 필요)
if [ -f "/opt/ros/humble/setup.bash" ]; then
    source /opt/ros/humble/setup.bash
    echo "✅ ROS2 Humble 환경을 소스했습니다."
else
    echo "⚠️ 경고: ROS2 환경을 찾을 수 없습니다. 빌드가 실패할 수 있습니다."
fi

echo "🚀 [1/3] 워크스페이스 빌드를 시작합니다..."

# 빌드 명령어:
# 1. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 플래그를 추가하여 clangd용 JSON 파일 생성
# 2. --symlink-install 옵션을 사용하여 개발 중 라이브러리/실행 파일 업데이트를 용이하게 함
colcon build --symlink-install --cmake-args -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 마지막 빌드 명령의 성공 여부 확인
if [ $? -ne 0 ]; then
    echo "❌ 빌드에 실패했습니다. 스크립트를 종료합니다."
    exit 1
fi

echo "✅ 빌드가 성공적으로 완료되었습니다."

# ----------------------------------------------------
# 2. clangd 설정 파일 (compile_commands.json) 병합
# ----------------------------------------------------

echo "🔗 [2/3] 모든 패키지의 compile_commands.json을 병합합니다."

JSON_DEST="compile_commands.json"
TEMP_JSON="/tmp/compile_commands_merged.json"

# 기존 파일/링크 삭제
if [ -f "$JSON_DEST" ] || [ -L "$JSON_DEST" ]; then
    rm -f "$JSON_DEST"
fi

# Python으로 모든 패키지의 compile_commands.json 병합
python3 << 'EOF'
import json
import os
import glob

compile_commands = []

# build/ 디렉토리에서 모든 패키지의 compile_commands.json 찾기
pattern = "build/*/compile_commands.json"
json_files = glob.glob(pattern)

if not json_files:
    print("⚠️  경고: compile_commands.json 파일을 찾을 수 없습니다.")
    exit(0)

print(f"📦 {len(json_files)}개의 패키지에서 compile_commands.json을 찾았습니다.")

for json_file in sorted(json_files):
    package_name = json_file.split('/')[1]
    try:
        with open(json_file, 'r') as f:
            data = json.load(f)
            if isinstance(data, list):
                compile_commands.extend(data)
                print(f"   ✓ {package_name}: {len(data)}개 항목 추가")
    except Exception as e:
        print(f"   ⚠️  {package_name}: 파일 읽기 실패 - {e}")

# 병합된 결과를 임시 파일에 저장
with open('/tmp/compile_commands_merged.json', 'w') as f:
    json.dump(compile_commands, f, indent=2)

print(f"✅ 총 {len(compile_commands)}개의 컴파일 명령을 병합했습니다.")
EOF

# 병합된 파일을 워크스페이스 루트로 복사
if [ -f "$TEMP_JSON" ]; then
    mv "$TEMP_JSON" "$JSON_DEST"
    echo "✅ '${JSON_DEST}' 파일이 성공적으로 생성되었습니다. (clangd 준비 완료)"
else
    echo "⚠️  경고: compile_commands.json 병합에 실패했습니다."
fi


source ./install/setup.bash

echo "========================================================================"
echo "✅ 빌드 및 환경 설정이 완료되었습니다."
echo "   (주의: 'source ./catkin.sh' 로 스크립트를 실행시켜야합니다."
echo "========================================================================"
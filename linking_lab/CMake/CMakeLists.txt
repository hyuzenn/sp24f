cmake_minimum_required(VERSION 3.5.0)
project(leveldb_bench VERSION 0.1.0 LANGUAGES C CXX)

# 디버그 모드 설정
set(CMAKE_BUILD_TYPE Debug)

# 실행 파일 생성 및 라이브러리 링크
add_executable(leveldb_bench ../main.cpp)
target_link_libraries(leveldb_bench leveldb)

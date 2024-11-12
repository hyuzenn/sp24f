cmake_minimum_required(VERSION 3.10)
project(leveldb_bench VERSION 0.1.0 LANGUAGES C CXX)

# C++17 표준 사용
set(CMAKE_CXX_STANDARD 17)

# 디버깅을 위한 컴파일러 플래그 설정
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# include 디렉토리 설정
include_directories("/mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/include")

# main.cpp 컴파일 및 링크
add_executable(leveldb_bench /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/main.cpp)

# LevelDB 라이브러리 및 헤더 포함
target_include_directories(leveldb_bench PRIVATE /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/include)

# 라이브러리 링크 경로 설정 및 LevelDB 라이브러리 링크
target_link_directories(leveldb_bench PRIVATE
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/build
)
target_link_libraries(leveldb_bench PRIVATE leveldb)

find_package(Snappy REQUIRED)
target_link_libraries(leveldb_bench PRIVATE snappy)
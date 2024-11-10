cmake_minimum_required(VERSION 3.5.0)
project(leveldb_bench VERSION 0.1.0 LANGUAGES C CXX)

# 소스 파일 경로
set(SOURCE_FILES ../main.cpp)

# 디버그 모드 설정
set(CMAKE_BUILD_TYPE Debug)

# 실행 파일 생성
add_executable(leveldb_bench ${SOURCE_FILES})

# write_batch.cc 파일을 소스에 추가하여 링크
# 추가적으로 필요한 LevelDB 소스 파일들 링크
target_sources(leveldb_bench PRIVATE
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/write_batch.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/memtable.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/coding.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/arena.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/dbformat.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/logging.cc 
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/third_party/benchmark/src/string_util.cc
)

# 라이브러리 및 include 디렉토리 설정
include_directories(
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/include
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util 
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/port
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db
)

# 라이브러리 링크 경로 설정
link_directories(/mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/build)

# 실행 파일과 라이브러리 링크
target_link_libraries(leveldb_bench leveldb)

# 실행 파일 출력 경로 설정
set_target_properties(leveldb_bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY /mnt/c/Users/huj22/source/repos/sp24f/linking_lab)

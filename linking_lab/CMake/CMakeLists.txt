# cmake_minimum_required(VERSION 3.5.0)
# project(leveldb_bench VERSION 0.1.0 LANGUAGES C CXX)

# # C++ 표준 설정
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # 소스 파일 경로
# set(SOURCE_FILES ../main.cpp)

# # 디버그 모드 설정
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# # 실행 파일 생성
# add_executable(leveldb_bench /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/leveldbutil.cc)

# # 가장 최근에 생성된 .log 파일을 찾는 명령 실행
# execute_process(
#     COMMAND find /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/build/mydb -name "*.log" -type f -print0 | xargs -0 ls -t | head -n 1
#     OUTPUT_VARIABLE RECENT_LOG_FILE
#     OUTPUT_STRIP_TRAILING_WHITESPACE
# )

# # RECENT_LOG_FILE 경로 출력 (디버깅용)
# message(STATUS "Recent log file: ${RECENT_LOG_FILE}")

# # 가장 최근에 생성된 log 파일을 dump 명령에 사용
# add_custom_target(run_bench
#     COMMAND ./leveldb_bench dump ${RECENT_LOG_FILE}
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )

# # LevelDB의 필수 소스 파일들 추가
# target_sources(leveldb_bench PRIVATE
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/write_batch.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/memtable.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/coding.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/arena.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/dbformat.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/logging.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/third_party/benchmark/src/string_util.cc
# )

# # include 디렉토리 설정
# include_directories("/mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/include")

# # 라이브러리 링크 경로 설정 및 LevelDB 라이브러리 링크
# target_link_directories(leveldb_bench PRIVATE
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/build
# )
# target_link_libraries(leveldb_bench leveldb)

# # 실행 파일 출력 경로 설정
# set_target_properties(leveldb_bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/build)


cmake_minimum_required(VERSION 3.5.0)
project(leveldb_bench VERSION 0.1.0 LANGUAGES C CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 파일 경로
set(SOURCE_FILES ../main.cpp)

# 디버그 모드 설정
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# 실행 파일 생성
# add_executable(leveldb_bench ${SOURCES} ${LEVELDB_SRC})
add_executable(leveldb_bench /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/leveldbutil.cc)

# 가장 최근에 생성된 .log 파일을 찾는 명령 실행
# execute_process(
#     COMMAND sh -c "find /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/build/mydb -name \"*.log\" -type f -print0 | xargs -0 ls -t | head -n 1"
#     OUTPUT_VARIABLE RECENT_LOG_FILE
#     OUTPUT_STRIP_TRAILING_WHITESPACE
# )

# # RECENT_LOG_FILE 경로 출력 (디버깅용)
# message(STATUS "Recent log file: ${RECENT_LOG_FILE}")

# # 가장 최근에 생성된 log 파일을 dump 명령에 사용
# add_custom_target(run_bench
#     COMMAND ./leveldb_bench dump ${RECENT_LOG_FILE}
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )

# add_custom_target(run_bench
#     COMMAND ./leveldb_bench dump /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/build/mydb/000003.log
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
# )

# LevelDB의 필수 소스 파일들 추가
target_sources(leveldb_bench PRIVATE
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/write_batch.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/memtable.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/coding.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/arena.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/dbformat.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/logging.cc
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/third_party/benchmark/src/string_util.cc
)

# include 디렉토리 설정
include_directories("/mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/include")

# 라이브러리 링크 경로 설정 및 LevelDB 라이브러리 링크
target_link_directories(leveldb_bench PRIVATE
    /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/build
)
target_link_libraries(leveldb_bench leveldb)

# 실행 파일 출력 경로 설정
set_target_properties(leveldb_bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/build)

#######################################################################

# cmake_minimum_required(VERSION 3.5.0)
# project(leveldb_bench VERSION 0.1.0 LANGUAGES C CXX)

# # C++ 표준 설정
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # 소스 파일 경로
# set(SOURCE_FILES ../main.cpp)

# # 디버그 모드 설정
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# # 실행 파일 생성
# add_executable(leveldb_bench ${SOURCE_FILES})

# # LevelDB의 필수 소스 파일들 추가
# target_sources(leveldb_bench PRIVATE
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/write_batch.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/memtable.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/coding.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/arena.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/dbformat.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util/logging.cc
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/third_party/benchmark/src/string_util.cc
# )

# # include 디렉토리 설정
# target_include_directories(leveldb_bench PRIVATE
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/include
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/util
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/port
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db
# )

# # 라이브러리 링크 경로 설정 및 LevelDB 라이브러리 링크
# target_link_directories(leveldb_bench PRIVATE
#     /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/build
# )
# target_link_libraries(leveldb_bench leveldb)

# # 실행 파일 출력 경로 설정
# set_target_properties(leveldb_bench PROPERTIES RUNTIME_OUTPUT_DIRECTORY /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/build)
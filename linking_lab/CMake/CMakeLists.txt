# cmake_minimum_required(VERSION 3.5.0)
# project(leveldb_bench VERSION 0.1.0 LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# add_executable(leveldb_bench ../main.cpp)

# include_directories(/mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/include)
# link_directories(/mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/build)
# target_link_libraries(leveldb_bench PRIVATE leveldb)


################### basic
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
# add_executable(leveldb_bench /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/main.cpp)
# # add_executable(leveldb_bench /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/leveldbutil.cc)

# # 가장 최근에 생성된 .log 파일을 찾는 명령 실행
# execute_process(
#     COMMAND find /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/build/mydb -name "*.log" -type f -print0
#     COMMAND xargs -0 ls -t
#     COMMAND head -n 1
#     OUTPUT_VARIABLE RECENT_LOG_FILE
#     OUTPUT_STRIP_TRAILING_WHITESPACE
# )

# # RECENT_LOG_FILE 경로 출력 (디버깅용)
# message(STATUS "Recent log file: ${RECENT_LOG_FILE}")

# # RECENT_LOG_FILE을 leveldbutil.cc에서 사용할 수 있도록 정의
# set(DEFAULT_LOG_PATH ${RECENT_LOG_FILE})

# # leveldbutil.cc에 default_log_path를 전달
# add_definitions(-DDEFAULT_LOG_PATH="${DEFAULT_LOG_PATH}")




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
# # add_executable(leveldb_bench ${SOURCES} ${LEVELDB_SRC})
# add_executable(leveldb_bench /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/leveldb/db/leveldbutil.cc)

# add_custom_target(run_bench
#     COMMAND ./leveldb_bench dump /mnt/c/Users/huj22/source/repos/sp24f/linking_lab/build/mydb/000003.log
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
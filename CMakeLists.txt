cmake_minimum_required(VERSION 3.10)
project(RoundingSatWithPreprocessor)

set(CMAKE_CXX_STANDARD 14)
get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
message(STATUS "languages: ${languages}")

include_directories(
    include
    papilo/src
    papilo/external
)

# Boost
SET (BOOST_ROOT "/usr/local/Cellar/boost")
SET (BOOST_INCLUDEDIR "/usr/local/Cellar/boost/include")
SET (BOOST_LIBRARYDIR "/usr/local/Cellar/boost/lib")

SET (BOOST_MIN_VERSION "1.55.0")
set (Boost_NO_BOOST_CMAKE ON)
FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED)

message(STATUS "Setting up BOOST")
message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Papilo
add_subdirectory(papilo)
set(PAPILO_DIR ${CMAKE_BINARY_DIR})

find_package(PAPILO CONFIG)


file(GLOB SOURCES "src/*.cpp")


# SET (PAPILO_DIR "/${CMAKE_CURRENT_SOURCE_DIR}/../papilo/build")
# find_package(PAPILO REQUIRED PATHS ${PAPILO_DIR})
# include_directories(${papilo_INCLUDE_DIRS})
# link_directories(${papilo_LIBRARY_DIRS})

# message(STATUS "Setting up papilo")
# message(STATUS " Includes - ${PAPILO_INCLUDE_DIRECTORIES}")
# message(STATUS " Library  - ${PAPILO_DIR}")

# foreach(dir ${papilo_LIBRARY_DIRS})
#     message("a")
# endforeach()



add_executable(RoundingSatWithPreprocessor ${SOURCES})
target_link_libraries(RoundingSatWithPreprocessor ${PAPILO_IMPORTED_TARGETS})
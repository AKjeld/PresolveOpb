cmake_minimum_required(VERSION 3.10)
project(PresolveOpb)

set(CMAKE_CXX_STANDARD 17)
get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
message(STATUS "languages: ${languages}")

include_directories(
    include
    papilo/src
    papilo/external
)

# Boost
SET (BOOST_ROOT "/usr/local/Cellar/boost/1.75.0_2")
SET (BOOST_INCLUDEDIR "/usr/local/Cellar/boost/include/1.75.0_2")
SET (BOOST_LIBRARYDIR "/usr/local/Cellar/boost/lib/1.75.0_2")

set (Boost_NO_BOOST_CMAKE ON)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED serialization program_options iostreams)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Fatal error: Boost (version >= 1.55) required.")
else()
  message(STATUS "Setting up BOOST")
  message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
  message(STATUS " Library  - ${Boost_LIBRARY_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif (NOT Boost_FOUND)



# include_directories(${Boost_INCLUDE_DIR})
# link_directories(${Boost_LIBRARY_DIRS})

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



add_executable(PresolveOpb ${SOURCES})
target_link_libraries(PresolveOpb ${PAPILO_IMPORTED_TARGETS} ${Boost_LIBRARIES} Boost::serialization Boost::program_options Boost::iostreams)
# target_link_libraries(PresolveOpb PRIVATE Boost::serialization)
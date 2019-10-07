cmake_minimum_required(VERSION 3.0)
project(SHMFalcon)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(Boost)
find_library(PTHREAD pthread)
find_library(LIBNIFALCON nifalcon)
message(STATUS ${LIBNIFALCON})

add_library(SHMFalcon SHARED src/SHMFalcon.cpp)
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SHMFalcon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(SHMFalcon ${Boost_LIBRARIES} ${PTHREAD} ${LIBNIFALCON})

add_executable(mainTest tests/main.cpp)
target_link_libraries(mainTest SHMFalcon)
target_link_libraries(mainTest ${Boost_LIBRARIES} ${PTHREAD} rt)
target_include_directories(mainTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

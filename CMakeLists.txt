cmake_minimum_required(VERSION 3.0)
project(SHMFalcon)
set (CMAKE_CXX_STANDARD 11)

find_package(Boost)

add_library(SHMFalcon SHARED src/SHMFalcon.cpp)
target_link_libraries(SHMFalcon ${Boost_LIBRARIES})
target_include_directories(SHMFalcon PUBLIC ${CMAKE_ROOT_DIR}/include/SHMFalcon.hpp)

add_executable(mainTest tests/main.cpp)
target_link_libraries(mainTest SHMFalcon)
target_link_libraries(mainTest ${Boost_LIBRARIES} rt)
target_include_directories(mainTest PUBLIC ${CMAKE_ROOT_DIR}/include/SHMFalcon.hpp)
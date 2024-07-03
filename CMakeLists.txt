cmake_minimum_required(VERSION 3.1)
project(sftrimmer)
find_package(PkgConfig REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_executable(sftrimmer sftrimmer.cpp)

pkg_check_modules(giglib REQUIRED IMPORTED_TARGET gig)
pkg_check_modules(sndfile REQUIRED IMPORTED_TARGET sndfile)
target_include_directories(sftrimmer PRIVATE ${giglib_INCLUDEDIR} ${sndfile_INCLUDEDIR})
target_link_libraries(sftrimmer PRIVATE PkgConfig::giglib PkgConfig::sndfile)

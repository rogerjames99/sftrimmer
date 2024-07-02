cmake_minimum_required(VERSION 3.1)
project(sftrimmer)
find_package(PkgConfig REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_executable(sftrimmer sftrimmer.cpp)

pkg_check_modules(libgig REQUIRED gig)
pkg_check_modules(sndfile REQUIRED sndfile)
target_compile_definitions(sftrimmer PRIVATE ${libgig_LDFLAGS} ${sndfile_LDFLAGS})
target_link_libraries(sftrimmer PRIVATE libgig sndfile)
target_include_directories(sftrimmer PRIVATE ${libgig_INCLUDEDIR} ${sndfile_INCLUDEDIR})

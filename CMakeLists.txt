cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(TAdaptiveGrid3D)

set(CMAKE_CXX_STANDARD 17)

if(DEFINED ENV{ROOTSYS})
    message("Found ROOTSYS at $ENV{ROOTSYS}")
else()
    message("ROOTSYS not found.  Searching in /cern.")
    message("  => This will match the first ROOT install it finds, which might not be what you want!")
    list(APPEND CMAKE_PREFIX_PATH /cern)
endif()

find_package(ROOT 6.16 CONFIG REQUIRED) 


# Include directories
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set output paths for libraries and executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# create library
set(LIB_SOURCES src/TAdaptiveGrid3D.cpp)
add_library(TAdaptiveGrid3D SHARED ${LIB_SOURCES})
target_link_libraries(TAdaptiveGrid3D ROOT::Core ROOT::MathCore ROOT::Matrix)

# install library
install(TARGETS TAdaptiveGrid3D DESTINATION lib)
install(FILES include/TAdaptiveGrid3D.h DESTINATION include)

# testing
function(add_test_executable target)
    add_executable(${target} test/${target}.cpp)
    target_link_libraries(${target} TAdaptiveGrid3D)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
    add_test(NAME ${target} COMMAND ${target})
endfunction()
enable_testing()

# hello world
add_test_executable(hello_world)
